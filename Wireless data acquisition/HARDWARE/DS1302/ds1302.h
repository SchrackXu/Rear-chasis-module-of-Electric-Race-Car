#ifndef _DS_1302_H
#define _DS_1302_H

#include "sys.h"
#include "delay.h"

//#define DAT_MODE_IN()  {GPIOB->CRL&=0XFFFFFF0F;GPIOB->CRL|=8<<4;}
//#define DAT_MODE_OUT() {GPIOB->CRL&=0XFFFFFF0F;GPIOB->CRL|=3<<4;}

#define RST_CLR  GPIO_ResetBits(GPIOB,GPIO_Pin_4)
#define RST_SET  GPIO_SetBits(GPIOB,GPIO_Pin_4)
#define DAT_CLR  GPIO_ResetBits(GPIOB,GPIO_Pin_1)
#define DAT_SET  GPIO_SetBits(GPIOB,GPIO_Pin_1)
#define DAT_READ GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_1)
#define SCK_CLR  GPIO_ResetBits(GPIOB,GPIO_Pin_3)
#define SCK_SET  GPIO_SetBits(GPIOB,GPIO_Pin_3)

#define DataToBcd(x) ((x/10)*16+(x%10))
#define BcdToData(x) ((x/16)*10+(x%16))

#define WRITE_SEC_REG	        0x80
#define WRITE_MIN_REG           0x82
#define WRITE_HOUR_REG          0x84
#define WRITE_DAY_REG           0x86
#define WRITE_MON_REG           0x88
#define WRITE_WEEK_REG          0x8a
#define WRITE_YEAR_REG          0x8c
#define READ_SEC_REG            0x81
#define READ_MIN_REG            0x83
#define READ_HOUR_REG           0x85
#define READ_DAY_REG            0x87
#define READ_MON_REG            0x89
#define READ_WEEK_REG           0x8b
#define READ_YEAR_REG           0x8d
#define WRITE_CONTROL_REG       0x8e
#define READ_CONTROL_REG        0x8f
#define WRITE_CHARGE_REG        0x90
#define READ_CHARGE_REG         0x91
#define WRITE_RAM_REG           0xc0
#define READ_RAM_REG            0xc1

typedef struct
{
	volatile uint8_t sec;
	volatile uint8_t min;
	volatile uint8_t hour;
	volatile uint8_t day;
	volatile uint8_t mon;
	volatile uint8_t week;
	volatile uint16_t year;
}DATE;

int DS1302_Init(const DATE * const date);
int DS1302_DateRead(DATE * const date);
int DS1302_DateSet(const DATE * const date);
int DS1302_time(char time[]);

extern DATE SysDate;

#endif

