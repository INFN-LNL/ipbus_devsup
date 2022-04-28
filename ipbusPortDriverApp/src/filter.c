/*
 * filter.c
 *
 *  Created on: May 4, 2016
 *      Author: davide.marcato@lnl.infn.it
 */

#include <stdio.h>
#include <dbDefs.h>
#include <subRecord.h>
#include <dbCommon.h>
#include <recSup.h>
#include <registryFunction.h>
#include <epicsExport.h>
#include <epicsTypes.h>
#include <cantProceed.h>
#include <string.h>

struct cbuff{
	epicsFloat64 *buf;
	size_t bufSize;
	size_t inx;
};

long filterInit(struct subRecord *psub) {
	struct cbuff *s = (struct cbuff *) psub->dpvt;

	if (!s){
		s = (struct cbuff *) callocMustSucceed(1, sizeof(struct cbuff), "");
		bzero(s, sizeof(*s));
		psub->dpvt = s;
	}
	if ( (s->buf == 0) || (psub->b > s->bufSize) ) {
		free(s->buf);
		s->buf = (epicsFloat64 *) callocMustSucceed ( (size_t) psub->b, sizeof(epicsFloat64), "Calloc failed");
		s->bufSize = (size_t) psub->b;
		s->inx=0;
	}
    return(0);
}

long filterProc(struct subRecord *psub)
{
	filterInit(psub);

	struct cbuff *s = (struct cbuff *) psub->dpvt;

	s->buf[s->inx++] = psub->a;
	s->inx %= s->bufSize;

	size_t k=0;
	psub->val = 0;
	for (k=0; k<s->bufSize; k++) {
		psub->val += s->buf[k];
	}
	psub->val /= s->bufSize;

	return(0);
}

epicsRegisterFunction(filterInit);
epicsRegisterFunction(filterProc);


