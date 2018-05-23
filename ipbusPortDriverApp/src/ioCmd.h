#ifndef RFCSUPPORT_RFCUSBPORTDRIVERAPP_SRC_IOCMD_H_
#define RFCSUPPORT_RFCUSBPORTDRIVERAPP_SRC_IOCMD_H_

#include <asynDriver.h>

class baccoIo;
//class rfcIo;
//class rfcUsbPacket;

/** Structure which represent the "reason" of an asynUser. It holds all
 * the infomation needed to access a specific resource (an advanced version of
 * pasynUser->reason field) and will be referenced trough pasynUser->drvUser
 * field. It represents an extension of asynUser structure.
 */

class ioCmd
{
protected:
	bool _valid;
	unsigned _inx;
	unsigned _address;
	baccoIo	 *ioport;
	asynUser *pasynUser; // linked asynUser structure
public:
	ioCmd(baccoIo *io, asynUser *asyn): _valid(false), _inx(0), _address(0), ioport(io), pasynUser(asyn)
	{}
	unsigned getAddress() { return _address; }
	virtual ~ioCmd() {}
	virtual asynStatus io(int dir, void *value, const char *type, size_t nelm = 1){ return asynSuccess; };
	virtual bool isValid() { return true; }
	virtual bool matches(/*const rfcUsbPacket &*/) const;
};

ioCmd * ioCmdFactory(const char *type, const char *params, baccoIo *ioport, asynUser *pasynUser);









#endif /* RFCSUPPORT_RFCUSBPORTDRIVERAPP_SRC_IOCMD_H_ */
