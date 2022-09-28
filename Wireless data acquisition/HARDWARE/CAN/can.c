#include "can.h"
#include "led.h"
#include "delay.h"
#include "usart.h"
#include "dataprocess.h"

//CAN初始化
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

unsigned char VMS300[len300];
unsigned char VMS301[len301];
unsigned char VMS401[len401];
unsigned char MC302[len302];
unsigned char MC303[len303];
unsigned char MC304[len304];
unsigned char MC305[len305];
unsigned char MC306[len306];
unsigned char MC307[len307];
unsigned char MC308[len308];
unsigned char MC309[len309];
unsigned char BMS310[len310];
unsigned char BMS311[len311];
unsigned char BMS412[len412];
unsigned char CCF320[len320];
unsigned char CCF321[len321];
//unsigned char CCF328[len328];
unsigned char CCR322[len322];
unsigned char CCF323[len323];
unsigned char CCF324[len324];
unsigned char CCF325[len325];
unsigned char DTU340[len340];
unsigned char STWP350[len350];
unsigned char VMS360[len360];
unsigned char VMS329[len329];
unsigned char VMS32A[len32A];
unsigned char VMS32B[len32B];
unsigned char VMS32C[len32C];

u8 flag300;
u8 flag301;
u8 flag302;
u8 flag303;
u8 flag304;
u8 flag305;
u8 flag306;
u8 flag307;
u8 flag308;
u8 flag309;
u8 flag310;
u8 flag311;
u8 flag320;
u8 flag321;
u8 flag328;
u8 flag322;
u8 flag323;
u8 flag324;
u8 flag325;
u8 flag340;
u8 flag350;
u8 flag360;
u8 flag401;
u8 flag412;
u8 flag329;
u8 flag32A;
u8 flag32B;
u8 flag32C;

//复制数组
void copyArrayList(unsigned char target[], unsigned char toBeCopied[])
{
	int i;
	for(i=0; i<8; i++){
		target[i] = toBeCopied[i];
	}
}


u8 CAN1_Mode_Init(u8 tsjw,u8 tbs2,u8 tbs1,u16 brp,u8 mode)
{

	GPIO_InitTypeDef GPIO_InitStructure; 
	CAN_InitTypeDef        CAN_InitStructure;
 	CAN_FilterInitTypeDef  CAN_FilterInitStructure;
#if CAN_RX0_INT_ENABLE 
   	NVIC_InitTypeDef  NVIC_InitStructure;
#endif

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);//使能PORTA时钟          											 

  	RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1, ENABLE);//使能CAN1时钟	

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//复用推挽
    GPIO_Init(GPIOA, &GPIO_InitStructure);		//初始化IO
   
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;//上拉输入
    GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化IO
	  
 	//CAN单元设置
 	CAN_InitStructure.CAN_TTCM=DISABLE;						 //非时间触发通信模式  //
 	CAN_InitStructure.CAN_ABOM=DISABLE;						 //软件自动离线管理	 //
  	CAN_InitStructure.CAN_AWUM=DISABLE;						 //睡眠模式通过软件唤醒(清除CAN->MCR的SLEEP位)//
  	CAN_InitStructure.CAN_NART=ENABLE;						 	//禁止报文自动传送 //
  	CAN_InitStructure.CAN_RFLM=DISABLE;						 //报文不锁定,新的覆盖旧的 // 
  	CAN_InitStructure.CAN_TXFP=DISABLE;						 //优先级由报文标识符决定 //
  	CAN_InitStructure.CAN_Mode= mode;	         //模式设置： mode:0,普通模式;1,回环模式; //
  	//设置波特率
  	CAN_InitStructure.CAN_SJW=tsjw;				//重新同步跳跃宽度(Tsjw)为tsjw+1个时间单位  CAN_SJW_1tq	 CAN_SJW_2tq CAN_SJW_3tq CAN_SJW_4tq
  	CAN_InitStructure.CAN_BS1=tbs1; //Tbs1=tbs1+1个时间单位CAN_BS1_1tq ~CAN_BS1_16tq
  	CAN_InitStructure.CAN_BS2=tbs2;//Tbs2=tbs2+1个时间单位CAN_BS2_1tq ~	CAN_BS2_8tq
  	CAN_InitStructure.CAN_Prescaler=brp;            //分频系数(Fdiv)为brp+1	//
  	CAN_Init(CAN1, &CAN_InitStructure);            // 初始化CAN1 

 	CAN_FilterInitStructure.CAN_FilterNumber=0;	  //过滤器0
 	CAN_FilterInitStructure.CAN_FilterMode=CAN_FilterMode_IdMask; 
  	CAN_FilterInitStructure.CAN_FilterScale=CAN_FilterScale_32bit; //32位 
  	CAN_FilterInitStructure.CAN_FilterIdHigh=0x0000;////32位ID
  	CAN_FilterInitStructure.CAN_FilterIdLow=0x0000;
  	CAN_FilterInitStructure.CAN_FilterMaskIdHigh=0x0000;//32位MASK
  	CAN_FilterInitStructure.CAN_FilterMaskIdLow=0x0000;
  	CAN_FilterInitStructure.CAN_FilterFIFOAssignment=CAN_Filter_FIFO0;//过滤器0关联到FIFO0
 	CAN_FilterInitStructure.CAN_FilterActivation=ENABLE; //激活过滤器0

  	CAN_FilterInit(&CAN_FilterInitStructure);//滤波器初始化
