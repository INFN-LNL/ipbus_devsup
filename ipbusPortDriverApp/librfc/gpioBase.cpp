/*
 * gpioBase.cpp
 *
 *  Created on: Jun 24, 2015
 *      Author: damiano
 */

#include "gpioBase.h"
#include <assert.h>


gpioLine * gpioBase::newGpioLine(unsigned line, int dir)
{
	return new gpioLine(this, line, dir);
}

gpioField * gpioBase::newGpioField(unsigned width, unsigned offset, int dir)
{
	return new gpioField(this, width, offset, dir);
}


gpioField::gpioField(gpioBase *io, unsigned width, unsigned offset, int dir) :
		gpio(io), wth(width), off(offset), _dir(dir)
{
}

uint32_t gpioField::get()
{
	return gpio->get_field(wth, off);
}

uint32_t gpioField::set(uint32_t val)
{
	return gpio->set_field(wth, off, val);
}

int gpioField::setDir(int dir)
{
	return gpio->set_field_dir(wth, off, dir);
}
