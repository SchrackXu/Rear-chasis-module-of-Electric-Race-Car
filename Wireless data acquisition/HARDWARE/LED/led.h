#ifndef __LED_H
#define __LED_H	 
#include "sys.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEKս��STM32������
//LED��������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2012/9/2
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2009-2019
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 

#define LEDSD PCout(0)
#define LEDData_On GPIOC->BRR = 0x01<<1
#define LEDData_Off GPIOC->BSRR = 0x01<<1
//#define LEDData PCout(1)
//#define LEDCanH_On GPIOC->BRR = 0x01<<2
//#define LEDCanH_Off GPIOC->BSRR = 0x01<<2
#define LEDCanH PCout(2)
#define LEDCanL PCout(3)

void LED_Init(void);//��ʼ��

		 				    
#endif
