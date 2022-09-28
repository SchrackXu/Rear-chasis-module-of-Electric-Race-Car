#include "timer.h"
#include "led.h"
#include "can.h"
#include "delay.h"
unsigned char FLA310[]='1';
unsigned char FLA_310[]='0';

#define DIR __disable_irq()
#define EIR __enable_irq()

int sendFlag;

//通用定时器3中断初始化
//这里时钟选择为APB1的2倍，而APB1为36M
//arr：自动重装值。
//psc：时钟预分频数
//这里使用的是定时器3!
void TIM3_Int_Init(u16 arr,u16 psc)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); //时钟使能
	
	//定时器TIM3初始化
	TIM_TimeBaseStructure.TIM_Period = arr; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值	
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //设置用来作为TIMx时钟频率除数的预分频值
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //根据指定的参数初始化TIMx的时间基数单位
 
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE ); //使能指定的TIM3中断,允许更新中断

	//中断优先级NVIC设置
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;  //TIM3中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //先占优先级0级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;  //从优先级3级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure);  //初始化NVIC寄存器


	TIM_Cmd(TIM3, ENABLE);  //使能TIMx					 
}
//定时器3中断服务程序
void TIM3_IRQHandler(void)   //TIM3中断
{
	CanRxMsg RxMessage;
	u8 canbuf[8];
	u8 key;
	u8 key2;
	
	if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)
	{
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
//		sendFlag = 1;
//		LEDSD = !LEDSD;
		
		//Can1接收数据
		//if (flag308 = 0);wirelessSend('yH
		DIR;
		key=Can1_Receive_Msg(canbuf);
		if(key)	{sendFlag = 1;}
		
		//Can2接收数据 
		key2=Can2_Receive_Msg(canbuf);
    if(key2) sendFlag = 1;
// 		if(flag310==0)
// 		{
// 			delay_ms(500);
// 			if(flag310==0)
// 				dataProcess(FLA310,len321,'k');
// 		}
// 		if(flag310==1)
// 			dataProcess(FLA_310,len321,'k');
			
		EIR;
	}
}












