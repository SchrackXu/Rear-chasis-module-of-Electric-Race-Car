#include "led.h"

//初始化PA1到PA5为输出口.并使能时钟		    
//输出模块 IO初始化
void LED_Config(void)
{ 
	GPIO_InitTypeDef  GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOB, ENABLE);	 //使能PA和PC端口时钟
	// GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2 继电器 GPIO_Pin_8|GPIO_Pin_9 LED
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_8|GPIO_Pin_9;	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10;	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
	GPIO_Init(GPIOB, &GPIO_InitStructure);	
	GPIO_ResetBits(GPIOB,GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_10);
GPIO_SetBits(GPIOB,GPIO_Pin_8|GPIO_Pin_9);
	
	//根据设定参数初始化GPIOB.5? 
}

void Relay_Init(void)
{
	GPIO_ResetBits(GPIOC,GPIO_Pin_0);
	GPIO_ResetBits(GPIOC,GPIO_Pin_1);
	GPIO_ResetBits(GPIOC,GPIO_Pin_2);
	
	GPIO_ResetBits(GPIOC,GPIO_Pin_8);
	GPIO_ResetBits(GPIOC,GPIO_Pin_9);
}

