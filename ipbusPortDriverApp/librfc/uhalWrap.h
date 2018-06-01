#ifndef UHALWRAP_H_
#define UHALWRAP_H_

#ifdef __cplusplus

#include <uhal/uhal.hpp>
#include <uhal/log/log.hpp>
//#include "uhal/tests/tools.hpp"
#include "uhal/ProtocolUDP.hpp"

#include <boost/filesystem.hpp>
//#include <boost/test/unit_test.hpp>

using namespace uhal;


class hwIntWr : public HwInterface{
public :
	hwIntWr(const char * c, const char * ch) : HwInterface((new ConnectionManager(c))->getDevice(ch)) {uhal::disableLogging();};
};


extern "C" {

#endif
typedef struct hwIntWr hwIntWr;
hwIntWr *newhwIntWr(void *c, void * ch);

#ifdef __cplusplus

};

#endif


#endif /* UHALWRAP_H_ */

