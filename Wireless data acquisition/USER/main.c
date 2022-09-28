#include "led.h"
#include "delay.h"
#include "sys.h"
#include "usart.h"	 
#include "can.h" 
#include "timer.h"
#include "wireless.h"
#include "ff.h"
#include "mmc_sd.h"
#include "exfuns.h"
#include "ds1302.h"
#include "dataprocess.h"
#include "test_part.h"
#include "string.h"
 //GPS声明
// void errorLog(int num);
// void parseGpsBuffer(void);
// void printGpsBuffer(void);
//----------------------------------------------------------------------------------------------------------------------------
 int main(void)
 {	
	
    unsigned char arraylist[8]={'2','2','3','4','5','6','7','8'};
//	 u8 arraylist[8]={1,0,1,0,0,0,0,0};
//	 char arraylist[1] ={0x20};
	FRESULT res;
//	uint8_t time[8]; 
	char time[20];
	char dirname[]="17-06-06";
	char txtname[]="17-06-06/00-00.txt";
	int i;
	unsigned char FLA310[]={'1'};
  unsigned char FLA_310[]={'0'};
	
	 
	delay_init();	    	 //延时函数初始化	  
	NVIC_Configuration(); 	 //设置NVIC中断分组2:2位抢占优先级，2位响应优先级
	uart_init(9600);
	uart_init2(115200);	 	//串口初始化为9600
	LED_Init();		  		//初始化与LED连接的硬件接口 	
	TIM3_Int_Init(49,799);	//10Khz的计数频率，计数到1250为125ms
	wireless_init();
//	clrStruct();            //GPS数组
	exfuns_init();          //为fatfs相关变量申请内存 
	DS1302_Init(&SysDate);  //初始化RTC
	CAN1_Mode_Init(CAN_SJW_1tq,CAN_BS2_8tq,CAN_BS1_9tq,4,CAN_Mode_Normal);//CAN1初始化正常模式,波特率500Kbps 
	CAN2_Mode_Init(CAN_SJW_1tq,CAN_BS2_8tq,CAN_BS1_9tq,4,CAN_Mode_Normal);//CAN2初始化正常模式,波特率500Kbps 
//	CAN1_Mode_Init(CAN_SJW_1tq,CAN_BS2_8tq,CAN_BS1_9tq,4,CAN_Mode_LoopBack);//CAN1初始化正常模式,波特率500Kbps 
//	CAN2_Mode_Init(CAN_SJW_1tq,CAN_BS2_8tq,CAN_BS1_9tq,4,CAN_Mode_LoopBack);//CAN2初始化正常模式,波特率500Kbps 

//    DS1302_DateSet(&SysDate);//给时钟设值要用吗?
//  SD_Initialize();            //使用时取消注释
	//test_rtc();
  //test_sd();
	//GPIO_ResetBits(GPIOC,GPIO_Pin_0);
 // test_wireless();
	//test_SDandRTC();
//    while(1)
//	{
//		test_sd();
////	}

//	while(SD_Initialize()); //检测SD卡              暂时
//	LEDSD = 0;                                      //注释

//   fatbuf = arraylist;
// 	f_mount(fs[0],"0:",0); 					//挂载SD卡      从这里到最后一行先注释，等到无线调出来再说
// 	res = f_readdir(&dir,&fileinfo);
// 	DS1302_time(time);
// 	time2dirname(time,dirname);
// 	time2txtname(time,txtname);
// 	if(res != FR_OK){
// 		res = f_mkdir(dirname);
// 	}
// 	else{
// 		res = f_opendir(&dir,dirname);
// 	}
// 	res = f_open(file, txtname, FA_CREATE_ALWAYS  | FA_WRITE);
// 	f_close(file);
// 	f_mount(0,"0:",0);// Unregister a work area before discard it
//// 	
////// 	
//	while(1)
//		{
		//GPIO_ResetBits(GPIOC,GPIO_Pin_0);
	  //dataProcess(BMS311,len311,head311);//实验
// 		parseGpsBuffer();//得到位置信息
// 		printGpsBuffer();//
// 		wirelessSend(Save_Data.GPS_Buffer,GPS_Buffer_Length);//发送GPS信息
// 		i=0;
//  		f_mount(fs[0],"0:",0);                                      //等无线调出来在搞这个
//  		res = f_opendir(&dir,dirname);
//  		res = f_open(file, txtname, FA_OPEN_EXISTING  | FA_WRITE);
// 		while(1)//原来是这里是i<50
// 		{ if (flag310==1){dataProcess(FLA_310,len321,'k');}
// 			delay_ms(100);
// 		  if(flag310==0){dataProcess(FLA310,len321,'k');}
// 			if(sendFlag == 1)
// 			{ GPIO_ResetBits(GPIOC,GPIO_Pin_0);
// 				sendFlag = 0;
// 				res = f_lseek(file, f_size(file)); //指针移到文件最后
// 				DS1302_time(time);
// 				if(flag300==1){flag300 = 0;dataProcess(VMS300,len300,head300);sdwrite(time,"300",VMS300,len300,file,bw);}
// 				if(flag301==1){flag301 = 0;dataProcess(VMS301,len301,head301);sdwrite(time,"301",VMS301,len301,file,bw);}
// 				if(flag401==1){flag401 = 0;dataProcess(VMS401,len401,head401);sdwrite(time,"401",VMS401,len401,file,bw);}
// 				if(flag302==1){flag302 = 0;dataProcess(MC302,len302,head302);sdwrite(time,"302",MC302,len302,file,bw);}
// 				if(flag303==1){flag303 = 0;dataProcess(MC303,len303,head303);sdwrite(time,"303",MC303,len303,file,bw);}
// 				if(flag304==1){flag304 = 0;dataProcess(MC304,len304,head304);sdwrite(time,"304",MC304,len304,file,bw);}
// 				if(flag305==1){flag305 = 0;dataProcess(MC305,len305,head305);sdwrite(time,"305",MC305,len305,file,bw);}
// 				if(flag306==1){flag306 = 0;dataProcess(MC306,len306,head306);sdwrite(time,"306",MC306,len306,file,bw);}
// 				if(flag307==1){flag307 = 0;dataProcess(MC307,len307,head307);sdwrite(time,"307",MC307,len307,file,bw);}
// 				if(flag308==1){flag308 = 0;dataProcess(MC308,len308,head308);sdwrite(time,"308",MC308,len308,file,bw);}
// 				if(flag309==1){flag309 = 0;dataProcess(MC309,len309,head309);sdwrite(time,"309",MC309,len309,file,bw);}
// 				if(flag310==1){flag310 = 0;dataProcess(BMS310,len310,head310);sdwrite(time,"310",BMS310,len310,file,bw);}
// 				else{dataProcess(FLA310,len321,'k');}
// 				if(flag311==1){flag311 = 0;dataProcess(BMS311,len311,head311);sdwrite(time,"311",BMS311,len311,file,bw);}
// 				if(flag412==1){flag412 = 0;dataProcess(BMS412,len412,head412);sdwrite(time,"412",BMS412,len412,file,bw);}
// 				if(flag320==1){flag320 = 0;dataProcess(CCF320,len320,head320);sdwrite(time,"320",CCF320,len320,file,bw);}
// 				if(flag321==1){flag321 = 0;dataProcess(CCF321,len321,head321);sdwrite(time,"321",CCF321,len321,file,bw);}
// 				//if(flag328==1){flag328 = 0;dataProcess(CCF328,len328,head328);sdwrite(time,"328",CCF328,len328,file,bw);}
// 				//if(flag322==1){flag322 = 0;dataProcess(CCR322,len322,head322);sdwrite(time,"322",CCR322,len322,file,bw);}
// 				if(flag323==1){flag323 = 0;dataProcess(CCF323,len323,head323);sdwrite(time,"323",CCF323,len323,file,bw);}
// 				if(flag324==1){flag324 = 0;dataProcess(CCF324,len324,head324);sdwrite(time,"324",CCF324,len324,file,bw);}
// 			//	if(flag325==1){flag325 = 0;dataProcess(CCF325,len325,head325);sdwrite(time,"325",CCF325,len325,file,bw);}
// 				if(flag340==1){flag340 = 0;dataProcess(DTU340,len340,head340);sdwrite(time,"340",DTU340,len340,file,bw);}
// 				if(flag350==1){flag350 = 0;dataProcess(STWP350,len350,head350);sdwrite(time,"350",STWP350,len350,file,bw);}
// 				
// 				
// //				if(flag300==1)	   {flag300 = 0;dataProcess300(VMS300);res = f_write(file, VMS300, sizeof(VMS300), &bw);}
// //				else if(flag401==1){flag401 = 0;dataProcess401(VMS401);res = f_write(file, VMS401, sizeof(VMS401), &bw);}
// //				else if(flag310==1){flag310 = 0;dataProcess310(BMS310);res = f_write(file, BMS310, sizeof(BMS310), &bw);}
// //				else if(flag311==1){flag311 = 0;dataProcess311(BMS311);res = f_write(file, BMS311, sizeof(BMS311), &bw);}
// //				else if(flag412==1){flag412 = 0;dataProcess412(BMS412);res = f_write(file, BMS412, sizeof(BMS412), &bw);}
// //				else if(flag320==1){flag320 = 0;dataProcess320(CCF320);res = f_write(file, CCF320, sizeof(CCF320), &bw);}
// //				else if(flag321==1){flag321 = 0;dataProcess321(CCF321);res = f_write(file, CCF321, sizeof(CCF321), &bw);}
// //				else if(flag328==1){flag328 = 0;dataProcess328(CCF328);res = f_write(file, CCF328, sizeof(CCF328), &bw);}
// //				else if(flag322==1){flag322 = 0;dataProcess322(CCR322);res = f_write(file, CCR322, sizeof(CCR322), &bw);}
// //				else if(flag329==1){flag329 = 0;dataProcess329(CCR329);res = f_write(file, CCR329, sizeof(CCR329), &bw);}
// //				else if(flag340==1){flag340 = 0;dataProcess340(DTU340);res = f_write(file, DTU340, sizeof(DTU340), &bw);}
// //				else if(flag350==1){flag350 = 0;dataProcess350(STWP350);res = f_write(file, STWP350, sizeof(STWP350), &bw);}
// //				res = f_write(file, "\r\n", 2, &bw);
// 				
// 				
// //				if(flag300==1)	   {flag300 = 0;dataProcess300(VMS300);}
// //				else if(flag401==1){flag401 = 0;dataProcess401(VMS401);}
// //				else if(flag310==1){flag310 = 0;dataProcess310(BMS310);}
// //				else if(flag311==1){flag311 = 0;dataProcess311(BMS311);}
// //				else if(flag412==1){flag412 = 0;dataProcess412(BMS412);}
// //				else if(flag320==1){flag320 = 0;dataProcess320(CCF320);}
// //				else if(flag321==1){flag321 = 0;dataProcess321(CCF321);}
// //				else if(flag328==1){flag328 = 0;dataProcess328(CCF328);LEDCanH = 0;}
// //				else if(flag322==1){flag322 = 0;dataProcess322(CCR322);LEDCanH = 0;}
// //				else if(flag329==1){flag329 = 0;dataProcess329(CCR329);LEDCanL = 0;}
// //				else if(flag340==1){flag340 = 0;dataProcess340(DTU340);}
// //				else if(flag350==1){flag350 = 0;dataProcess350(STWP350);}
// 				i++;
// 			}
// 		}
/////////////////////////////////////////////////////////////////////////////////////////////////////
while(1)//原来是这里是i<50
		{
			if (flag300==1){dataProcess(FLA_310,len321,'a');}
		  if (flag310==1){dataProcess(FLA_310,len321,'k');}
		  if (flag305==1){dataProcess(FLA_310,len321,'f');}
		  if (flag306==1){dataProcess(FLA_310,len321,'g');}
	    if (flag307==1){dataProcess(FLA_310,len321,'h');}
		  if (flag308==1){dataProcess(FLA_310,len321,'i');}
		  if (flag350==1){dataProcess(FLA_310,len321,'u');}
		  if (flag401==1){dataProcess(FLA_310,len321,'v');}
			if(sendFlag == 1)
			{// GPIO_ResetBits(GPIOC,GPIO_Pin_0);
				delay_ms(100);
				sendFlag = 0;
//				res = f_lseek(file, f_size(file)); //指针移到文件最后
//				DS1302_time(time);
				if(flag300==1){flag300 = 0;dataProcess(VMS300,len300,head300);sdwrite(time,"300",VMS300,len300,file,bw);}
				else {dataProcess(FLA310,len321,'a');}
				if(flag301==1){flag301 = 0;dataProcess(VMS301,len301,head301);sdwrite(time,"301",VMS301,len301,file,bw);}
				if(flag401==1){flag401 = 0;dataProcess(VMS401,len401,head401);sdwrite(time,"401",VMS401,len401,file,bw);}
				else {dataProcess(FLA310,len321,'v');}
				if(flag302==1){flag302 = 0;dataProcess(MC302,len302,head302);sdwrite(time,"302",MC302,len302,file,bw);}
				if(flag303==1){flag303 = 0;dataProcess(MC303,len303,head303);sdwrite(time,"303",MC303,len303,file,bw);}
				if(flag304==1){flag304 = 0;dataProcess(MC304,len304,head304);sdwrite(time,"304",MC304,len304,file,bw);}
				if(flag305==1){flag305 = 0;dataProcess(MC305,len305,head305);sdwrite(time,"305",MC305,len305,file,bw);}
				else {dataProcess(FLA310,len321,'f');}
				if(flag306==1){flag306 = 0;dataProcess(MC306,len306,head306);sdwrite(time,"306",MC306,len306,file,bw);}
        else {dataProcess(FLA310,len321,'g');}
				if(flag307==1){flag307 = 0;dataProcess(MC307,len307,head307);sdwrite(time,"307",MC307,len307,file,bw);}
				else {dataProcess(FLA310,len321,'h');}
				if(flag308==1){flag308 = 0;dataProcess(MC308,len308,head308);sdwrite(time,"308",MC308,len308,file,bw);}
			  else {dataProcess(FLA310,len321,'i');}
				if(flag309==1){flag309 = 0;dataProcess(MC309,len309,head309);sdwrite(time,"309",MC309,len309,file,bw);}
				if(flag310==1){flag310 = 0;dataProcess(BMS310,len310,head310);sdwrite(time,"310",BMS310,len310,file,bw);}
				else {dataProcess(FLA310,len321,'k');}
				if(flag311==1){flag311 = 0;dataProcess(BMS311,len311,head311);sdwrite(time,"311",BMS311,len311,file,bw);}
				if(flag412==1){flag412 = 0;dataProcess(BMS412,len412,head412);sdwrite(time,"412",BMS412,len412,file,bw);}
				if(flag320==1){flag320 = 0;dataProcess(CCF320,len320,head320);sdwrite(time,"320",CCF320,len320,file,bw);}
				if(flag321==1){flag321 = 0;dataProcess(CCF321,len321,head321);sdwrite(time,"321",CCF321,len321,file,bw);}
				//if(flag328==1){flag328 = 0;dataProcess(CCF328,len328,head328);sdwrite(time,"328",CCF328,len328,file,bw);}
				if(flag322==1){flag322 = 0;dataProcess(CCR322,len322,head322);sdwrite(time,"322",CCR322,len322,file,bw);}
				if(flag323==1){flag323 = 0;dataProcess(CCF323,len323,head323);sdwrite(time,"323",CCF323,len323,file,bw);}
				if(flag324==1){flag324 = 0;dataProcess(CCF324,len324,head324);sdwrite(time,"324",CCF324,len324,file,bw);}
				if(flag325==1){flag325 = 0;dataProcess(CCF325,len325,head325);sdwrite(time,"325",CCF325,len325,file,bw);}
				if(flag340==1){flag340 = 0;dataProcess(DTU340,len340,head340);sdwrite(time,"340",DTU340,len340,file,bw);}
				if(flag360==1){flag360 = 0;dataProcess(VMS360,len360,head360);sdwrite(time,"360",VMS360,len360,file,bw);}
				if(flag329==1){flag329 = 0;dataProcess(VMS329,len329,head329);sdwrite(time,"329",VMS329,len329,file,bw);}
				if(flag32A==1){flag32A = 0;dataProcess(VMS32A,len32A,head32A);sdwrite(time,"32A",VMS32A,len32A,file,bw);}
				if(flag32B==1){flag32B = 0;dataProcess(VMS32B,len32B,head32B);sdwrite(time,"32B",VMS32B,len32B,file,bw);}
				if(flag32C==1){flag32C = 0;dataProcess(VMS32C,len32C,head32C);sdwrite(time,"32C",VMS32C,len32C,file,bw);}
				if(flag350==1){flag350 = 0;dataProcess(STWP350,len350,head350);sdwrite(time,"350",STWP350,len350,file,bw);}
				else {dataProcess(FLA310,len321,'u');}
				}
				
				
//				if(flag300==1)	   {flag300 = 0;dataProcess300(VMS300);res = f_write(file, VMS300, sizeof(VMS300), &bw);}
//				else if(flag401==1){flag401 = 0;dataProcess401(VMS401);res = f_write(file, VMS401, sizeof(VMS401), &bw);}
//				else if(flag310==1){flag310 = 0;dataProcess310(BMS310);res = f_write(file, BMS310, sizeof(BMS310), &bw);}
//				else if(flag311==1){flag311 = 0;dataProcess311(BMS311);res = f_write(file, BMS311, sizeof(BMS311), &bw);}
//				else if(flag412==1){flag412 = 0;dataProcess412(BMS412);res = f_write(file, BMS412, sizeof(BMS412), &bw);}
//				else if(flag320==1){flag320 = 0;dataProcess320(CCF320);res = f_write(file, CCF320, sizeof(CCF320), &bw);}
//				else if(flag321==1){flag321 = 0;dataProcess321(CCF321);res = f_write(file, CCF321, sizeof(CCF321), &bw);}
//				else if(flag328==1){flag328 = 0;dataProcess328(CCF328);res = f_write(file, CCF328, sizeof(CCF328), &bw);}
//				else if(flag322==1){flag322 = 0;dataProcess322(CCR322);res = f_write(file, CCR322, sizeof(CCR322), &bw);}
//				else if(flag329==1){flag329 = 0;dataProcess329(CCR329);res = f_write(file, CCR329, sizeof(CCR329), &bw);}
//				else if(flag340==1){flag340 = 0;dataProcess340(DTU340);res = f_write(file, DTU340, sizeof(DTU340), &bw);}
//				else if(flag350==1){flag350 = 0;dataProcess350(STWP350);res = f_write(file, STWP350, sizeof(STWP350), &bw);}
//				res = f_write(file, "\r\n", 2, &bw);
				
				
//				if(flag300==1)	   {flag300 = 0;dataProcess300(VMS300);}
//				else if(flag401==1){flag401 = 0;dataProcess401(VMS401);}
//				else if(flag310==1){flag310 = 0;dataProcess310(BMS310);}
//				else if(flag311==1){flag311 = 0;dataProcess311(BMS311);}
//				else if(flag412==1){flag412 = 0;dataProcess412(BMS412);}
//				else if(flag320==1){flag320 = 0;dataProcess320(CCF320);}
//				else if(flag321==1){flag321 = 0;dataProcess321(CCF321);}
//				else if(flag328==1){flag328 = 0;dataProcess328(CCF328);LEDCanH = 0;}
//				else if(flag322==1){flag322 = 0;dataProcess322(CCR322);LEDCanH = 0;}
//				else if(flag329==1){flag329 = 0;dataProcess329(CCR329);LEDCanL = 0;}
//				else if(flag340==1){flag340 = 0;dataProcess340(DTU340);}
//				else if(flag350==1){flag350 = 0;dataProcess350(STWP350);}
				i++;
			
//			if (flag300==1){dataProcess(FLA_310,len321,'a');}
//		  if (flag310==1){dataProcess(FLA_310,len321,'k');}
//		  if (flag305==1){dataProcess(FLA_310,len321,'f');}
//		  if (flag306==1){dataProcess(FLA_310,len321,'g');}
//	    if (flag307==1){dataProcess(FLA_310,len321,'h');}
//		  if (flag308==1){dataProcess(FLA_310,len321,'i');}
//		  if (flag350==1){dataProcess(FLA_310,len321,'u');}
//		  if (flag401==1){dataProcess(FLA_310,len321,'v');}
			delay_ms(100);
//      if(flag300==0){dataProcess(FLA310,len321,'a');}
//			if(flag401==0){dataProcess(FLA310,len321,'v');}
//			if(flag305==0){dataProcess(FLA310,len321,'f');}
//			if(flag306==0){dataProcess(FLA310,len321,'g');}
//			if(flag307==0){dataProcess(FLA310,len321,'h');}
//			if(flag308==0){dataProcess(FLA310,len321,'i');}
//			if(flag310==0){dataProcess(FLA310,len321,'k');}
//			if(flag350==0){dataProcess(FLA310,len321,'u');}
					
		}

//		f_close(file);
//		f_mount(0,"0:",0);// Unregister a work area before discard it
//	}


}
//--------------------------------------GPS-----------------------------------------------------
// void errorLog(int num)
// {
// 	
// 	while (1)
// 	{
// 	  	printf("ERROR%d\r\n",num);
// 	}
// }