#if CAN_RX0_INT_ENABLE
	
	CAN_ITConfig(CAN1,CAN_IT_FMP0,ENABLE);//FIFO0消息挂号中断允许.		    
  
  	NVIC_InitStructure.NVIC_IRQChannel = CAN1_RX0_IRQn;
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;     // 主优先级为1
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;            // 次优先级为0
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  	NVIC_Init(&NVIC_InitStructure);
#endif
	return 0;
}   
//#if CAN_RX0_INT_ENABLE	//使能RX0中断
////中断服务函数			    
//void CAN1_RX1_IRQHandler(void)
//{
//  	CanRxMsg RxMessage;
////	int i=0;
////  CAN_Receive(CAN1, 0, &RxMessage);
////	for(i=0;i<8;i++)
////	printf("rxbuf[%d]:%d\r\n",i,RxMessage.Data[i]);
//	
//	CAN_Receive(CAN1, CAN_FIFO0, &RxMessage);//读取数据	
//	LEDCanH = 0;
//	CAN_FIFORelease(CAN1,CAN_FIFO0);
//	CAN_ClearITPendingBit(CAN1,CAN_IT_FMP0);
//	
//    if (RxMessage.StdId == 0x300)	{copyArrayList(VMS300, RxMessage.Data);flag300 = 1;}
//	if (RxMessage.StdId == 0x401)	{copyArrayList(VMS401, RxMessage.Data);flag401 = 1;}
//	if (RxMessage.StdId == 0x310)	{copyArrayList(BMS310, RxMessage.Data);flag310 = 1;}
//	if (RxMessage.StdId == 0x311)	{copyArrayList(BMS311, RxMessage.Data);flag311 = 1;}
//	if (RxMessage.StdId == 0x412)	{copyArrayList(BMS412, RxMessage.Data);flag412 = 1;}
//	if (RxMessage.StdId == 0x320)	{copyArrayList(CCF320, RxMessage.Data);flag320 = 1;}
//	if (RxMessage.StdId == 0x321)	{copyArrayList(CCF321, RxMessage.Data);flag321 = 1;}
//	if (RxMessage.StdId == 0x328)	{copyArrayList(CCF328, RxMessage.Data);flag328 = 1;}
//	if (RxMessage.StdId == 0x322)	{copyArrayList(CCR322, RxMessage.Data);flag322 = 1;}
//	if (RxMessage.StdId == 0x329)	{copyArrayList(CCR329, RxMessage.Data);flag329 = 1;}
//	if (RxMessage.StdId == 0x340)	{copyArrayList(DTU340, RxMessage.Data);flag340 = 1;}
//	if (RxMessage.StdId == 0x350)	{copyArrayList(STWP350, RxMessage.Data);flag350 = 1;}
//	
//}
//#endif


