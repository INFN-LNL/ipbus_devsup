#ifndef IOCSUPPORT_RFCUSBPORTDRIVERAPP_SRC_BACCOIO_H_
#define IOCSUPPORT_RFCUSBPORTDRIVERAPP_SRC_BACCOIO_H_

#define STAT_REG  "stat_reg"
#define MAGIC_WORD 0xabcdfedc

#include <asynDriver.h>
#include <uhalWrap.h>
#include <registerIPBUS.h>
#include <baccoObject.h>


class ioCmd;

/** C++ class representing the asynPort.
 * It will be userd by the asyn's portDriver as a opaque pointer.
 * It can be considered a C++ extension of the rfcPort structure defined in the
 * rfcAsyn.c module. By design we don't want to know details on rfcPort so we keep
 * it as a void * link.
 */

//class usbIo {
//	static void process_rx_packet(const rfcUsbPacket &rx, void *arg);
//protected:
//public:
//	/** the serail number of the USB board */
//	char *serial;
//	/** libusb device instance */
//	libusb_device *usb_device;
//	/** libusb device handle */
//	libusb_device_handle *usb_handle;
//	/** rfcUsbDevice class implementing the SW <-> HW comunication protocol */
//	rfcUsbDevice rfc_usb;
//	/** link to the asyn rfcPort instance */
//	void *asyn_port;
//	bool _dummy;
//
//	usbIo(void *p, const char *sno);
//	virtual ~usbIo(){};
//	//virtual int connect();
//	int disconnect();
//	ioCmd *factoryIoCmd(const char *str, asynUser *pasynUser);
//};

class ipbusIo {
public:
	hwIntWr * hw;
	/** register to control the actual connection */
	registersIPBUS bacc_stat_reg;

	ipbusIo(void *p);
	virtual ~ipbusIo(){};
	virtual int connect();
	virtual int disconnect();
	ioCmd *factoryIoCmd(const char *str, asynUser *pasynUser);

};

class baccoIo : public ipbusIo {
public:

	/** register that represent the FPGA memory port */
	registersIPBUS slave_1;
	registersIPBUS slave_2;

	/** high level bacco board representation */
	baccoObject *bacco_obj;

	baccoIo(void *p, const char *sl1 = "sl1", const char *sl2 = "sl2");
	virtual ~baccoIo(){};
	virtual int connect();
	virtual int disconnect();
	ioCmd *factoryIoCmd(const char *str, asynUser *pasynUser);

	void write(unsigned inx, uint32_t address, uint32_t value);
	uint32_t read(unsigned inx, uint32_t address);
	uint32_t read(unsigned inx, uint32_t address, uint32_t *data, uint32_t dsize);

};





#endif /* IOCSUPPORT_RFCUSBPORTDRIVERAPP_SRC_BACCOIO_H_ */
