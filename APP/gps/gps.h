#ifndef _gps_H
#define _gps_H

#include "system.h"
#include "usart.h"
extern u8 gpssendflag;
extern	u8	gpsposition[8];
extern	char *gpsdelim;
extern	char gpsdest[10];
extern		char gpsdest1[10];
extern	char*gpsp;
extern		char*gpsp1;
extern		char * gpsdest_string;
extern	u16 lathalf1;
extern	u16 lathalf2;
extern	u16 loghalf1;
extern	u16 loghalf2;
void parseGpsBuffer(void);
void printGpsBuffer(void);
		void gpsinit(void);
		u16 str2int(const char *str);
void ublox_gpsinit(void);

void errorLog(int num);
#endif
