#include "can.h"
#include "led.h"
#include "delay.h"
#include "usart1.h"

//CAN初始化，使用定时器中断发送can，不使用can自带中断
//tsjw:重新同步跳跃时间单元.范围:1~3; CAN_SJW_1tq	 CAN_SJW_2tq CAN_SJW_3tq CAN_SJW_4tq
//tbs2:时间段2的时间单元.范围:1~8;
//tbs1:时间段1的时间单元.范围:1~16;	  CAN_BS1_1tq ~CAN_BS1_16tq
//brp :波特率分频器.范围:1~1024;(实际要加1,也就是1~1024) tq=(brp)*tpclk1
//注意以上参数任何一个都不能设为0,否则会乱.
//波特率=Fpclk1/((tsjw+tbs1+tbs2)*brp);
//mode:0,普通模式;1,回环模式;
//Fpclk1的时钟在初始化的时候设置为36M,如果设置CAN_Normal_Init(1,8,7,5,1);
//则波特率为:36M/((1+8+7)*5)=450Kbps
//返回值:0,初始化OK;
//    其他,初始化失败;

//CAN1 整车CAN 负责收取0x300 发送0x322 0x329 	500kb波特率
//CAN2 单独用来接收光学传感器信息 1M波特率 接收0x1ffffffa  &&  0x1ffffffb

uint8_t InputInfo;  	//继电器
uint8_t CAN1_Config(uint8_t tsjw,uint8_t tbs2,uint8_t tbs1,uint16_t brp,uint8_t mode)//500kb
{
	GPIO_InitTypeDef GPIO_InitStructure; 
	CAN_InitTypeDef  CAN_InitStructure;
	CAN_FilterInitTypeDef  CAN_FilterInitStructure;
#if CAN_RX0_INT_ENABLE 
	NVIC_InitTypeDef  NVIC_InitStructure;
#endif
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);//使能PORTA时钟	 ////                  											 
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1, ENABLE);//使能CAN1时钟	////

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;							/////
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//复用推挽
	GPIO_Init(GPIOA, &GPIO_InitStructure);					//初始化IO
 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;							//////
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;		//上拉输入
	GPIO_Init(GPIOA, &GPIO_InitStructure);					//初始化IO				//////
	// CAN单元设置
	CAN_InitStructure.CAN_TTCM = DISABLE;		//非时间触发通信模式  
	CAN_InitStructure.CAN_ABOM = DISABLE;		//软件自动离线管理	 
	CAN_InitStructure.CAN_AWUM = DISABLE;		//睡眠模式通过软件唤醒(清除CAN->MCR的SLEEP位)
	CAN_InitStructure.CAN_NART = ENABLE;		//禁止报文自动传送 
	CAN_InitStructure.CAN_RFLM = DISABLE;		//报文不锁定,新的覆盖旧的  
	CAN_InitStructure.CAN_TXFP = DISABLE;		//优先级由报文标识符决定 
	CAN_InitStructure.CAN_Mode = mode;	    //模式设置： mode:0,普通模式;1,回环模式; //
	// 设置波特率
	CAN_InitStructure.CAN_SJW = tsjw;				//重新同步跳跃宽度(Tsjw)为tsjw+1个时间单位  CAN_SJW_1tq	 CAN_SJW_2tq CAN_SJW_3tq CAN_SJW_4tq
	CAN_InitStructure.CAN_BS1 = tbs1; 			//Tbs1=tbs1+1个时间单位CAN_BS1_1tq ~CAN_BS1_16tq
	CAN_InitStructure.CAN_BS2 = tbs2;				//Tbs2=tbs2+1个时间单位CAN_BS2_1tq ~	CAN_BS2_8tq
	CAN_InitStructure.CAN_Prescaler = brp;  //分频系数(Fdiv)为brp+1	
	CAN_Init(CAN1, &CAN_InitStructure);   // 初始化CAN1    /////
