#include "can.h"
#include "led.h"
#include "delay.h"
#include "usart.h"
#include "dataprocess.h"

//CAN��ʼ��
//tsjw:����ͬ����Ծʱ�䵥Ԫ.��Χ:1~3; CAN_SJW_1tq	 CAN_SJW_2tq CAN_SJW_3tq CAN_SJW_4tq
//tbs2:ʱ���2��ʱ�䵥Ԫ.��Χ:1~8;
//tbs1:ʱ���1��ʱ�䵥Ԫ.��Χ:1~16;	  CAN_BS1_1tq ~CAN_BS1_16tq
//brp :�����ʷ�Ƶ��.��Χ:1~1024;(ʵ��Ҫ��1,Ҳ����1~1024) tq=(brp)*tpclk1
//ע�����ϲ����κ�һ����������Ϊ0,�������.
//������=Fpclk1/((tsjw+tbs1+tbs2)*brp);
//mode:0,��ͨģʽ;1,�ػ�ģʽ;
//Fpclk1��ʱ���ڳ�ʼ����ʱ������Ϊ36M,�������CAN_Normal_Init(1,8,7,5,1);
//������Ϊ:36M/((1+8+7)*5)=450Kbps
//����ֵ:0,��ʼ��OK;
//    ����,��ʼ��ʧ��;

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

//��������
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

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);//ʹ��PORTAʱ��          											 

  	RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1, ENABLE);//ʹ��CAN1ʱ��	

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//��������
    GPIO_Init(GPIOA, &GPIO_InitStructure);		//��ʼ��IO
   
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;//��������
    GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��IO
	  
 	//CAN��Ԫ����
 	CAN_InitStructure.CAN_TTCM=DISABLE;						 //��ʱ�䴥��ͨ��ģʽ  //
 	CAN_InitStructure.CAN_ABOM=DISABLE;						 //����Զ����߹���	 //
  	CAN_InitStructure.CAN_AWUM=DISABLE;						 //˯��ģʽͨ���������(���CAN->MCR��SLEEPλ)//
  	CAN_InitStructure.CAN_NART=ENABLE;						 	//��ֹ�����Զ����� //
  	CAN_InitStructure.CAN_RFLM=DISABLE;						 //���Ĳ�����,�µĸ��Ǿɵ� // 
  	CAN_InitStructure.CAN_TXFP=DISABLE;						 //���ȼ��ɱ��ı�ʶ������ //
  	CAN_InitStructure.CAN_Mode= mode;	         //ģʽ���ã� mode:0,��ͨģʽ;1,�ػ�ģʽ; //
  	//���ò�����
  	CAN_InitStructure.CAN_SJW=tsjw;				//����ͬ����Ծ���(Tsjw)Ϊtsjw+1��ʱ�䵥λ  CAN_SJW_1tq	 CAN_SJW_2tq CAN_SJW_3tq CAN_SJW_4tq
  	CAN_InitStructure.CAN_BS1=tbs1; //Tbs1=tbs1+1��ʱ�䵥λCAN_BS1_1tq ~CAN_BS1_16tq
  	CAN_InitStructure.CAN_BS2=tbs2;//Tbs2=tbs2+1��ʱ�䵥λCAN_BS2_1tq ~	CAN_BS2_8tq
  	CAN_InitStructure.CAN_Prescaler=brp;            //��Ƶϵ��(Fdiv)Ϊbrp+1	//
  	CAN_Init(CAN1, &CAN_InitStructure);            // ��ʼ��CAN1 

 	CAN_FilterInitStructure.CAN_FilterNumber=0;	  //������0
 	CAN_FilterInitStructure.CAN_FilterMode=CAN_FilterMode_IdMask; 
  	CAN_FilterInitStructure.CAN_FilterScale=CAN_FilterScale_32bit; //32λ 
  	CAN_FilterInitStructure.CAN_FilterIdHigh=0x0000;////32λID
  	CAN_FilterInitStructure.CAN_FilterIdLow=0x0000;
  	CAN_FilterInitStructure.CAN_FilterMaskIdHigh=0x0000;//32λMASK
  	CAN_FilterInitStructure.CAN_FilterMaskIdLow=0x0000;
  	CAN_FilterInitStructure.CAN_FilterFIFOAssignment=CAN_Filter_FIFO0;//������0������FIFO0
 	CAN_FilterInitStructure.CAN_FilterActivation=ENABLE; //���������0

  	CAN_FilterInit(&CAN_FilterInitStructure);//�˲�����ʼ��
