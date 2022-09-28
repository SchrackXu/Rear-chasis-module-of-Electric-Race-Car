#include "test_part.h"
#include "led.h"
#include "delay.h"
#include "wireless.h"
#include "can.h"
#include "timer.h"
#include "ds1302.h"
#include "ff.h"
#include "mmc_sd.h"
#include "exfuns.h"
#include "dataprocess.h"

//除了test_sd,其他在主程序里测试时都要放在while里

/*******************************************
               测试LED
*******************************************/
void test_led()
{
	LEDSD = 0;
	LEDCanH = 0;
	LEDCanL = 0;
	LEDData_On;
	delay_ms(500);
	LEDSD = 1;
	LEDCanH = 1;
	LEDCanL = 1;
	LEDData_Off;
	delay_ms(500);	
}


/*******************************************
            测试无线发送
*******************************************/
void test_wireless()
{
	char arraylist[8]={'1','2','3','4','5','6','7','8'};
	while(1) {wirelessSend(arraylist,8);
	delay_ms(300);}
}


/*******************************************
            测试CAN(环回模式)
*******************************************/
void test_canloopback()
{
	u8 arraylist[8]={1,0,1,0,0,0,0,0};
	Can1_Send_Msg(0x401,arraylist,8);
	if(flag401==1)
	{
		flag401 = 0;
		LEDCanH = !LEDCanH;
		delay_ms(300);
	}
}


/*******************************************
       测试CAN(正常模式)和定时器中断
*******************************************/
void test_cannormal()
{
	if(sendFlag)
	{
		sendFlag = 0;
        
		if(flag300==1){flag300 = 0;dataProcess(VMS300,len300,head300);}
				if(flag301==1){flag301 = 0;dataProcess(VMS301,len301,head301);}
				if(flag401==1){flag401 = 0;dataProcess(VMS401,len401,head401);}
				if(flag302==1){flag302 = 0;dataProcess(MC302,len302,head302);}
				if(flag303==1){flag303 = 0;dataProcess(MC303,len303,head303);}
				if(flag304==1){flag304 = 0;dataProcess(MC304,len304,head304);}
				if(flag305==1){flag305 = 0;dataProcess(MC305,len305,head305);}
				if(flag306==1){flag306 = 0;dataProcess(MC306,len306,head306);}
				if(flag307==1){flag307 = 0;dataProcess(MC307,len307,head307);}
				if(flag308==1){flag308 = 0;dataProcess(MC308,len308,head308);}
				if(flag309==1){flag309 = 0;dataProcess(MC309,len309,head309);}
				if(flag310==1){flag310 = 0;dataProcess(BMS310,len310,head310);}
				if(flag311==1){flag311 = 0;dataProcess(BMS311,len311,head311);}
				if(flag412==1){flag412 = 0;dataProcess(BMS412,len412,head412);}
				if(flag320==1){flag320 = 0;dataProcess(CCF320,len320,head320);}
				if(flag321==1){flag321 = 0;dataProcess(CCF321,len321,head321);}
//				if(flag328==1){flag328 = 0;dataProcess(CCF328,len328,head328);}
				if(flag322==1){flag322 = 0;dataProcess(CCR322,len322,head322);}
				if(flag323==1){flag323 = 0;dataProcess(CCF323,len323,head323);}
				if(flag324==1){flag324 = 0;dataProcess(CCF324,len324,head324);}
				if(flag325==1){flag325 = 0;dataProcess(CCF325,len325,head325);}
				if(flag340==1){flag340 = 0;dataProcess(DTU340,len340,head340);}
				if(flag350==1){flag350 = 0;dataProcess(STWP350,len350,head350);}
		
		delay_ms(200);
			
	}
}



/*******************************************
               测试RTC
*******************************************/
void test_rtc()
{
	char time[20];
//	DS1302_DateSet(&SysDate);
	while(1)
	{
		DS1302_time(time);
		wirelessSend(time,20);
		delay_ms(1000);
	}
}



