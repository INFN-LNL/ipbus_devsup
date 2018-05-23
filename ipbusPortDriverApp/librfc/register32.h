/*
 * register32.h
 *
 *  Created on: Jun 24, 2015
 *      Author: damiano
 */

#ifndef REGISTER32_H_
#define REGISTER32_H_

#include "gpioBase.h"


class registers32;

class register32 : public gpioBase {
    registers32 *_regs;
    uint32_t _add;
public:
	virtual ~register32() {}
    register32(registers32 *regs, uint32_t add): _regs(regs), _add(add) {}
    gpioField * newField(unsigned wi, unsigned sh, int dir);
	virtual unsigned set_field(unsigned width, unsigned offset, unsigned value);
	virtual unsigned get_field(unsigned width, unsigned offset);
	virtual int set_field_dir(unsigned width, unsigned offset, int dir);
	uint32_t get() const;
	void set(uint32_t val);
};

class registers32 {
	registers32 *_parent;
	size_t		_offset;
public:
	registers32(registers32 *parent=0, size_t offset=0);
	virtual ~registers32() {};
	register32 * newRegister(uint32_t add);
	virtual void set(uint32_t address, uint32_t value);
	virtual uint32_t get(uint32_t address);
	register32 operator[](const int &inx);
};

class registers32Dummy : public registers32 {
public:
	virtual ~registers32Dummy() {};
	virtual void set(uint32_t address, uint32_t value);
	virtual uint32_t get(uint32_t address);
};

#endif /* REGISTER32_H_ */
