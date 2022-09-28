#ifndef __DATAPROCESS_H
#define	__DATAPROCESS_H
#include "sys.h"
#include "exfuns.h"

#define head300 'A'
#define head301 'B'
#define head302 'C'
#define head303 'D'
#define head304 'E'
#define head305 'F'
#define head306 'G'
#define head307 'H'
#define head308 'I'
#define head309 'J'
#define head310 'K'
#define head311 'L'
#define head320 'M'
#define head321 'N'
#define head328 'O'
#define head322 'P'
#define head323 'Q'
#define head324 'R'
#define head325 'S'
#define head340 'T'
#define head350 'U'
#define head360 'Y'
#define head401 'V'
#define head412 'W'
#define head329 '@'
#define head32A '#'
#define head32B '$'
#define head32C '%'

#define len300 4
#define len301 4
#define len302 7
#define len303 5
#define len304 7
#define len305 8
#define len306 8
#define len307 8
#define len308 8
#define len309 5
#define len310 6
#define len311 8
#define len320 8
#define len321 5
//#define len328 1
#define len322 5
#define len323 6
#define len324 6
#define len325 4
#define len340 8
#define len350 4
#define len360 8
#define len401 5
#define len412 8
#define len329 8
#define len32A 8
#define len32B 8
#define len32C 8
void u8dec_TwoHex(u8 dec, char* outhigh, char* outlow);

void dataProcess(unsigned char data[],int len,char head);

void sdwrite(char time[],char flag[],unsigned char canmessage[],int length,FIL* file,UINT bw);
void three2onestr(char *result,char str1[],int str1len,char str2,char str3[],int str3len);

#endif