u8 CAN2_Mode_Init(u8 tsjw,u8 tbs2,u8 tbs1,u16 brp,u8 mode)
{

    GPIO_InitTypeDef GPIO_InitStructure; 
	CAN_InitTypeDef        CAN_InitStructure;
 	CAN_FilterInitTypeDef  CAN_FilterInitStructure;
#if CAN2_RX0_INT_ENABLE 
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
	  
 	//CAN单元设置
 	CAN_InitStructure.CAN_TTCM=DISABLE;						 //非时间触发通信模式  //
 	CAN_InitStructure.CAN_ABOM=DISABLE;						 //软件自动离线管理	 //
  	CAN_InitStructure.CAN_AWUM=DISABLE;						 //睡眠模式通过软件唤醒(清除CAN->MCR的SLEEP位)//
  	CAN_InitStructure.CAN_NART=ENABLE;						 	//禁止报文自动传送 //
  	CAN_InitStructure.CAN_RFLM=DISABLE;						 //报文不锁定,新的覆盖旧的 // 
  	CAN_InitStructure.CAN_TXFP=DISABLE;						 //优先级由报文标识符决定 //
  	CAN_InitStructure.CAN_Mode= mode;	         //模式设置： mode:0,普通模式;1,回环模式; //
  	//设置波特率
  	CAN_InitStructure.CAN_SJW=tsjw;				//重新同步跳跃宽度(Tsjw)为tsjw+1个时间单位  CAN_SJW_1tq	 CAN_SJW_2tq CAN_SJW_3tq CAN_SJW_4tq
  	CAN_InitStructure.CAN_BS1=tbs1; //Tbs1=tbs1+1个时间单位CAN_BS1_1tq ~CAN_BS1_16tq
  	CAN_InitStructure.CAN_BS2=tbs2;//Tbs2=tbs2+1个时间单位CAN_BS2_1tq ~	CAN_BS2_8tq
  	CAN_InitStructure.CAN_Prescaler=brp;            //分频系数(Fdiv)为brp+1	//
  	CAN_Init(CAN2, &CAN_InitStructure);            // 初始化CAN1 

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
  
  	NVIC_InitStructure.NVIC_IRQChannel = CAN2_RX1_IRQn;
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;     // 主优先级为1
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;            // 次优先级为0
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  	NVIC_Init(&NVIC_InitStructure);
#endif
	return 0;
}
//#if CAN2_RX0_INT_ENABLE	//使能RX0中断
////中断服务函数			    
//void CAN2_RX0_IRQHandler(void)
//{
//  	CanRxMsg RxMessage;
////	int i=0;
////  CAN_Receive(CAN1, 0, &RxMessage);
////	for(i=0;i<8;i++)
////	printf("rxbuf[%d]:%d\r\n",i,RxMessage.Data[i]);
//	
//	CAN_Receive(CAN2, CAN_FIFO0, &RxMessage);//读取数据	
//    
//	if (RxMessage.StdId == 0x300)	{copyArrayList(VMS300, RxMessage.Data);flag300 = 1;}
//	if (RxMessage.StdId == 0x301)	{copyArrayList(VMS301, RxMessage.Data);flag301 = 1;}
//	
//}
//#endif


//can发送一组数据(固定格式:ID为0X12,标准帧,数据帧)	
//len:数据长度(最大为8)				     
//msg:数据指针,最大为8个字节.
//返回值:0,成功;
//		 其他,失败;
u8 Can1_Send_Msg(uint32_t id,u8* msg,u8 len)
{	
  u8 mbox;
  u16 i=0;
  CanTxMsg TxMessage;
  TxMessage.StdId=id;					 // 标准标识符为0
  TxMessage.ExtId=0x12;				 // 设置扩展标示符（29位）
  TxMessage.IDE=0;			 // 使用扩展标识符
  TxMessage.RTR=0;		 // 消息类型为数据帧，一帧8位
  TxMessage.DLC=len;							 // 发送两帧信息
  for(i=0;i<len;i++)
  TxMessage.Data[i]=msg[i];				 // 第一帧信息          
  mbox= CAN_Transmit(CAN1, &TxMessage);   
  i=0;
  while((CAN_TransmitStatus(CAN1, mbox)==CAN_TxStatus_Failed)&&(i<0XFFF))i++;	//等待发送结束
  if(i>=0XFFF)return 1;
  return 0;		
}

u8 Can2_Send_Msg(uint32_t id,u8* msg,u8 len)
{	
  u8 mbox;
  u16 i=0;
  CanTxMsg TxMessage;
  TxMessage.StdId=id;					 // 标准标识符为0
  TxMessage.ExtId=0x12;				 // 设置扩展标示符（29位）
  TxMessage.IDE=0;			 // 使用扩展标识符
  TxMessage.RTR=0;		 // 消息类型为数据帧，一帧8位
  TxMessage.DLC=len;							 // 发送两帧信息
  for(i=0;i<len;i++)
  TxMessage.Data[i]=msg[i];				 // 第一帧信息          
  mbox= CAN_Transmit(CAN2, &TxMessage);   
  i=0;
  while((CAN_TransmitStatus(CAN2, mbox)==CAN_TxStatus_Failed)&&(i<0XFFF))i++;	//等待发送结束
  if(i>=0XFFF)return 1;
  return 0;		
}