#if CAN_RX0_INT_ENABLE
	
	CAN_ITConfig(CAN1,CAN_IT_FMP0,ENABLE);//FIFO0��Ϣ�Һ��ж�����.		    
  
  	NVIC_InitStructure.NVIC_IRQChannel = CAN1_RX0_IRQn;
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;     // �����ȼ�Ϊ1
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;            // �����ȼ�Ϊ0
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  	NVIC_Init(&NVIC_InitStructure);
#endif
	return 0;
}   
//#if CAN_RX0_INT_ENABLE	//ʹ��RX0�ж�
////�жϷ�����			    
//void CAN1_RX1_IRQHandler(void)
//{
//  	CanRxMsg RxMessage;
////	int i=0;
////  CAN_Receive(CAN1, 0, &RxMessage);
////	for(i=0;i<8;i++)
////	printf("rxbuf[%d]:%d\r\n",i,RxMessage.Data[i]);
//	
//	CAN_Receive(CAN1, CAN_FIFO0, &RxMessage);//��ȡ����	
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

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);//ʹ��PORTAʱ��	                   											 

  	RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN2, ENABLE);//ʹ��CAN1ʱ��	

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//��������
    GPIO_Init(GPIOB, &GPIO_InitStructure);		//��ʼ��IO
   
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;//��������
    GPIO_Init(GPIOB, &GPIO_InitStructure);//��ʼ��IO
	  
 	//CAN��Ԫ����
 	CAN_InitStructure.CAN_TTCM=DISABLE;						 //��ʱ�䴥��ͨ��ģʽ  //
 	CAN_InitStructure.CAN_ABOM=DISABLE;						 //����Զ����߹���	 //
  	CAN_InitStructure.CAN_AWUM=DISABLE;						 //˯��ģʽͨ���������(���CAN->MCR��SLEEPλ)//
  	CAN_InitStructure.CAN_NART=ENABLE;						 	//��ֹ�����Զ����� //
  	CAN_InitStructure.CAN_RFLM=DISABLE;						 //���Ĳ�����,�µĸ��Ǿɵ� // 
  	CAN_InitStructure.CAN_TXFP=DISABLE;						 //���ȼ��ɱ��ı�ʶ������ //
  	CAN_InitStructure.CAN_Mode= mode;	         //ģʽ���ã� mode:0,��ͨģʽ;1,�ػ�ģʽ; //
  	//���ò�����
  	CAN_InitStructure.CAN_SJW=tsjw;				//����ͬ����Ծ���(Tsjw)Ϊtsjw+1��ʱ�䵥λ  CAN_SJW_1tq	 CAN_SJW_2tq CAN_SJW_3tq CAN_SJW_4tq
  	CAN_InitStructure.CAN_BS1=tbs1; //Tbs1=tbs1+1��ʱ�䵥λCAN_BS1_1tq ~CAN_BS1_16tq
  	CAN_InitStructure.CAN_BS2=tbs2;//Tbs2=tbs2+1��ʱ�䵥λCAN_BS2_1tq ~	CAN_BS2_8tq
  	CAN_InitStructure.CAN_Prescaler=brp;            //��Ƶϵ��(Fdiv)Ϊbrp+1	//
  	CAN_Init(CAN2, &CAN_InitStructure);            // ��ʼ��CAN1 

 	CAN_FilterInitStructure.CAN_FilterNumber=14;	  //������0
 	CAN_FilterInitStructure.CAN_FilterMode=CAN_FilterMode_IdMask; 
  	CAN_FilterInitStructure.CAN_FilterScale=CAN_FilterScale_32bit; //32λ 
  	CAN_FilterInitStructure.CAN_FilterIdHigh=0x0000;////32λID
  	CAN_FilterInitStructure.CAN_FilterIdLow=0x0000;
	CAN_FilterInitStructure.CAN_FilterMaskIdHigh=0x0000;//32λMASK
  	CAN_FilterInitStructure.CAN_FilterMaskIdLow=0x0000;
  	CAN_FilterInitStructure.CAN_FilterFIFOAssignment=CAN_Filter_FIFO0;//������0������FIFO0
 	CAN_FilterInitStructure.CAN_FilterActivation=ENABLE; //���������0

  	CAN_FilterInit(&CAN_FilterInitStructure);//�˲�����ʼ��
