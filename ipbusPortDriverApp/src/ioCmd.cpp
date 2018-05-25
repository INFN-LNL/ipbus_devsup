/*
 * ioCmd.cpp
 *
 *  Created on: Feb 10, 2016
 *      Author: damiano
 */


#include "ioCmd.h"
#include "baccoIo.h"
#include <inttypes.h>
#include <endian.h>

//#include "../librfc/rfcUsbPacket.h"
#include "baccoObj.h"

 bool ioCmd::matches(/*const rfcUsbPacket &tr*/) const
 {
	return true;
 	//return 	_inx == tr.get_hw_port() &&
 	//		_address >= tr.get_start_address() &&
 	//		_address < tr.get_start_address() + tr.get_data_size();
 }

/****************************************************************/

class baccoIoCmd : public ioCmd
{
protected:
	baccoIo	 *ioport;
public:
	baccoIoCmd(baccoIo *io, asynUser *asyn): ioCmd(io, asyn), ioport(io)	{}
	//unsigned getAddress() { return _address; }
	virtual ~baccoIoCmd() {}
	virtual asynStatus io(int dir, void *value, const char *type, size_t nelm=1){ return asynSuccess; };
    virtual bool isValid() { return true; }
	virtual bool matches(/*const rfcUsbPacket &*/) const;
};




/*****************************************************************************/

class regIoCmd : public ioCmd
{
protected:
	int width;    		// width in bits of the field
	int shift;	  		// position of the field within the register
	char sign;	  		// 's' = signed, 'u'=unsigned
	int decbits;  		// number of decimal bits (for fields representing fixed float
	epicsInt32 i32read(uint32_t reg);
	epicsFloat64 f64read(uint32_t reg);
	uint32_t i32write(epicsInt32 reg);
	uint32_t f64write(epicsFloat64 val);
public:
	regIoCmd(baccoIo *io, asynUser *asyn, const char *parameters);
	virtual ~regIoCmd() {};
	virtual asynStatus io(int dir, void *value, const char *type, size_t nelm=1);
	bool matches(/*const rfcUsbPacket &tr*/) const;
};

 regIoCmd::regIoCmd(baccoIo *io, asynUser *asyn, const char *parameters): ioCmd(io, asyn),
 		width(32), shift(0), sign('u'), decbits(0)
 {
 	_valid = sscanf(parameters, "%i,%i,%i,%i,%[us],%i", &_inx,
 			&_address, &width, &shift, &sign, &decbits) >= 2;
 }

asynStatus regIoCmd::io(int dir, void *value, const char *type, size_t nelm)
{
	uint32_t reg, oreg;
	uint32_t msk = width < 32 ? (1<<width) - 1 : 0xffffffff;
	if (dir) {

		reg = ioport->read(_inx, _address);
		reg >>= shift;
		reg &= msk;
		if (type == ioc_int32)
			*(epicsInt32*)value = i32read(reg);
		else if (type == ioc_float64)
			*(epicsFloat64*)value = f64read(reg);
		else
			return asynError;
	} else {
		if (type == ioc_int32)
			reg = i32write(*(epicsInt32*)value);
		else if (type == ioc_float64)
			reg = f64write(*(epicsFloat64*)value);
		else
			return asynError;
		if (width < 32) {
			oreg = ioport->read(_inx, _address);
			reg &= msk;
			reg <<= shift;
			reg |= oreg & ~(msk << shift);
		}
		ioport->write(_inx, _address, reg);
	}
	return asynSuccess;
}

epicsInt32 regIoCmd::i32read(uint32_t reg)
{
	epicsInt32 res = reg;
	if (width < 32)
		if (sign == 's' && reg >= uint32_t(1 << (width-1)))
			res -= (1 << width);
	return res >> decbits;
}

epicsFloat64 regIoCmd::f64read(uint32_t reg)
{
	epicsFloat64 res = reg;
	if (width < 32)
		if (sign == 's' && res >= 1 << (width-1))
			res -= (1 << width);
	return res / (1 << decbits);
}


uint32_t regIoCmd::i32write(epicsInt32 val)
{
	uint32_t reg = val;
	if (width < 32)
		if (sign == 's' && val < 0)
			reg += (1 << width);
	reg = val << decbits;
	return reg;
}

uint32_t regIoCmd::f64write(epicsFloat64 val)
{
	uint32_t reg = val * (1 << decbits);
	if (width < 32)
		if (sign == 's' && val < 0)
			reg += (1 << width);
	return reg;
}
// DA MODIFICARE !!!!!!!!!!!!!!!!!!!!!!!
bool regIoCmd::matches(/*const rfcUsbPacket &tr*/) const
{
	return true;
	//return tr.get_hw_interface() == ttype_mem && ioCmd::matches(tr);
}

/*****************************************************************************/

/*
 * syntax:
 *
 *  regArray(<register port>,<port address>,[<size>])
 *  To be used by waveform, so the size comes from RW requests.
 */

class ioCmdRegisterArray: public ioCmd
{
	unsigned _nelm;
	uint32_t *_lbuf;

public:
	ioCmdRegisterArray(baccoIo *io, asynUser *asyn, const char *parameters);
	asynStatus io(int dir, void *value, const char *type, size_t nelm);
};


ioCmdRegisterArray::ioCmdRegisterArray(baccoIo *io, asynUser *asyn, const char *parameters) : ioCmd(io, asyn), _nelm(0), _lbuf(0)
{
	_valid = sscanf(parameters, "%i,%i,%i", &_inx, &_address, &_nelm) >= 2;
	if (_nelm)
		_lbuf = (uint32_t *)realloc(_lbuf, _nelm*sizeof(uint32_t));
}

asynStatus ioCmdRegisterArray::io(int dir, void *value, const char *type, size_t nelm)
{

	int err;
	if (nelm == 0)
		nelm = _nelm;
	if (nelm == 0){
		epicsStdoutPrintf("regArray: requested 0 size data transfer\n");
		return asynSuccess;
	}
	if (dir) {
		if (nelm>_nelm){
			_nelm = nelm;
			_lbuf = (uint32_t *)realloc(_lbuf, _nelm*sizeof(uint32_t));
		}
		if ((err = ioport->read(_inx, _address, _lbuf, nelm)))
			return asynError;
		if (type == ioc_int32) {
			epicsInt32 *ivalue  = (epicsInt32*)value;
			for(unsigned i=0; i< nelm; i++)
				ivalue[i] = _lbuf[i];
		}
		if (type == ioc_float64) {
			epicsFloat64 *ivalue  = (epicsFloat64*)value;
			for(unsigned i=0; i< nelm; i++)
				ivalue[i] = _lbuf[i];
		}
	} else {
		epicsStdoutPrintf("ioCmdRegisterArray only supports read()\n");
		return asynError;
	}
	return asynSuccess;
}

/*****************************************************************************/

ioCmd * ioCmdFactory(const char *type, const char *params, baccoIo *ioport, asynUser *pasynUser)
{
	ioCmd *cmd = 0;
	if(strcmp(type, "reg") == 0)
		cmd = new regIoCmd(ioport, pasynUser, params);
	if(strcmp(type, "regArray") == 0)
		cmd = new ioCmdRegisterArray(ioport, pasynUser, params);
	return cmd;
}



