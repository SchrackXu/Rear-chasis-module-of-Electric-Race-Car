#ifndef __TEST_PART_H
#define	__TEST_PART_H
#include "sys.h"

void test_led();
void test_wireless();
void test_canloopback();
void test_cannormal();
void test_rtc();
void test_sd();
void test_SDandRTC();

 void time2txtname(char time[],char txtname[]);
 void time2dirname(char time[],char dirname[]);

#endif