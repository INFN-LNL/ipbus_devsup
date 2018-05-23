/*
 * gpioBase.h
 *
 *  Created on: Jun 24, 2015
 *      Author: damiano
 */

#ifndef GPIOBASE_H_
#define GPIOBASE_H_

#include <inttypes.h>
#include <memory>


class gpioField;
class gpioLine;


class gpioBase
{
public:
	virtual ~gpioBase() {};
	void set_line(unsigned line, bool value) { set_field(1, line, value); }
	bool get_line(unsigned line) { return get_field(1, line); }
	void set_line_dir(unsigned line, int dir) { set_field_dir(1, line, dir); }
	gpioLine * newGpioLine(unsigned line, int dir);
	gpioField * newGpioField(unsigned width, unsigned offset, int dir);
	virtual unsigned set_field(unsigned width, unsigned offset, unsigned value)=0;
	virtual unsigned get_field(unsigned width, unsigned offset)=0;
	virtual int set_field_dir(unsigned width, unsigned offset, int dir)=0;
};


class gpioField {
	gpioBase *gpio;
	unsigned wth;
	unsigned off;
	int _dir;
public:
	gpioField(gpioBase *io, unsigned width, unsigned offset, int dir=0);
	virtual ~gpioField() {};
	uint32_t get();
	uint32_t set(uint32_t);
	virtual int setDir(int dir);
	virtual void reset() {
		setDir(_dir);
	}
};

class gpioLine : public gpioField {
public:
	gpioLine(gpioBase *io, unsigned offset, int dir=0) :
		gpioField(io, 1, offset, dir)
	{}
	uint32_t get() { return gpioField::get() ? 1 : 0; }
	uint32_t set(uint32_t v) { return gpioField::set(v ? 1 : 0); }
};

#endif /* GPIOBASE_H_ */
