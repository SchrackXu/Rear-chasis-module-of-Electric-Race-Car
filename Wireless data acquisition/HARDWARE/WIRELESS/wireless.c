#include "sys.h"
#include "wireless.h"
#include "delay.h"
#include "led.h"

u8 wirelessBusy()
{
	return !(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_10));
}

void wireless_init()
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	//ʹ��PORT Bʱ��      ԭ����PA8��9
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5|GPIO_Pin_6;	//MD0 MD1 �˿�����
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 			//�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;			//IO���ٶ�Ϊ50MHz
	GPIO_Init(GPIOB, &GPIO_InitStructure);								//�����趨������ʼ��GPIOB.5 6

//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;						//AUX �˿�����
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; //��������
//	GPIO_Init(GPIOA, &GPIO_InitStructure);					 			//�����趨������ʼ��GPIOB.7
	
	GPIOB->BRR = 0x03<<8;//MD0 MD1 =0������ģʽ
	GPIO_ResetBits(GPIOB,GPIO_Pin_5|GPIO_Pin_6);
}

void wirelessSend(char arrayList[], int length)
{
	int i;
	while(wirelessBusy());
	LEDData_On;
	for(i=0; i<length; i++){
		
		USART_SendData(USART2, arrayList[i]);
		while(USART_GetFlagStatus(USART2,USART_FLAG_TXE)!=SET);
		delay_ms(4);
		
	}
	LEDData_Off;
}

void wirelessSend_u8(u8 arrayList[], int length)
{
	int i;
	while(wirelessBusy());
	LEDData_On;
	for(i=0; i<length; i++){
		
		USART_SendData(USART2, arrayList[i]);
		while(USART_GetFlagStatus(USART2,USART_FLAG_TXE)!=SET);
		delay_ms(4);
		
	}
	LEDData_Off;
}