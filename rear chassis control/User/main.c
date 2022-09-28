#include "led.h"
#include "usart1.h"
#include "can.h"
#include "timer.h"
#include "adc.h"
#include "stm32f10x_dma.h"
#include "delay.h"

#define offset 120

extern uint8_t TxM323[8];		//cc_rear_sensor_b
extern uint8_t TxM324[6];		//cc_rear_sensor_c
extern u8 InputInfo;
extern u16 AD_Value[3];
extern uint8_t key;  
				
u8 TxM322[8];		//cc_rear_sensor_a
uint8_t Rx[8];
   //接收到的数据数组的长度
uint8_t key2;
// uint8_t RxM[8];			 //接收数据数组
u16 adcx_save0;//adc for DPRR
u16 adcx_save1;//adc for DPRL
u16 adcx_save2;//adc for DPTR

int main(void)
{
	int i;int j;
	float led0pwmval=0;
	u8 dir=1;	  
	uint8_t	ReadValueC0;
	uint8_t	ReadValueC2;
	NVIC_Configuration();
	USART_Config();
	LED_Config();		  					//初始化与LED连接的硬件接口PD8-PD13 PC0~PC3
	Relay_Init();							//继电器上电时断开
	adc_config();
	adc_start();
	CAN1_Config(CAN_SJW_1tq, CAN_BS2_8tq, CAN_BS1_9tq, 4, CAN_Mode_Normal);		//CAN初始化普通模式,波特率500Kbps 
	CAN2_Config(CAN_SJW_1tq, CAN_BS2_4tq, CAN_BS1_4tq, 4, CAN_Mode_Normal);		//传感器通讯协议波特率为1M
	TIM3_Int_Init(99,7199);	//定时器中断发送can
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);		//使能指定的ADC1的软件转换启动功能	
	DMA_Cmd(DMA1_Channel1, ENABLE);
	NVIC_Configuration(); 	 //设置NVIC中断分组2:2位抢占优先级，2位响应优先级
 	TIM3_PWM_Init(7199,199);	 //不分频。PWM频率=72000/900=8Khz   第一次299，第二次39  使用50Hz(7200,200)
	TIM_SetCompare2(TIM3,610);
	
	while(1) {	  
//			if(dir)led0pwmval+=0.001;
//		    else led0pwmval-=0.001;
// 		    if(led0pwmval>720)dir=0;
//		    if(led0pwmval==0)dir=1;										 
//		    TIM_SetCompare2(TIM3,led0pwmval);	
//		    TIM_SetCompare2(TIM3,720);
//				while(i<20000000)
//				i++;
//				i=0;
//			    TIM_SetCompare2(TIM3,181);//led0pwmval值为720时转180°，可控角度为45°-225°
//			while(j<20000000)
//				j++;
//			j=0;
//		
//			TIM_SetCompare2(TIM3,90);
//		delay_ms(1000);
		   // TIM_SetCompare2(TIM3,750);	//600时舵的上边与电机壳平行，750时大概45度
		//delay_ms(1000);		
		GPIO_ResetBits(GPIOB,GPIO_Pin_9);
		adcx_save0 = get_ChannelVale(ADC_Channel_0);	//读取PA0电压 DPRL
		adcx_save1 = get_ChannelVale(ADC_Channel_1);			//读取PA1电压 DPRR
		adcx_save2 = get_ChannelVale(ADC_Channel_2);
		if(GPIO_ReadOutputDataBit(GPIOC, GPIO_Pin_0))
			ReadValueC0 = 0;
		else
			ReadValueC0 = 1;
		if(GPIO_ReadOutputDataBit(GPIOC, GPIO_Pin_2))
			ReadValueC2 = 0;
		else
			ReadValueC2 = 1;
		if(key2) {
			// TxM322[3] = Rx[2];
			 //TxM322[4] = Rx[3];
			 TxM322[5] = Rx[0];
			 TxM322[6] = Rx[1];
		}
		TxM322[0] = (adcx_save0)&0x00FF;
		TxM322[1] =0;
		TxM322[1] = (adcx_save0>>8)&0x000F;
 	  TxM322[1] |= (adcx_save1&0x000F)<<4;
		TxM322[2] = ((adcx_save1>>4)&0x00FF);
		 
		TxM322[3]=adcx_save2&0x00FF;
		TxM322[7]=0;
		TxM322[4]=(adcx_save2>>8)&0x000F;
		

		key2 = CAN2_Receive_Msg(Rx);					
		key = CAN1_Receive_Msg();

//			delay_ms(1);
		if(key)//接收到主控控制信息
		{GPIO_ResetBits(GPIOB,GPIO_Pin_8);

			if(InputInfo&0x08)//取DRS
//				{
//				if(dir)led0pwmval++;
//		    else led0pwmval--;
// 		    if(led0pwmval>300)dir=0;
//		    if(led0pwmval==0)dir=1;									
//		    TIM_SetCompare2(TIM3,led0pwmval);	
//				}
//			else
//			{
//				 TIM_SetCompare2(TIM3,300);
//			}
//		
			{
			TIM_SetCompare2(TIM3,795);
			}//看具体情况调占空比
			else
			{
				TIM_SetCompare2(TIM3,610);//角度0°到45°时似乎无法定位
			}//即占空比0-1/40时无法定位

			if(InputInfo&0x20)//取刹车灯
			{
				GPIO_SetBits(GPIOB,GPIO_Pin_6);
			}
			else
			{
				GPIO_ResetBits(GPIOB,GPIO_Pin_6);
			}
			if(InputInfo&0x40)//取蜂鸣器
			{
				GPIO_SetBits(GPIOB,GPIO_Pin_7);
			}
			else
			{
				GPIO_ResetBits(GPIOB,GPIO_Pin_7);
			}
				if(InputInfo&0x10)//取水泵
			{
				GPIO_SetBits(GPIOB,GPIO_Pin_10);
			}
			else
			{
				GPIO_ResetBits(GPIOB,GPIO_Pin_10);
			}
			
    
		}

	}
}