// -------------- CAN filter 设置 -------------------//
	CAN_FilterInitStructure.CAN_FilterNumber = 0;	  //过滤器0
	CAN_FilterInitStructure.CAN_FilterMode = CAN_FilterMode_IdMask; 
	CAN_FilterInitStructure.CAN_FilterScale = CAN_FilterScale_32bit; //16位 
	CAN_FilterInitStructure.CAN_FilterIdHigh = 0x6000;//
	CAN_FilterInitStructure.CAN_FilterIdLow = 0x0000;
	CAN_FilterInitStructure.CAN_FilterMaskIdHigh = 0xFFFF;//32位MASK
	CAN_FilterInitStructure.CAN_FilterMaskIdLow = 0x0000;
	CAN_FilterInitStructure.CAN_FilterFIFOAssignment = CAN_Filter_FIFO0;//过滤器0关联到FIFO0
	CAN_FilterInitStructure.CAN_FilterActivation = ENABLE; //激活过滤器0
	CAN_FilterInit(&CAN_FilterInitStructure);//滤波器初始化
#if CAN_RX0_INT_ENABLE
	CAN_ITConfig(CAN1,CAN_IT_FMP0,ENABLE);												//FIFO0消息挂号中断允许.
	NVIC_InitStructure.NVIC_IRQChannel = USB_LP_CAN1_RX0_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;     // 主优先级为1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;            // 次优先级为0
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
#endif
	return 0;
}   

uint8_t CAN2_Config(uint8_t tsjw,uint8_t tbs2,uint8_t tbs1,uint16_t brp,uint8_t mode)//1Mb
{ 
	GPIO_InitTypeDef GPIO_InitStructure; 
	CAN_InitTypeDef  CAN_InitStructure;
 	CAN_FilterInitTypeDef CAN_FilterInitStructure;
#if CAN_RX2_INT_ENABLE 
  NVIC_InitTypeDef  NVIC_InitStructure;
#endif
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);//使能PORTA时钟	                   											 
 	RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN2, ENABLE);//使能CAN1时钟	
	
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//复用推挽
  GPIO_Init(GPIOB, &GPIO_InitStructure);		//初始化IO
   
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;//上拉输入
  GPIO_Init(GPIOB, &GPIO_InitStructure);//初始化IO
	// -------------- CAN 设置 -------------------//
 	CAN_InitStructure.CAN_TTCM = DISABLE;						 //非时间触发通信模式  //
 	CAN_InitStructure.CAN_ABOM = DISABLE;						 //软件自动离线管理	 //
	CAN_InitStructure.CAN_AWUM = DISABLE;						 //睡眠模式通过软件唤醒(清除CAN->MCR的SLEEP位)//
	CAN_InitStructure.CAN_NART = ENABLE;						 	//禁止报文自动传送 //
	CAN_InitStructure.CAN_RFLM = DISABLE;						 //报文不锁定,新的覆盖旧的 // 
	CAN_InitStructure.CAN_TXFP = DISABLE;						 //优先级由报文标识符决定 //
	CAN_InitStructure.CAN_Mode = mode;	         //模式设置： mode:0,普通模式;1,回环模式; //
	CAN_InitStructure.CAN_SJW = tsjw;				//重新同步跳跃宽度(Tsjw)为tsjw+1个时间单位  CAN_SJW_1tq	 CAN_SJW_2tq CAN_SJW_3tq CAN_SJW_4tq
	CAN_InitStructure.CAN_BS1 = tbs1; //Tbs1=tbs1+1个时间单位CAN_BS1_1tq ~CAN_BS1_16tq
	CAN_InitStructure.CAN_BS2 = tbs2;//Tbs2=tbs2+1个时间单位CAN_BS2_1tq ~	CAN_BS2_8tq
	CAN_InitStructure.CAN_Prescaler = brp;            //分频系数(Fdiv)为brp+1	//
	CAN_Init(CAN2, &CAN_InitStructure);            // 初始化CAN1 
  // -------------- CAN filter 设置 -------------------//
	CAN_FilterInitStructure.CAN_FilterNumber=14;	  //过滤器0
 	CAN_FilterInitStructure.CAN_FilterMode=CAN_FilterMode_IdMask; 
	CAN_FilterInitStructure.CAN_FilterScale=CAN_FilterScale_32bit; //32位 
	CAN_FilterInitStructure.CAN_FilterIdHigh=0x0000;////32位ID
	CAN_FilterInitStructure.CAN_FilterIdLow=0x0000;
	CAN_FilterInitStructure.CAN_FilterMaskIdHigh=0x0000;//32位MASK
	CAN_FilterInitStructure.CAN_FilterMaskIdLow=0x0000;
	CAN_FilterInitStructure.CAN_FilterFIFOAssignment=CAN_Filter_FIFO0;//过滤器0关联到FIFO0
 	CAN_FilterInitStructure.CAN_FilterActivation=ENABLE; //激活过滤器0
 	CAN_FilterInit(&CAN_FilterInitStructure);//滤波器初始化