#if CAN2_RX0_INT_ENABLE
	
	CAN_ITConfig(CAN2,CAN_IT_FMP0,ENABLE);//FIFO0��Ϣ�Һ��ж�����.		    
  
  	NVIC_InitStructure.NVIC_IRQChannel = CAN2_RX1_IRQn;
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;     // �����ȼ�Ϊ1
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;            // �����ȼ�Ϊ0
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  	NVIC_Init(&NVIC_InitStructure);
#endif
	return 0;
}
//#if CAN2_RX0_INT_ENABLE	//ʹ��RX0�ж�
////�жϷ�����			    
//void CAN2_RX0_IRQHandler(void)
//{
//  	CanRxMsg RxMessage;
////	int i=0;
////  CAN_Receive(CAN1, 0, &RxMessage);
////	for(i=0;i<8;i++)
////	printf("rxbuf[%d]:%d\r\n",i,RxMessage.Data[i]);
//	
//	CAN_Receive(CAN2, CAN_FIFO0, &RxMessage);//��ȡ����	
//    
//	if (RxMessage.StdId == 0x300)	{copyArrayList(VMS300, RxMessage.Data);flag300 = 1;}
//	if (RxMessage.StdId == 0x301)	{copyArrayList(VMS301, RxMessage.Data);flag301 = 1;}
//	
//}
//#endif


//can����һ������(�̶���ʽ:IDΪ0X12,��׼֡,����֡)	
//len:���ݳ���(���Ϊ8)				     
//msg:����ָ��,���Ϊ8���ֽ�.
//����ֵ:0,�ɹ�;
//		 ����,ʧ��;
u8 Can1_Send_Msg(uint32_t id,u8* msg,u8 len)
{	
  u8 mbox;
  u16 i=0;
  CanTxMsg TxMessage;
  TxMessage.StdId=id;					 // ��׼��ʶ��Ϊ0
  TxMessage.ExtId=0x12;				 // ������չ��ʾ����29λ��
  TxMessage.IDE=0;			 // ʹ����չ��ʶ��
  TxMessage.RTR=0;		 // ��Ϣ����Ϊ����֡��һ֡8λ
  TxMessage.DLC=len;							 // ������֡��Ϣ
  for(i=0;i<len;i++)
  TxMessage.Data[i]=msg[i];				 // ��һ֡��Ϣ          
  mbox= CAN_Transmit(CAN1, &TxMessage);   
  i=0;
  while((CAN_TransmitStatus(CAN1, mbox)==CAN_TxStatus_Failed)&&(i<0XFFF))i++;	//�ȴ����ͽ���
  if(i>=0XFFF)return 1;
  return 0;		
}

u8 Can2_Send_Msg(uint32_t id,u8* msg,u8 len)
{	
  u8 mbox;
  u16 i=0;
  CanTxMsg TxMessage;
  TxMessage.StdId=id;					 // ��׼��ʶ��Ϊ0
  TxMessage.ExtId=0x12;				 // ������չ��ʾ����29λ��
  TxMessage.IDE=0;			 // ʹ����չ��ʶ��
  TxMessage.RTR=0;		 // ��Ϣ����Ϊ����֡��һ֡8λ
  TxMessage.DLC=len;							 // ������֡��Ϣ
  for(i=0;i<len;i++)
  TxMessage.Data[i]=msg[i];				 // ��һ֡��Ϣ          
  mbox= CAN_Transmit(CAN2, &TxMessage);   
  i=0;
  while((CAN_TransmitStatus(CAN2, mbox)==CAN_TxStatus_Failed)&&(i<0XFFF))i++;	//�ȴ����ͽ���
  if(i>=0XFFF)return 1;
  return 0;		
}


//can�ڽ������ݲ�ѯ
//buf:���ݻ�����;	 
//����ֵ:0,�����ݱ��յ�;
//		 ����,���յ����ݳ���;
u8 Can1_Receive_Msg(u8 *buf)
{		   		   
 	u32 i;
	CanRxMsg RxMessage;
    if( CAN_MessagePending(CAN1,CAN_FIFO0)==0)return 0;		//û�н��յ�����,ֱ���˳� 
    CAN_Receive(CAN1, CAN_FIFO0, &RxMessage);//��ȡ����	
	
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
    if( CAN_MessagePending(CAN2,CAN_FIFO0)==0)return 0;		//û�н��յ�����,ֱ���˳� 
    CAN_Receive(CAN2, CAN_FIFO0, &RxMessage);//��ȡ����	
    
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














