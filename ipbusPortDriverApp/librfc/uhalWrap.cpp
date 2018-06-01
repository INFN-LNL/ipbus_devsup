#include "uhalWrap.h"




extern "C" {


hwIntWr *newhwIntWr(void *c, void *ch){
	return new hwIntWr((const char *) c, (const char *) ch);
}


};