#if CAN2_RX0_INT_ENABLE
	CAN_ITConfig(CAN2,CAN_IT_FMP0,ENABLE);//FIFO0消息挂号中断允许.		    
  NVIC_InitStructure.NVIC_IRQChannel = USB_LP_CAN2_RX0_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;     // 主优先级为1
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;            // 次优先级为0
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
#endif
	return 0;
}

uint8_t CAN_Send_Msg(uint8_t* TxM,uint8_t len, uint16_t id)	//发送CAN
{	
  uint8_t mbox;
  uint16_t i = 0;
  CanTxMsg TxMessage;
  TxMessage.StdId = id;		 // 标准标识符为id
  TxMessage.IDE = 0;			 // 使用扩展标识符
  TxMessage.RTR = 0;		   // 消息类型为数据帧，一帧8位
  TxMessage.DLC = len;							 // 发送len字节的信息
  for (i=0; i<len; i++)
		TxMessage.Data[i]=TxM[i];				 // 第一帧信息          
  mbox = CAN_Transmit(CAN1, &TxMessage);  
  i = 0;
  while((CAN_TransmitStatus(CAN1, mbox) == CAN_TxStatus_Failed) && (i<0XFFF))
		i++;															//等待发送结束
  if(i >= 0XFFF)
		return 1;
  return 0;		
}

//can口接收数据查询
//buf:数据缓存区;	 
//返回值:0,无数据被收到;
//其他,接收的数据长度;
uint8_t CAN1_Receive_Msg(void)							//接收主控0x300
{		   		   
	CanRxMsg RxMessage;
  if(CAN_MessagePending(CAN1,CAN_FIFO0) == 0)
		return 0;																			//没有接收到数据,直接退出
  CAN_Receive(CAN1, CAN_FIFO0, &RxMessage);				//读取数据
	if(RxMessage.StdId == 0x300)										//主控控制帧	
		InputInfo = RxMessage.Data[1];         
	return RxMessage.DLC;	
}

uint8_t CAN2_Receive_Msg(uint8_t* buf)									//接收光学传感器 SPD数据
{		   		
	CanRxMsg RxMessage;
	if (CAN_MessagePending(CAN2,CAN_FIFO0) == 0)
		return 0;																						//没有接收到数据,直接退出 
	CAN_Receive(CAN2, CAN_FIFO0, &RxMessage);							//读取数据	
	if(RxMessage.ExtId == 0x1ffffffb){
		buf[0] = RxMessage.Data[4];
		buf[1] = RxMessage.Data[5];
	}
	else if(RxMessage.ExtId == 0x1ffffffa){
		buf[2] = RxMessage.Data[2];
		buf[3] = RxMessage.Data[3];
	}
	return RxMessage.DLC;	
}
