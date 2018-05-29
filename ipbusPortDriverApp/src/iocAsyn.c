#include <asynDriver.h>
#include <cantProceed.h>
#include <epicsStdio.h>
#include <epicsExport.h>
#include <asynUInt32Digital.h>
#include <asynInt32.h>
#include <asynInt32Array.h>
#include <asynFloat64.h>
#include <asynDrvUser.h>
#include <errlog.h>
#include <string.h>
#include "baccoObj.h"
#include <uhalWrap.h>
#include <epicsAssert.h>
#include <endian.h>

#define MAX_PORTS 40


struct baccoPort {
	char *name;
	void *bacco;
	asynInterface bacco_common_itf;
	asynInterface bacco_UInt32digital_itf;
	asynInterface bacco_int32_itf;
	asynInterface bacco_drvUser_itf;
	asynInterface float64_itf;
	asynInterface int32Array_itf;
	/** Asyn private data to support Interrupts on asynInt32 interface */
	void *pasynInt32Pvt;
	/** Asyn private data to support Interrupts on asynInt32Array interface */
	void *pasynInt32ArrayPvt;
	/** Asyn private data to support Interrupts on asynUInt32Digital int. */
	void *pasynUInt32DigitalPvt;
};

static struct baccoPort _ports[MAX_PORTS];
static int nports = 0;

/******************************************************************************/

int portRegister(struct baccoPort *p, const char *name)
{
	p->name = (char *)mallocMustSucceed(strlen(name) + 1, "");
	strcpy(p->name, name);

	//create a new port
	if (pasynManager->registerPort(name, ASYN_CANBLOCK, 1, 0, 0) != asynSuccess)
		return 1;
	//register interfaces to the newly created port
	pasynUInt32DigitalBase->initialize(name, &p->bacco_UInt32digital_itf);
	pasynInt32Base->initialize(name, &p->bacco_int32_itf);
	pasynInt32ArrayBase->initialize(name, &p->int32Array_itf);
	pasynFloat64Base->initialize(name, &p->float64_itf);
	pasynManager->registerInterface(name, &p->bacco_drvUser_itf);
	pasynManager->registerInterface(name, &p->bacco_common_itf);
	pasynManager->registerInterruptSource(name, &p->bacco_int32_itf,
											&p->pasynInt32Pvt);
	pasynManager->registerInterruptSource(name, &p->int32Array_itf,
											&p->pasynInt32ArrayPvt);
	pasynManager->registerInterruptSource(name, &p->bacco_UInt32digital_itf,
						&p->pasynUInt32DigitalPvt);
	return 0;
}


int baccoPortRegister(struct baccoPort *p, const char *name, const char *file_p, const char *label)
{
	char *file = "file://";
	char *file_path = (char *)mallocMustSucceed(strlen(file_p) + strlen(file) + 1, "");
	strcpy(file_path, file);
	strcat(file_path, file_p);
	if (!(p->bacco = get_ioc_ipbus(file_path, label)))
		return 1;
	return portRegister(p, name);
}

void bacco_create_port(const char *name, const char *file_p, const char *label)
{
	if (nports == MAX_PORTS || baccoPortRegister(&_ports[nports], name, file_p, label)) {
		errlogPrintf("bacco: Cannot more than %d ports\n", MAX_PORTS);
		return;
	}
	nports++;	
}

//void usb_create_port(const char *name, const char *sno)
//{
//	if (nports == MAX_PORTS || usbPortRegister(&_ports[nports], name, sno)) {
//		errlogPrintf("rfc: Cannot more than %d ports\n", MAX_PORTS);
//		return;
//	}
//	nports++;
//}

/************************* COMMON INTERFACE ***********************************/

static void asyn_common_report(void *drvPvt, FILE *fp, int details)
{
	struct baccoPort *port = (struct baccoPort*)drvPvt;
	fprintf(fp, "Reporting port '%s'\n", port->name); 
}

