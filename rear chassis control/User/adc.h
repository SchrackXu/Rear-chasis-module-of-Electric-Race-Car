#ifndef __ADC_H
#define __ADC_H	
#include "sys.h"
#include "stm32f10x_dma.h"

void Adc_Init1(void);
void Adc_Init2(void);
void DMA_Configuration(void);

void adc_gpio_init(void);
void adc_init(void);
void adc_dma_init(void);
void adc_config(void);
void adc_start(void);
uint16_t get_ChannelVale(uint8_t ADC_Channel);

uint16_t Get_Adc1(uint8_t ch); 
uint16_t Get_Adc_Average1(uint8_t ch,uint8_t times); 
uint16_t Get_Adc2(uint8_t ch); 
uint16_t Get_Adc_Average2(uint8_t ch,uint8_t times); 
 
#endif 
