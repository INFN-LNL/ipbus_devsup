
#include "baccoObj.h"
#include <stdio.h>
#include <malloc.h>
#include <cantProceed.h>
#include <string.h>
#include <errlog.h>
#include "iocAsyn.h"
#include "ioCmd.h"
#include "baccoIo.h"
#include <uhalWrap.h>


/** Wrapper function for creating a new port, it is intended to be used
 *  by asyn Port driver.
 */

extern "C" void * get_ioc_ipbus(void *p, const char *sno)
{
	hwIntWr *hw = new hwIntWr((const char *)p, sno);
	baccoIo *ioc = new baccoIo(hw);
	return ioc;
}

//extern "C" void * get_rfc(void *p, const char *sno)
//{
//	rfcIo *rfc = new rfcIo(p, sno);
//	return rfc;
//}
//
//extern "C" void * get_usb(void *p, const char *sno)xscdvf
//{
//	usbIo *rfc = new usbIo(p, sno);
//	return rfc;
//}

/** Wrapper to connect a port, it is intended to be used
 *  by asyn Port driver.
 */

extern "C" int bacco_connect(void *r)
{
	baccoIo *ioc = static_cast<baccoIo*>(r);
	if (!ioc)
		return 1;
	return ioc->connect();
}

/** Wrapper to disconnect a port, it is intended to be used
 *  by asyn Port driver.
 */

//extern "C" void rfc_disconnect(void *r)
//{
//	rfcIo *rfc = static_cast<rfcIo*>(r);
//	if (rfc)
//		rfc->disconnect();
//}
//
//extern "C" void interruptFire(void *prt, const void *rxd);
//
///** Callback function called whenever a R/W request to the HW has been completed
// *  this will fire an int32array interrupt to inform asynUsers with scan mode
// *  I/O intr.
// */
//
//void rfc_listener_cb(struct rfcUsbPacket *rq,
//					const struct rfcUsbPacket *answ, void *usr)
//{
//	if (rq->get_error_code() == 0 && answ->get_data_direction())
//		interruptFire(((rfcIo *)usr)->asyn_port, answ);
//}


/** this function will be used to build the ioCmd struct related to a specific
 * asynUser given a DBLIK parameter string.
 * It is intended to be called by "create" method of asyn's drvUser interface.
 * At the moment the allowed parameter syntax is one of the following:
 *
 * reg(<hwport>, <address>[,<width>[,<shift>[,<signed>[,<decimal bits>]]]])
 * rfchreg(<address>[,<width>[,<shift>[,<signed>[,<decimal bits>]]]])
 * rfch(<channel>),<parameter>
 * strm(<channel>,<length>)
 * status(<address>,<width>,<shift>)
 *
 *
*/


extern "C"
asynStatus ioc_get_parameter_link(void *io, const char *parameters,
									asynUser *pasynUser)
{
	baccoIo *port = static_cast<baccoIo *>(io);
	ioCmd *cmd = port->factoryIoCmd(parameters, pasynUser);
	if (!cmd || !cmd->isValid()) {
		asynPrint(pasynUser, ASYN_TRACE_ERROR, "parameter parsing error\n");
		return asynDisabled;
	}
	return asynSuccess;
}

static ioCmd * getIoCmd(asynUser *pasynUser)
{
	assert(pasynUser);
	return static_cast<ioCmd *>(pasynUser->drvUser);
}

/** Function called by asyn portDriver interfaces implementations.
 */
extern "C" asynStatus bacco_io(void *obj, asynUser *pasynUser, const char *type,
								int dir, void *value, size_t nelm)
{
	ioCmd *cmd = getIoCmd(pasynUser);
	if (!cmd) {
		snprintf(pasynUser->errorMessage, pasynUser->errorMessageSize,
			"PV not linked to any register");
			return asynDisabled;
	}
	return cmd->io(dir, value, type, nelm);
}



/** This function will be called from asynInt32 port driver interface
  * implementation functions to check if a given asynUser matches incoming data,
  * if it does the matching data will be returned on data parameter.
  *
  * @param pasynUser[in]: the user to check
  * @param rxd[in]: incoming data to be casted as rfcUsbDeviceTrasfer.
  * @param data[out]: output data (if any)
  * @return : 1 on success otherwise 0
  */


//extern "C" int rfc_resource_match_int32(asynUser *pasynUser,  const void *rxd, uint32_t *data)
//{
//    const rfcUsbPacket *tr = static_cast<const rfcUsbPacket *>(rxd);
//	ioCmd *cmd = getIoCmd(pasynUser);
//	if (cmd->matches(*tr)) {
//		*data = be32toh(tr->get_data_buffer()[cmd->getAddress() -
//				tr->get_start_address()]);
//		return 1;
//	}
//	return 0;
//}
//
//extern "C" int rfc_resource_match_uint32(asynUser *pasynUser,  const void *rxd,
//					 epicsUInt32 *data)
//{
//	const rfcUsbPacket *tr =
//				static_cast<const rfcUsbPacket *>(rxd);
//	ioCmd *cmd = getIoCmd(pasynUser);
//	if (cmd->matches(*tr)) {
//		*data = be32toh(tr->get_data_buffer()[cmd->getAddress() -
//				tr->get_start_address()]);
//		return 1;
//	}
//	return 0;
//}

/** This function will be called from asynInt32Array port driver interface
  * implementation functions to check if a given asynUser matches incoming data,
  * if it does the matching data will be returned on data and dlen parameters.
  *
  * @param pasynUser[in]: the user to check
  * @param rxd[in]: incoming data to be casted as rfcUsbDeviceTransfer.
  * @param data[out]: output data (if any)
  * @param data[out]: output data len (if any)
  * @return : 1 on success otherwise 0
  */

//extern "C"
//int rfc_resource_match_int32array(asynUser *pasynUser, const void *xd,
//									epicsInt32 **data, size_t *dlen)
//{
//    const rfcUsbPacket *tr = static_cast<const rfcUsbPacket *>(xd);
//	ioCmd *cmd = getIoCmd(pasynUser);
//	if (cmd->matches(*tr)) {
//		*data = (epicsInt32 *)tr->get_data_buffer();
//		*dlen = tr->get_data_size();
//		return 1;
//	}
//	return 0;
//}


/** constants used by asyn portDriver interfaces implementations to specify
 * the data type.
 */

const char *ioc_int32 = "ioc_int32";
const char *ioc_float64 = "ioc_float64";