static asynStatus asyn_common_connect(void *drvPvt, asynUser *pasynUser)
{
	struct baccoPort *port = (struct baccoPort*)drvPvt;
	errlogSevPrintf(errlogInfo, "Connecting port '%s' (%p)...",
					port->name, pasynUser);

	if (!bacco_connect(port->bacco)) {
		pasynManager->exceptionConnect(pasynUser);	
		errlogSevPrintf(errlogInfo, "connected!\n");
		return asynSuccess;
	}
	errlogSevPrintf(errlogInfo, "NOT connected!\n");
	if (pasynUser)
		strncpy(pasynUser->errorMessage, "Cannot connect", pasynUser->errorMessageSize);
	return asynError;

}

static asynStatus asyn_common_disconnect(void *drvPvt, asynUser *pasynUser)
{
	struct baccoPort *port = (struct baccoPort*)drvPvt;
//	errlogSevPrintf(errlogInfo, "Disconnecting port '%s'\n", port->name);
//	rfc_disconnect(port->bacco);
//	pasynManager->exceptionDisconnect(pasynUser);
	return asynSuccess;
}

static asynCommon bacco_common_itf = {
	.report = asyn_common_report,
	.connect = asyn_common_connect,
	.disconnect = asyn_common_disconnect
};


/************************* UINT32DIGITAL INTERFACE ****************************/

static asynStatus asynUInt32Digital_write(void *drvPvt, asynUser *pasynUser,
         epicsUInt32 value, epicsUInt32 mask)
{
	epicsUInt32 regvalue;
	asynStatus err;
	struct baccoPort *port = (struct baccoPort*)drvPvt;
	if ((err = bacco_io(port->bacco, pasynUser, ioc_int32, 1, &regvalue, 1))
		!= asynSuccess) {
		return err;
	}
	if (mask & value) {
		regvalue |= mask;
	} else {
		regvalue &= ~mask;
	}
	return bacco_io(port->bacco, pasynUser, ioc_int32, 0, &regvalue, 1);
}

static asynStatus asynUInt32Digital_read(void *drvPvt, asynUser *pasynUser,
        epicsUInt32 *value, epicsUInt32 mask) 
{
	struct baccoPort *port = (struct baccoPort*)drvPvt;
	return bacco_io(port->bacco, pasynUser, ioc_int32, 1, (epicsInt32 *)&value, 1);
}

static asynUInt32Digital bacco_UInt32digital_itf = {
	.write = asynUInt32Digital_write,
	.read = asynUInt32Digital_read
};


/** See int32InterruptFire documentation: same concept
 */

void uint32DigitalInterruptFire(struct baccoPort *port, const void *rxd)
{
	ELLLIST *pclientList;
	interruptNode *pnode;
	asynInt32Interrupt *pinterrupt;
	epicsUInt32 data;

	//TODO
	return;

//	pasynManager->interruptStart(port->pasynInt32Pvt, &pclientList);
//	pnode = (interruptNode *)ellFirst(pclientList);
//	while (pnode) {
//		pinterrupt = pnode->drvPvt;
//		if (rfc_resource_match_uint32(pinterrupt->pasynUser, rxd, &data))
//			pinterrupt->callback(pinterrupt->userPvt,
//						pinterrupt->pasynUser, data);
//		pnode = (interruptNode *)ellNext(&pnode->node);
//	}
//	pasynManager->interruptEnd(port->pasynInt32Pvt);
}

/************************* INT32 INTERFACE ************************************/

static asynStatus asynInt32_write(void *drvPvt, asynUser *pasynUser, 
									epicsInt32 value)
{
	struct baccoPort *port = (struct baccoPort*)drvPvt;
	return bacco_io(port->bacco, pasynUser, ioc_int32, 0,  &value, 1);
}