// void parseGpsBuffer()
// {
// 	char *subString;
// 	char *subStringNext;
// 	char i = 0;
// 	if (Save_Data.isGetData)
// 	{
// 		Save_Data.isGetData = false;
// 		printf("**************\r\n");
// 		printf(Save_Data.GPS_Buffer);

// 		
// 		for (i = 0 ; i <= 6 ; i++)
// 		{
// 			if (i == 0)
// 			{
// 				if ((subString = strstr(Save_Data.GPS_Buffer, ",")) == NULL)
// 					errorLog(1);	//解析错误
// 			}
// 			else
// 			{
// 				subString++;
// 				if ((subStringNext = strstr(subString, ",")) != NULL)
// 				{
// 					char usefullBuffer[2]; 
// 					switch(i)
// 					{
// 						case 1:memcpy(Save_Data.UTCTime, subString, subStringNext - subString);break;	//获取UTC时间
// 						case 2:memcpy(usefullBuffer, subString, subStringNext - subString);break;	//获取UTC时间
// 						case 3:memcpy(Save_Data.latitude, subString, subStringNext - subString);break;	//获取纬度信息
// 						case 4:memcpy(Save_Data.N_S, subString, subStringNext - subString);break;	//获取N/S
// 						case 5:memcpy(Save_Data.longitude, subString, subStringNext - subString);break;	//获取经度信息
// 						case 6:memcpy(Save_Data.E_W, subString, subStringNext - subString);break;	//获取E/W

