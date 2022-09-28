#ifndef __CAN_H
#define __CAN_H	 
#include "sys.h"	
#include "dataProcess.h"

//CAN接收RX0中断使能
#define CAN_RX0_INT_ENABLE	0		//0,不使能;1,使能.		
#define CAN2_RX0_INT_ENABLE	0		//0,不使能;1,使能.	
										 							 				    
u8 CAN1_Mode_Init(u8 tsjw,u8 tbs2,u8 tbs1,u16 brp,u8 mode);//CAN1初始化
u8 CAN2_Mode_Init(u8 tsjw,u8 tbs2,u8 tbs1,u16 brp,u8 mode);//CAN2初始化
 
u8 Can1_Send_Msg(uint32_t id,u8* msg,u8 len);	//发送数据
u8 Can2_Send_Msg(uint32_t id,u8* msg,u8 len);

u8 Can1_Receive_Msg(u8 *buf);	//接收数据
u8 Can2_Receive_Msg(u8 *buf);


extern unsigned char VMS300[len300];
extern unsigned char VMS301[len301];
extern unsigned char VMS401[len401];
extern unsigned char MC302[len302];
extern unsigned char MC303[len303];
extern unsigned char MC304[len304];
extern unsigned char MC305[len305];
extern unsigned char MC306[len306];
extern unsigned char MC307[len307];
extern unsigned char MC308[len308];
extern unsigned char MC309[len309];
extern unsigned char BMS310[len310];
extern unsigned char BMS311[len311];
extern unsigned char BMS412[len412];
extern unsigned char CCF320[len320];
extern unsigned char CCF321[len321];
//extern unsigned char CCF328[len328];
extern unsigned char CCR322[len322];
extern unsigned char CCF323[len323];
extern unsigned char CCF324[len324];
extern unsigned char CCF325[len325];
extern unsigned char DTU340[len340];
extern unsigned char STWP350[len350];
extern unsigned char VMS360[len360];
extern unsigned char VMS329[len329];
extern unsigned char VMS32A[len32A];
extern unsigned char VMS32B[len32B];
extern unsigned char VMS32C[len32C];

extern u8 flag300;
extern u8 flag301;
extern u8 flag302;
extern u8 flag303;
extern u8 flag304;
extern u8 flag305;
extern u8 flag306;
extern u8 flag307;
extern u8 flag308;
extern u8 flag309;
extern u8 flag310;
extern u8 flag311;
extern u8 flag320;
extern u8 flag321;
extern u8 flag328;
extern u8 flag322;
extern u8 flag323;
extern u8 flag324;
extern u8 flag325;
extern u8 flag340;
extern u8 flag350;
extern u8 flag360;
extern u8 flag401;
extern u8 flag412;
extern u8 flag329;
extern u8 flag32A;
extern u8 flag32B;
extern u8 flag32C;


#endif

















