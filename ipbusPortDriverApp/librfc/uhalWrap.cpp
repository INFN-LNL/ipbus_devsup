#include "uhalWrap.h"

conManWr::conManWr(char *con) {
	_con = new ConnectionManager(con);
}


//int hwIntWr::connect(){
//	registersIPBUS r(*this, "d");
//	uint32_t n = r.get(0);
//	if (n == CTRL_WORLD)
//	    return 0;
//	else return 1;
//}

extern "C" {

conManWr * newconManWr(void *c){
	return new conManWr((char*)c);
}

hwIntWr *newhwIntWr(void *c, void *ch){
	return new hwIntWr((const char *) c, (const char *) ch);
}

//hwIntWr *newhwIntWr(void *c, void *ch){
//	return new hwIntWr((ConnectionManager *) c, (const char *) ch);
//}

//ConnectionManager* getConn(myUhal * m){
//	return m->getConn();
//}

};
