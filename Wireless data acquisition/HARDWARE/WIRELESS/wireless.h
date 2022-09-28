#ifndef __WIRELESS_H
#define __WIRELESS_H

#include "sys.h"

void wireless_init();
void wirelessSend(char arrayList[], int length);
void wirelessSend_u8(u8 arrayList[], int length);

#endif