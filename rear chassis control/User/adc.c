 /*********
 20170802
 an example 
 ************/

#include "stm32f10x.h"
#include "adc.h"
#define CH_NUM  3           
#include "stm32f10x_dma.h"

static uint16_t ad_value[CH_NUM]={0};
/** @para none
  * @ret  none
  * @brief adc???gpio???
  */
void adc_gpio_init(void)
{
    GPIO_InitTypeDef gpio;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
    gpio.GPIO_Mode = GPIO_Mode_AIN;                         //???????
    gpio.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2;   //ADC1 --channel0,1,2
    gpio.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA,&gpio); 
}
/** @para none
  * @ret  none
  * @brief adc??
  */
void adc_init(void)
{
    ADC_InitTypeDef adc;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
    RCC_ADCCLKConfig(RCC_PCLK2_Div6);                       //??ad???72/6=12M,ad???????14M
    
    ADC_DeInit(ADC1);
    adc.ADC_Mode                = ADC_Mode_Independent;     //???????
    adc.ADC_ContinuousConvMode  = ENABLE;                   //??????
    adc.ADC_DataAlign           = ADC_DataAlign_Right;      //?????
    adc.ADC_ExternalTrigConv    = ADC_ExternalTrigConv_None;//???????
    adc.ADC_NbrOfChannel        = CH_NUM ;                       //??3???
    adc.ADC_ScanConvMode        = ENABLE;                   //??????
    ADC_Init(ADC1,&adc);
    //?????ADC???,???????????
    ADC_RegularChannelConfig(ADC1, ADC_Channel_0, 1, ADC_SampleTime_55Cycles5);
    ADC_RegularChannelConfig(ADC1, ADC_Channel_1, 2, ADC_SampleTime_55Cycles5);
    ADC_RegularChannelConfig(ADC1, ADC_Channel_2, 3, ADC_SampleTime_55Cycles5);
    //??adc?DMA
    ADC_DMACmd(ADC1,ENABLE);
    //??ADC1
    ADC_Cmd(ADC1, ENABLE);
    /* Enable ADC1 reset calibaration register */   
    ADC_ResetCalibration(ADC1);  //????
    /* Check the end of ADC1 reset calibration register */
    while(ADC_GetResetCalibrationStatus(ADC1));  //????????
    /* Start ADC1 calibaration */
    ADC_StartCalibration(ADC1); //????
    /* Check the end of ADC1 calibration */
    while(ADC_GetCalibrationStatus(ADC1));   //??????


}
/** @para none
  * @ret  none
  * @brief ??dma
  */
void adc_dma_init()
{
    DMA_InitTypeDef dma;
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
    
    DMA_DeInit(DMA1_Channel1);
    dma.DMA_PeripheralBaseAddr  = (u32)&(ADC1->DR);     //DMA????????
    dma.DMA_MemoryBaseAddr      = (u32)&ad_value;       //???????
    dma.DMA_DIR                 = DMA_DIR_PeripheralSRC;//DMA??????SRC??,????????
    dma.DMA_M2M                 = DMA_M2M_Disable;      //M2M????
    dma.DMA_PeripheralDataSize  = DMA_PeripheralDataSize_HalfWord;//?????????16?
    dma.DMA_MemoryDataSize      = DMA_MemoryDataSize_HalfWord;  //DMA??????,HalfWord???16?    
    dma.DMA_BufferSize          = CH_NUM;                       //DMA????CH_NUM?
    dma.DMA_MemoryInc           = DMA_MemoryInc_Enable;         //???????,????????
    dma.DMA_PeripheralInc       = DMA_PeripheralInc_Disable;    //???????,????????
    dma.DMA_Mode                = DMA_Mode_Circular;            //????,???????
    dma.DMA_Priority            = DMA_Priority_High;            //DMA????
    DMA_Init(DMA1_Channel1,&dma); 
}
/** @para none
  * @ret  none
  * @brief ??adc??
  */
void adc_config(void)
{
    adc_gpio_init();
    adc_dma_init();
    adc_init();
}
/** @para none
  * @ret  none
  * @brief ??adc,????
  */
void adc_start(void)
{
    /* Start ADC1 Software Conversion */ 
    ADC_SoftwareStartConvCmd(ADC1, ENABLE);  //??????,ADC??DMA???????RAM?? 
    DMA_Cmd(DMA1_Channel1,ENABLE);    
}


/** @para ADC_Channel --- (ADC_Channel0 ~ ADC_Channel17)
  * @ret  ADC_Channel_x ?ADC?
  */
uint16_t get_ChannelVale(uint8_t ADC_Channel)
{
    uint16_t ret = 0;
    switch(ADC_Channel)
    {
        case ADC_Channel_0:
            ret = ad_value[0];
            break;
        case ADC_Channel_1:
            ret = ad_value[1];
            break;
        case ADC_Channel_2:
            ret = ad_value[2];
            break;
    }
    return ret;
}