//can口接收数据查询
//buf:数据缓存区;	 
//返回值:0,无数据被收到;
//		 其他,接收的数据长度;
u8 Can1_Receive_Msg(u8 *buf)
{		   		   
 	u32 i;
	CanRxMsg RxMessage;
    if( CAN_MessagePending(CAN1,CAN_FIFO0)==0)return 0;		//没有接收到数据,直接退出 
    CAN_Receive(CAN1, CAN_FIFO0, &RxMessage);//读取数据	
	
    if (RxMessage.StdId == 0x300)	{copyArrayList(VMS300, RxMessage.Data);flag300 = 1;}
	if (RxMessage.StdId == 0x401)	{copyArrayList(VMS401, RxMessage.Data);flag401 = 1;}
	if (RxMessage.StdId == 0x310)	{copyArrayList(BMS310, RxMessage.Data);flag310 = 1;}
	if (RxMessage.StdId == 0x311)	{copyArrayList(BMS311, RxMessage.Data);flag311 = 1;}
	if (RxMessage.StdId == 0x412)	{copyArrayList(BMS412, RxMessage.Data);flag412 = 1;}
	if (RxMessage.StdId == 0x320)	{copyArrayList(CCF320, RxMessage.Data);flag320 = 1;}
	if (RxMessage.StdId == 0x321)	{copyArrayList(CCF321, RxMessage.Data);flag321 = 1;}
//	if (RxMessage.StdId == 0x328)	{copyArrayList(CCF328, RxMessage.Data);flag328 = 1;}
	if (RxMessage.StdId == 0x322)	{copyArrayList(CCR322, RxMessage.Data);flag322 = 1;}
	if (RxMessage.StdId == 0x323)	{copyArrayList(CCF323, RxMessage.Data);flag323 = 1;}
	if (RxMessage.StdId == 0x324)	{copyArrayList(CCF324, RxMessage.Data);flag324 = 1;}
	//if (RxMessage.StdId == 0x325)	{copyArrayList(CCF325, RxMessage.Data);flag325 = 1;}
	if (RxMessage.StdId == 0x340)	{copyArrayList(DTU340, RxMessage.Data);flag340 = 1;}
	if (RxMessage.StdId == 0x350)	{copyArrayList(STWP350, RxMessage.Data);flag350 = 1;}

	
	return RxMessage.DLC;	
}

u8 Can2_Receive_Msg(u8 *buf)
{		   		   
 	u32 i;
	CanRxMsg RxMessage;
    if( CAN_MessagePending(CAN2,CAN_FIFO0)==0)return 0;		//没有接收到数据,直接退出 
    CAN_Receive(CAN2, CAN_FIFO0, &RxMessage);//读取数据	
    
	if (RxMessage.StdId == 0x300)	{copyArrayList(VMS300, RxMessage.Data);flag300 = 1;}
	if (RxMessage.StdId == 0x301)	{copyArrayList(VMS301, RxMessage.Data);flag301 = 1;}
	if (RxMessage.StdId == 0x302)	{copyArrayList(MC302, RxMessage.Data);flag302 = 1;}
	if (RxMessage.StdId == 0x303)	{copyArrayList(MC303, RxMessage.Data);flag303 = 1;}
	if (RxMessage.StdId == 0x304)	{copyArrayList(MC304, RxMessage.Data);flag304 = 1;}
	if (RxMessage.StdId == 0x325)	{copyArrayList(MC305, RxMessage.Data);flag305 = 1;}
	if (RxMessage.StdId == 0x326)	{copyArrayList(MC306, RxMessage.Data);flag306 = 1;}
	if (RxMessage.StdId == 0x327)	{copyArrayList(MC307, RxMessage.Data);flag307 = 1;}
	if (RxMessage.StdId == 0x328)	{copyArrayList(MC308, RxMessage.Data);flag308 = 1;}
	if (RxMessage.StdId == 0x309)	{copyArrayList(MC309, RxMessage.Data);flag309 = 1;}
	if (RxMessage.StdId == 0x306)	{copyArrayList(VMS360, RxMessage.Data);flag360 = 1;}
	if (RxMessage.StdId == 0x329)	{copyArrayList(VMS329, RxMessage.Data);flag329 = 1;}
	if (RxMessage.StdId == 0x32A)	{copyArrayList(VMS32A, RxMessage.Data);flag32A = 1;}
	if (RxMessage.StdId == 0x32B)	{copyArrayList(VMS32B, RxMessage.Data);flag32B = 1;}
	if (RxMessage.StdId == 0x32C)	{copyArrayList(VMS32C, RxMessage.Data);flag32C = 1;}
	return RxMessage.DLC;	
}














