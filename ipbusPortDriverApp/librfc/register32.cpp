/*
 * register32.cpp
 *
 *  Created on: Jun 24, 2015
 *      Author: damiano
 */

#include "register32.h"
#include <assert.h>
#include <stdio.h>

uint32_t register32::get() const
{
    return _regs->get(_add);
}

void register32::set(uint32_t val)
{
    _regs->set(_add, val);
}

register32 * registers32::newRegister(uint32_t add)
{
    return new register32(this, add);
}

gpioField * register32::newField(unsigned wi, unsigned sh, int dir)
{
	return new gpioField(this, wi, sh, dir);
}

unsigned register32::set_field(unsigned width, unsigned offset, unsigned value)
{
	assert(width+offset <= 32);
	uint32_t mask;
	if(width == 32)
		mask = 0xffffffff;
	else
		mask = ((1<<width) - 1) << offset;
	set((get() & ~mask) | ((value << offset) & mask));
	return 0;
}

unsigned register32::get_field(unsigned width, unsigned offset)
{
	assert(width+offset <= 32);
	uint32_t mask;
	if(width == 32)
		mask = 0xffffffff;
	else
		mask = ((1<<width) - 1) << offset;
	return (get() & mask) >> offset;
}

int register32::set_field_dir(unsigned width, unsigned offset, int dir)
{
	return 0; // nothing to do...
}

void registers32Dummy::set(uint32_t address, uint32_t value)
{
    printf("register32Dummy: writing to %08x value %08x\n", address, value);
}

uint32_t registers32Dummy::get(uint32_t address)
{
    printf("register32Dummy: reading from %08x\n", address);
    return 0;
}

registers32::registers32(registers32 *parent, size_t offset):
			_parent(parent), _offset(offset)
{}

void registers32::set(uint32_t address, uint32_t value)
{
	assert(_parent);
	_parent->set(address + _offset, value);
}

uint32_t registers32::get(uint32_t address)
{
	assert(_parent);
	return _parent->get(address + _offset);
}

register32 registers32::operator[](const int &inx)
{
	return register32(this, inx);
}