// 						default:break;
// 					}

// 					subString = subStringNext;
// 					Save_Data.isParseData = true;
// 					if(usefullBuffer[0] == 'A')
// 						Save_Data.isUsefull = true;
// 					else if(usefullBuffer[0] == 'V')
// 						Save_Data.isUsefull = false;

// 				}
// 				else
// 				{
// 					errorLog(2);	//解析错误
// 				}
// 			}


// 		}
// 	}
// }
// //----------------------------------------------------------------------------------------------------------------------
// void printGpsBuffer()
// {
// 	if (Save_Data.isParseData)
// 	{
// 		Save_Data.isParseData = false;
// 		
// 		printf("Save_Data.UTCTime = ");
// 		printf(Save_Data.UTCTime);
// 		printf("\r\n");

// 		if(Save_Data.isUsefull)
// 		{
// 			Save_Data.isUsefull = false;
// 			printf("Save_Data.latitude = ");
// 			printf(Save_Data.latitude);
// 			printf("\r\n");


// 			printf("Save_Data.N_S = ");
// 			printf(Save_Data.N_S);
// 			printf("\r\n");

// 			printf("Save_Data.longitude = ");
// 			printf(Save_Data.longitude);
// 			printf("\r\n");

// 			printf("Save_Data.E_W = ");
// 			printf(Save_Data.E_W);
// 			printf("\r\n");
// 		}
// 		else
// 		{
// 			printf("GPS DATA is not usefull!\r\n");
// 		}
// 		
// 	}
//}