/*******************************************
               测试SD
*******************************************/
void test_sd()
{
//	u8 arraylist[8]={1,0,1,0,0,0,0,0};
	u8 arraylist[] = {0x00,0x60,0x00,0x05,0x50,0x00,0x05,0x00};
	char canchar[16];
	FRESULT res;
	int i,j;
	
    while(SD_Initialize()); //检测SD卡
	LEDSD = 0;

	f_mount(fs[0],"0:",0); 					//挂载SD卡 
	res = f_readdir(&dir,&fileinfo);
	if(res != FR_OK){
		res = f_mkdir("0:125");           //建立第一个文件夹123
	//  res = f_mkdir("123/sub2");      //在123下建立文件夹sub2
	//  res = f_mkdir("123/sub2/sub3"); //在123的sub2下建立文件夹sub3
	}
	else{
		res = f_opendir(&dir,"0:125");
	}
    res = f_open(file, "0:/125/read.txt", FA_CREATE_ALWAYS  | FA_WRITE); //在123/sub2/sub3下建立read.txt
	f_close(file);
	f_mount(0,"0:",0);// Unregister a work area before discard it
	
	//while(1)
	{
		f_mount(fs[0],"0:",0);
		res = f_opendir(&dir,"0:124");
		res = f_open(file, "0:/124/read.txt", FA_OPEN_EXISTING  | FA_WRITE);
		for(i=0;i<50;i++)
		{
			res = f_lseek(file, f_size(file)); //指针移到文件最后
			for(j=0;j<8;j++)
			{
				u8dec_TwoHex(arraylist[j],&canchar[j*2],&canchar[j*2+1]);
			}
			res = f_write(file, canchar, sizeof(canchar), &bw);
			res = f_write(file, "\r\n", 2, &bw);
		}
		f_close(file);
		f_mount(0,"0:",0);// Unregister a work area before discard it
	}
}


/*********************************************************************
                           SD + RTC
*********************************************************************/
void time2dirname(char time[],char dirname[])
{
	dirname[3]=time[5];//日月
	dirname[4]=time[6];
	dirname[6]=time[8];
	dirname[7]=time[9];
}
 void time2txtname(char time[],char txtname[])
 {
	txtname[3]=time[5];
	txtname[4]=time[6];
	txtname[6]=time[8];
	txtname[7]=time[9];
	txtname[9]=time[11]; //时分秒
	txtname[10]=time[12];
	txtname[12]=time[14];
	txtname[13]=time[15];
 }
void test_SDandRTC()
{
	u8 arraylist[] = {0x00,0x60,0x00,0x05,0x50,0x00,0x05,0x00};  //320
	FRESULT res;
	int i;
	char time[20];
	char dirname[]="18-03-17";
	char txtname[]="18-03-17/00-00.txt";
	
	while(SD_Initialize()); //检测SD卡

//	f_mount(fs[0],"0:",0); //挂载SD卡 
//	res = f_readdir(&dir,&fileinfo);
	//DS1302_time(time);//取消注释
//time2dirname(time,dirname);
//	if(res != FR_OK){
//		res = f_mkdir(dirname);
//	}
//	else{
//		res = f_opendir(&dir,dirname);
//	}
//	DS1302_time(time);
	//time2txtname(time,txtname);
	
	while(1)
	{
		f_mount(fs[0],"0:",0); //挂载SD卡 
		res = f_readdir(&dir,&fileinfo);
		if(res != FR_OK){
			res = f_mkdir(dirname);
		}
		else{
			res = f_opendir(&dir,dirname);
		}
//		LEDSD = 0;
		res = f_open(file, txtname, FA_CREATE_ALWAYS  | FA_WRITE);
		for(i=0;i<50;i++)
		{
			res = f_lseek(file, file->fptr); //指针移到文件最后
			DS1302_time(time);
//			sdwrite(time,"320",arraylist,8,file,bw);
			f_write(file, arraylist, 8, &bw);
			delay_ms(1000);
		}
		f_close(file);
		f_mount(0,"0:",0);// Unregister a work area before discard it
	}
	
}




