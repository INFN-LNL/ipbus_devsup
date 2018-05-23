/*
 * register32RFC.h
 *
 *  Created on: Jun 26, 2015
 *      Author: damiano
 */

#ifndef REGISTERIPBUS_H_
#define REGISTERIPBUS_H_

#ifdef __cplusplus

#include <map>
#include "uhalWrap.h"
#include "register32.h"

using namespace uhal;

class registersIPBUS: public registers32  {
	HwInterface *_hw;
	const char  *_reg_id;
	uint32_t    _base_address;
	uint8_t _d; //if 0 you dont't do the dispatch else you do it. Default = 1
	typedef std::map<uint32_t, const char *>  mT;
	mT _m;   // key = address  -  value = node/reg

	const char * fromAddrtoNode(uint32_t id);
	const char * isAddressInMap(uint32_t id);
	const char * instertInMap(uint32_t id);
public:
	registersIPBUS(hwIntWr *hw, const char *reg_id, uint8_t d = 1);

	void set(uint32_t address, uint32_t value);
	uint32_t get(uint32_t address);
};

extern "C" {

#endif

typedef struct registersIPBUS registersIPBUS;

registersIPBUS *newregIP(void *h, void *r);

#ifdef __cplusplus

};

#endif

#endif /* REGISTER32RFC_H_ */
