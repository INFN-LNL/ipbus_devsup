
#include <epicsExport.h>
#include <iocsh.h>
#include <epicsStdio.h>
#include "iocAsyn.h"

static void baccoCreateIPBusPortCallFunc(const iocshArgBuf *args) {
	bacco_create_port(args[0].sval, args[1].sval, args[2].sval);
}

//static void createUsbPortCallFunc(const iocshArgBuf *args) {
//	usb_create_port(args[0].sval, args[1].sval);
//}
//
//static void rfcListUsbDevicesCallFunc(const iocshArgBuf *args) {
//	usbutils_devdesc *devs;
//	int ndevs, i;
//	usbutil_list_devices(&devs, &ndevs);
//	epicsStdoutPrintf("Found %d RFC devices\n", ndevs);
//	for(i = 0; i < ndevs; i++)
//		epicsStdoutPrintf("'%s' '%s'\n", devs[i].sno, devs[i].prod);
//}


static const iocshArg     name = {"name", iocshArgString};
static const iocshArg     file_path = {"file path", iocshArgString};
static const iocshArg     ioc_name = {"ioc_name", iocshArgString};


static const iocshArg    *createPortArgs[] =
{
	&name, 
	&file_path,
	&ioc_name
};


static const iocshFuncDef baccoCreateIPBusPortFuncDef =
{
	"baccoCreateIPBusPort", 3, createPortArgs
};

//static const iocshFuncDef createUsbPortFuncDef =
//{
//	"createUsbPort", 2, createPortArgs
//};
//
//static const iocshFuncDef rfcListUsbDevicesFuncDef =
//{
//	"rfcListUsbDevices", 0, 0
//};

static void baccoIocRegister(void)
{
    iocshRegister(&baccoCreateIPBusPortFuncDef, baccoCreateIPBusPortCallFunc);
    //iocshRegister(&createUsbPortFuncDef, createUsbPortCallFunc);
    //iocshRegister(&rfcListUsbDevicesFuncDef, rfcListUsbDevicesCallFunc);
}



epicsExportRegistrar(baccoIocRegister);

