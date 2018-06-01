
#ifndef _BACCO_OBJ_H_
#define _BACCO_OBJ_H_


#include <asynDriver.h>
#include <epicsTypes.h>

#if defined(__cplusplus)
extern "C" {
#endif




extern const char *ioc_int32;
extern const char *ioc_float64;


//void * get_rfc(void *p, const char *serial);
void * get_ioc_ipbus(void *p, const char *serial);
//void * get_usb(void *p, const char *serial);
//int rfc_connect(void *r);
int bacco_connect(void *r);
//void rfc_disconnect(void *r);
int bacco_disconnect(void *r);
asynStatus bacco_io(void *obj, asynUser *pasynUser, const char *type, int dir,
						void *value, size_t nelm);


asynStatus ioc_get_parameter_link(void *io, const char *parameters, asynUser *pasynUser);
//int rfc_resource_match_int32(asynUser *pasynUser,  const void *txd, uint32_t *data);
//int rfc_resource_match_uint32(asynUser *pasynUser,  const void *txd, epicsUInt32 *data);
//int rfc_resource_match_int32array(asynUser *pasynUser,  const void *xd, epicsInt32 **data, size_t *dlen);




#if defined(__cplusplus)
}
#endif

#endif

