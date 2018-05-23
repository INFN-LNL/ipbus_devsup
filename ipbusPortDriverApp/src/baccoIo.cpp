#include <cantProceed.h>
#include <stdio.h>
#include "baccoIo.h"
#include "ioCmd.h"


ipbusIo::ipbusIo(void *p) :
        hw((hwIntWr *)p),
		bacc_stat_reg(hw, STAT_REG) {}

int ipbusIo::connect()
{
	int ret;
	if (bacc_stat_reg.get(0) == MAGIC_WORLD)
		ret = 0;
	else
        ret = 1;
	return ret;
}

baccoIo::baccoIo(void *p, const char *sl1, const char *sl2) :
		ipbusIo(p),
		slave_1(hw, sl1),
		slave_2(hw, sl2)
{
	bacco_obj = new baccoObject();
}

int baccoIo::connect()
{
	return ipbusIo::connect();
}

//int usbIo::disconnect()
//{
//	if (!asyn_port || _dummy)
//		return 0;
//	rfc_usb.hw_disconnect();
//	if (usb_handle)	{
//		libusb_close(usb_handle);
//		usb_handle = 0;
//		libusb_unref_device(usb_device);
//		usb_device = 0;
//	}
//	return 0;
//}
//
ioCmd * baccoIo::factoryIoCmd(const char *str, asynUser *pasynUser)
{
	ioCmd *cmd = 0;
	char *params=0, *ftype=0;
	if (sscanf(str, "%m[^( ](%m[^)])", &ftype, &params) == 2) {
		cmd = ioCmdFactory(ftype, params, this, pasynUser);
		free(params);
		free(ftype);
		if (cmd && cmd->isValid())
			pasynUser->drvUser = static_cast<void *>(cmd);
	}
	return cmd;
}
//
//extern "C" void interruptFire(void *prt, const void *rxd);
//
//void usbIo::process_rx_packet(const rfcUsbPacket &rx, void *arg)
//{
//	usbIo *io = static_cast<usbIo*>(arg);
//	interruptFire(io->asyn_port, &rx);
//}


void baccoIo::write(unsigned inx, uint32_t address, uint32_t value){
	switch (inx)
	{
	    case 1:
	    	slave_1.set(address, value);
	        break;
	    case 2:
	    	slave_2.set(address, value);
	        break;
	}
}
uint32_t baccoIo:: read(unsigned inx, uint32_t address){
	switch (inx)
	{
	    case 1:
	    	return slave_1.get(address);
	        break;
	    case 2:
	    	return slave_2.get(address);
	        break;
	}
}
void set(uint32_t address, uint32_t value);