static asynStatus asynInt32_read(void *drvPvt, asynUser *pasynUser, 
									epicsInt32 *value) 
{
	struct baccoPort *port = (struct baccoPort*)drvPvt;
	return bacco_io(port->bacco, pasynUser, ioc_int32, 1, value, 1);
}

/*
static asynStatus asynInt32_getBounds(void *drvPvt, asynUser *pasynUser,
                           epicsInt32 *low, epicsInt32 *high)
{
	*low=0;
	*high=255;	
	errlogPrintf("GetBounds\n");

	return asynSuccess;
}
*/

static asynInt32 bacco_int32_itf = {
	.read = asynInt32_read,
	.write = asynInt32_write,
//	.getBounds = asynInt32_getBounds
};

/** Fires an interrupt whenever int32 data is available from lower level layers.
  * Lower level layers doesn't know about asyn clients waiting for interrupts,
  * and this level doesn't know anything about lower level data management
  * (USB, IPBus ecc...).
  * This function is supposed to be called from lower level SW layer to inform
  * the port driver about new int32 data, it retrieve the users waiting for data
  * and ask to low level layer if the user data match the incoming one.
  *
  * @param prt : low level port object (opaque for this module)
  * @param rxd : low level data object (opaque for this layer)
  */

static void int32InterruptFire(struct baccoPort *port, const void *rxd)
{
	ELLLIST *pclientList;
	interruptNode *pnode;
	asynInt32Interrupt *pinterrupt;
	//uint32_t data;

	//TODO
	return;

//	pasynManager->interruptStart(port->pasynInt32Pvt, &pclientList);
//    pnode = (interruptNode *)ellFirst(pclientList);
//    while (pnode) {
//        pinterrupt = pnode->drvPvt;
//		if (rfc_resource_match_int32(pinterrupt->pasynUser, rxd, &data))
//			pinterrupt->callback(pinterrupt->userPvt, pinterrupt->pasynUser,
//							data);
//        pnode = (interruptNode *)ellNext(&pnode->node);
//    }
//    pasynManager->interruptEnd(port->pasynInt32Pvt);
}

/************************* INT32ARRAY INTERFACE *******************************/

static void int32ArrayInterruptFire(struct baccoPort *port, const void *rxd);
static asynStatus int32ArrayRead(void *drvPvt, asynUser *pasynUser,
                       epicsInt32 *value, size_t nelements, size_t *nIn);

static asynInt32Array bacco_int32Array_itf = {
	.read = int32ArrayRead
};

static void int32ArrayInterruptFire(struct baccoPort *port, const void *rxd)
{
	ELLLIST *pclientList;
	interruptNode *pnode;
	asynInt32ArrayInterrupt *pinterrupt;
	epicsInt32 *data;
	size_t dlen;

	//TODO
	return;

//	pasynManager->interruptStart(port->pasynInt32ArrayPvt, &pclientList);
//    pnode = (interruptNode *)ellFirst(pclientList);
//    while (pnode) {
//        pinterrupt = pnode->drvPvt;
//		if (rfc_resource_match_int32array(pinterrupt->pasynUser, rxd, &data, &dlen)) {
//			pinterrupt->callback(pinterrupt->userPvt, pinterrupt->pasynUser,
//							data, dlen);
//		}
//        pnode = (interruptNode *)ellNext(&pnode->node);
//    }
//    pasynManager->interruptEnd(port->pasynInt32ArrayPvt);
}

static asynStatus int32ArrayRead(void *drvPvt, asynUser *pasynUser,
                       epicsInt32 *value, size_t nelements, size_t *nIn)
{
	asynStatus err;
	struct baccoPort *port = (struct baccoPort*)drvPvt;
	if ((err = bacco_io(port->bacco, pasynUser, ioc_int32, 1, value, nelements)))
			return err;
	*nIn = nelements;
	return asynSuccess;
}

/************************* FLOAT64 INTERFACE **********************************/


