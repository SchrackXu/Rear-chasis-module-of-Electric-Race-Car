#include "can.h"
#include "dataprocess.h"
#include "wireless.h"
#include "ff.h"
#include "string.h"

////将8位二进制数的后4位2进制位转换为1个16进制字符
//void u8dec_OneHex(u8 dec, char* out)
//{
//	char hex;
//	u8 tmp;
//	tmp = dec-dec/16*16;
//	if(tmp<10)	hex = (char)('0'+tmp);
//	else	hex = (char)('A'+tmp-10);
//	*out = hex;
//}

//将8位二进制数转换为2个16进制字符
void u8dec_TwoHex(u8 dec, char* outhigh, char* outlow)
{
	char hexh,hexl;
	u8 tmp;
	if((dec/16)<10)	hexh = (char)('0'+dec/16);
	else	hexh = (char)('A'+dec/16-10);
	tmp = dec-dec/16*16;
	if(tmp<10)	hexl = (char)('0'+tmp);
	else	hexl = (char)('A'+tmp-10);
	*outhigh = hexh;
	*outlow = hexl;
}

////将16位二进制数转换为4个16进制字符
//void u16dec_FourHex(u16 dec, char* out1, char* out2, char* out3, char* out4)
//{
//	u8 tmp1,tmp2;
//	tmp1 = (dec&0xFF00)>>8;
//	tmp2 = (dec&0xFF);
//	u8dec_TwoHex(tmp1, out1, out2);
//	u8dec_TwoHex(tmp2, out3, out4);
//}


//将3个字符串合并为1个字符串
void three2onestr(char *result,char str1[],int str1len,char str2,char str3[],int str3len)
{
	int i,j;
	for(i=0,j=0;j<str1len;i++,j++)result[i]=str1[j];
	for(j=0;j<1;i++,j++)result[i]=str2;
	for(j=0;j<str3len;i++,j++)result[i]=str3[j];
	result[i]=str2;
}


/////////////////////////////数据处理/////////////////////////////
void dataProcess(unsigned char data[],int len,char head)
{
	char dataSend[8];   //原来是8/11
	int i;
	dataSend[0] = 'y';
	dataSend[1] = head;
	for(i=0;i<len;i++)
	{
		dataSend[i+2] = data[i];
	}
	dataSend[len+2] = 'z';
	wirelessSend(dataSend,len+3);
}


//////////////////////////////////////////////////////////


void sdwrite(char time[],char flag[],unsigned char canmessage[],int length,FIL* file,UINT bw)
{
	char canchar[16];
	int tmp;
	char ctmp[24];
	
//	f_write(file, time, 20, &bw);
//	f_write(file, " ", 1, &bw);
//	f_write(file, flag, 1, &bw);
	three2onestr(ctmp,time,19,' ',flag,3);
	//wirelessSend(ctmp,24);
	f_write(file, ctmp, 24, &bw);
	for(tmp=0;tmp<length;tmp++)
	{
		u8dec_TwoHex(canmessage[tmp],&canchar[tmp*2],&canchar[tmp*2+1]);
	}
	f_write(file, canchar, length*2, &bw);
	f_write(file, "\r\n", 2, &bw);
}





