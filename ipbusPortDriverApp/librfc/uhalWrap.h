#ifndef UHALWRAP_H_
#define UHALWRAP_H_

#ifdef __cplusplus

#include <uhal/uhal.hpp>
//#include <uhal/tests/tools.hpp>
#include <boost/filesystem.hpp>

using namespace uhal;


class conManWr {
	ConnectionManager* _con;
public:
	conManWr(char *con);
	ConnectionManager* getConn() {return _con;};
};

class hwIntWr : public HwInterface{
public :
	hwIntWr(const char * c, const char * ch) : HwInterface((new ConnectionManager(c))->getDevice(ch)) {};
};

//class hwIntWr : public HwInterface{
//public :
//	hwIntWr(ConnectionManager * c, const char * ch) : HwInterface(c->getDevice(ch)) {};
//};
extern "C" {

#endif

typedef struct conManWr conManWr;
conManWr *newconManWr(void *c);

typedef struct hwIntWr hwIntWr;
hwIntWr *newhwIntWr(void *c, void * ch);

#ifdef __cplusplus

};

#endif


#endif /* UHALWRAP_H_ */