static asynStatus asynFloat64_write(void *drvPvt, asynUser *pasynUser, 
									epicsFloat64 value)
{
	struct baccoPort *port = (struct baccoPort*)drvPvt;
	return bacco_io(port->bacco, pasynUser, ioc_float64, 0, &value, 1);
}

static asynStatus asynFloat64_read(void *drvPvt, asynUser *pasynUser, 
									epicsFloat64 *value)
{
	struct baccoPort *port = (struct baccoPort*)drvPvt;
	return bacco_io(port->bacco, pasynUser, ioc_float64, 1, value, 1);
}

static asynFloat64 bacco_float64_itf = {
	.write = asynFloat64_write,
	.read = asynFloat64_read
};

/************************* DRVUSER INTERFACE **********************************/

    /*The following do not have to be called via queueRequest callback*/
static asynStatus asynDrvUser_create(void *drvPvt,asynUser *pasynUser,
        const char *drvInfo, const char **pptypeName,size_t *psize)
{
	struct baccoPort *port = (struct baccoPort *)drvPvt;
	//errlogPrintf("Creating drvUser %p %p '%s'\n", drvPvt, pasynUser, drvInfo);
	//return asynSuccess;
	return ioc_get_parameter_link(port->bacco, drvInfo, pasynUser);
}

static asynStatus asynDrvUser_getType(void *drvPvt, asynUser *pasynUser,
        const char **pptypeName,size_t *psize)
{
	//errlogPrintf("GetType\n");
	return asynSuccess;
}

static asynStatus asynDrvUser_destroy(void *drvPvt, asynUser *pasynUser)
{
	//errlogPrintf("Destroy\n");
	return asynSuccess;
}

static asynDrvUser bacco_drvUser_itf = {
	.create = asynDrvUser_create,
	.getType = asynDrvUser_getType,
	.destroy = asynDrvUser_destroy
};


/******************************************************************************/



/**
 * Fires and interrupt to users in interrupt mode on all interfaces
 * supporting interrupts
 *
 */

void interruptFire(void *prt, const void *rxd)
{
	struct baccoPort *port = (struct baccoPort*)prt;
	if (!port || !rxd) return;
	int32InterruptFire(port, rxd);
	int32ArrayInterruptFire(port, rxd);
	uint32DigitalInterruptFire(port, rxd);
}



/******************************************************************************/

void baccoPortInit(struct baccoPort *p)
{
	p->bacco_common_itf.interfaceType = asynCommonType;
	p->bacco_common_itf.pinterface = &bacco_common_itf;
	p->bacco_common_itf.drvPvt = p;

	p->bacco_UInt32digital_itf.interfaceType = asynUInt32DigitalType;
	p->bacco_UInt32digital_itf.pinterface = &bacco_UInt32digital_itf;
	p->bacco_UInt32digital_itf.drvPvt = p;

	p->bacco_int32_itf.interfaceType = asynInt32Type;
	p->bacco_int32_itf.pinterface = &bacco_int32_itf;
	p->bacco_int32_itf.drvPvt = p;

	p->float64_itf.interfaceType = asynFloat64Type;
	p->float64_itf.pinterface = &bacco_float64_itf;
	p->float64_itf.drvPvt = p;

	p->bacco_drvUser_itf.interfaceType = asynDrvUserType;
	p->bacco_drvUser_itf.pinterface = &bacco_drvUser_itf;
	p->bacco_drvUser_itf.drvPvt = p;

	p->int32Array_itf.interfaceType = asynInt32ArrayType;
	p->int32Array_itf.pinterface = &bacco_int32Array_itf;
	p->int32Array_itf.drvPvt = p;
}


static void baccoAsynRegister(void) {
	int i;
	nports = 0;
	for (i = 0; i < MAX_PORTS; i++) {
		baccoPortInit(&_ports[i]);
	}	
}


epicsExportRegistrar(baccoAsynRegister);


