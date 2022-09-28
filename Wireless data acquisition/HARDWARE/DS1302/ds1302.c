#include "ds1302.h"
#include "led.h"

DATE SysDate = {00, 37, 20, 16, 7, 4, 18};

void DAT_MODE_OUT(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; //��ͨ�������
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
}

void DAT_MODE_IN(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; //��������
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
}

/**
 * @brief DS1302���ų�ʼ��
 */
void DS1302_GPIO_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_AFIO, ENABLE);
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);  //ʹ�ܽ�ֹJTAG
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_3 | GPIO_Pin_4;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_4;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
}

/**
 * @brief ���ֽ�д
 * @param ��д������
 */
void DS1302_WriteOneByte(uint8_t data)
{
	uint8_t index = 0;
    DAT_MODE_OUT();
	SCK_CLR;
	for(index=0; index<8; index++)
	{
		SCK_CLR;
        DAT_MODE_OUT();
		delay_us(2);
		if (data & 0x01) /* ������д���� */
        {
			DAT_SET;
        }
		else
        { 
			DAT_CLR;
        }
//		delay_us(2);
        SCK_SET;
		data >>= 1;      /* �����λ��ʼд */
//		delay_us(2);
		
	}
}

/**
 * @brief ��ָ����ַд��ָ������
 * @param ָ����ַ
 * @param ָ������
 */
void DS1302_WriteByte(const uint8_t addr, const uint8_t data)
{
	RST_CLR;
	SCK_CLR;
//	delay_us(1);
	RST_SET;
//	delay_us(2);
	DS1302_WriteOneByte(addr);
	DS1302_WriteOneByte(data);
	RST_CLR;
	SCK_CLR;
//	delay_us(1);
}

/**
 * @brief ���ֽڶ�
 * @param �����ĵ�ַ
 */
uint8_t DS1302_ReadByte(const uint8_t addr)
{
	uint8_t index = 0, data = 0;
	RST_CLR;
	SCK_CLR;
	delay_us(1);
	RST_SET;
	delay_us(3);
	DS1302_WriteOneByte(addr);
    DAT_MODE_IN();
	for(index=0; index<8; index++)
	{
//		if (DAT_READ == 1) // �����ض�����
//        {
//			data |= 0x80;
//		}
//		SCK_SET;
//		data >>= 1;
//		delay_us(2);
//		SCK_CLR;
//		delay_us(2);
		
		
//		SCK_SET;
//		data >>=1;
//		SCK_CLR;
//		if(DAT_READ == 1)
//		{	data |=0x80;  }

		
//		if(DAT_READ == 1)
//		{	data |=0x80;  }
//		SCK_CLR;
//		data >>=1;
//		delay_us(3);
//		SCK_SET;


//		data >>=1;
//		if(DAT_READ == 1)
//		{	data |=0x80;  }
//		SCK_SET;
//		delay_us(3);
//		SCK_CLR;


		if(DAT_READ == 1)
		{	data |=0x80;  }
		SCK_SET;
		delay_us(3);
		SCK_CLR;
		data >>=1;

//		data >>=1;
//		SCK_CLR;
//		delay_us(2);
//		if(DAT_READ == 1)
//		{	data |=0x80;  }
//		SCK_SET;
//		delay_us(2);

		
	}
	delay_us(1);
	SCK_SET;
	RST_CLR;
	DAT_CLR;
	return data;
}

/**
 * @brief DS1302����
 * @param ����/ʱ��ṹ��ָ��
 */
int DS1302_DateSet(const DATE * const date)
{
	DS1302_WriteByte(WRITE_CONTROL_REG, 0x00); /* ȥ��д���� */
	DS1302_WriteByte(WRITE_SEC_REG,  DataToBcd(date->sec));
	DS1302_WriteByte(WRITE_MIN_REG,  DataToBcd(date->min));
	DS1302_WriteByte(WRITE_HOUR_REG, DataToBcd(date->hour));
	DS1302_WriteByte(WRITE_DAY_REG,  DataToBcd(date->day));
	DS1302_WriteByte(WRITE_MON_REG,  DataToBcd(date->mon));
	DS1302_WriteByte(WRITE_WEEK_REG, DataToBcd(date->week));
	DS1302_WriteByte(WRITE_YEAR_REG, DataToBcd(date->year));
    DS1302_WriteByte(WRITE_CONTROL_REG, 0x80); /* ����д���� */
	return 0;
}

/**
 * @brief DS1302��ȡ
 * @param ����/ʱ��ṹ��ָ��
 */
int DS1302_DateRead(DATE * const date)
{
	date->sec  = BcdToData(DS1302_ReadByte(READ_SEC_REG));
	date->min  = BcdToData(DS1302_ReadByte(READ_MIN_REG));
	date->hour = BcdToData(DS1302_ReadByte(READ_HOUR_REG));
	date->day  = BcdToData(DS1302_ReadByte(READ_DAY_REG));
	date->mon  = BcdToData(DS1302_ReadByte(READ_MON_REG));
	date->week = BcdToData(DS1302_ReadByte(READ_WEEK_REG));
	date->year = BcdToData(DS1302_ReadByte(READ_YEAR_REG)) + 2000;
	return 0;
}

/**
 * @brief DS1302��ʼ��
 * @param ����/ʱ��ṹ��ָ��
 * @note  ����ǳ����,���Կ�������������µ�Դ�Ե�ص������繦��,ֻ�����쳣����²�ʹ�õ�ع���
 * @note  ֱ������DS1302Ƭ��RAMʵ�� 1>����ǵ�һ���ϵ�,����������/ʱ�� 2>����,�Ͳ���Ҫ��������/ʱ��
 */
int DS1302_Init(const DATE * const date)
{
	DS1302_GPIO_Init();
	DS1302_WriteByte(WRITE_CONTROL_REG, 0x80); /* ����д���� */
    if (BcdToData(DS1302_ReadByte(READ_RAM_REG)) == 0x01) /* ������ǵ�һ���ϵ�,��ֱ���˳� */
	{
		return 0;
	}
    DS1302_WriteByte(WRITE_CONTROL_REG, 0x00); /* ȥ��д���� */
//  DS1302_WriteByte(WRITE_CHARGE_REG, 0xa9);  /* ʹ�ܵ�������繦��(һ��Ҫ�ǳ���زſ���ʹ�ô˹���!!!) */
    DS1302_WriteByte(WRITE_RAM_REG, DataToBcd(0x01));
    DS1302_WriteByte(WRITE_CONTROL_REG, 0x80); /* ����д���� */
	DS1302_DateSet(date);
	return 0;
}


int DS1302_time(char time[])
{
	DS1302_DateRead(&SysDate);
	time[0]='2';
	time[1]='0';
//	time[2]=SysDate.year/10+'0';
//	time[3]=SysDate.year%10+'0';
	time[2]='1';
	time[3]='8';
	time[4]='/';
	time[5]=SysDate.mon/10+'0';
	time[6]=SysDate.mon%10+'0';
	time[7]='/';
	time[8]=SysDate.day/10+'0';
	time[9]=SysDate.day%10+'0';
	time[10]=' ';
	time[11]=SysDate.hour/10+'0';
	time[12]=SysDate.hour%10+'0';
	time[13]=':';
	time[14]=SysDate.min/10+'0';
	time[15]=SysDate.min%10+'0';
	time[16]=':';
	time[17]=SysDate.sec/10+'0';
	time[18]=SysDate.sec%10+'0';
	time[19]='\n';
	
	return 0;

}

