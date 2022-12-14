/*-----------------------------------------------------------------------*/
/* Low level disk I/O module skeleton for FatFs     (C)ChaN, 2016        */
/*-----------------------------------------------------------------------*/
/* If a working storage control module is available, it should be        */
/* attached to the FatFs via a glue function rather than modifying it.   */
/* This is an example of glue functions to attach various exsisting      */
/* storage control modules to the FatFs module with a defined API.       */
/*-----------------------------------------------------------------------*/

#include "diskio.h"		/* FatFs lower layer API */
#include "mmc_sd.h"

///* Definitions of physical drive number for each drive */
//#define DEV_RAM		0	/* Example: Map Ramdisk to physical drive 0 */
//#define DEV_MMC		1	/* Example: Map MMC/SD card to physical drive 1 */
//#define DEV_USB		2	/* Example: Map USB MSD to physical drive 2 */
#define SD_CARD	 0  //SD??,????Ϊ0


/*-----------------------------------------------------------------------*/
/* Get Drive Status                                                      */
/*-----------------------------------------------------------------------*/

DSTATUS disk_status (
	BYTE pdrv		/* Physical drive nmuber to identify the drive */
)
{
	return 0;
}



/*-----------------------------------------------------------------------*/
/* Inidialize a Drive                                                    */
/*-----------------------------------------------------------------------*/

//DSTATUS disk_initialize (
//	BYTE pdrv				/* Physical drive nmuber to identify the drive */
//)
//{
//	DSTATUS stat;
//	int result;

//	switch (pdrv) {
//	case DEV_RAM :
//		result = RAM_disk_initialize();

//		// translate the reslut code here

//		return stat;

//	case DEV_MMC :
//		result = MMC_disk_initialize();

//		// translate the reslut code here

//		return stat;

//	case DEV_USB :
//		result = USB_disk_initialize();

//		// translate the reslut code here

//		return stat;
//	}
//	return STA_NOINIT;
//}
//??ʼ??????
DSTATUS disk_initialize (
	BYTE drv				/* Physical drive nmuber (0..) */
)
{	
	u8 res=0;	    
	switch(drv)
	{
		case SD_CARD://SD??
			res = SD_Initialize();//SD_Initialize() 
		 	if(res)//STM32 SPI??bug,??sd??????ʧ?ܵ?ʱ????????ִ????????????,???ܵ???SPI??д?쳣
			{
				SD_SPI_SpeedLow();
				SD_SPI_ReadWriteByte(0xff);//?ṩ??????8??ʱ??
				SD_SPI_SpeedHigh();
			}
  			break;
//		case EX_FLASH://?ⲿflash
//			SPI_Flash_Init();
//			if(SPI_FLASH_TYPE==W25Q64)FLASH_SECTOR_COUNT=2048*6;//W25Q64
//			else FLASH_SECTOR_COUNT=2048*2;						//????
// 			break;
		default:
			res=1; 
	}		 
	if(res)return  STA_NOINIT;
	else return 0; //??ʼ???ɹ?
}  



/*-----------------------------------------------------------------------*/
/* Read Sector(s)                                                        */
/*-----------------------------------------------------------------------*/

DRESULT disk_read (
	BYTE pdrv,		/* Physical drive nmuber to identify the drive */
	BYTE *buff,		/* Data buffer to store read data */
	DWORD sector,	/* Start sector in LBA */
	UINT count		/* Number of sectors to read */
)
{
	DRESULT res;

    if (!count)return RES_PARERR;//count???ܵ???0?????򷵻ز???????		 	 
	switch(pdrv)
	{
		case SD_CARD://SD??
			res=SD_ReadDisk(buff,sector,count);	 
		 	if(res)//STM32 SPI??bug,??sd??????ʧ?ܵ?ʱ????????ִ????????????,???ܵ???SPI??д?쳣
			{
				SD_SPI_SpeedLow();
				SD_SPI_ReadWriteByte(0xff);//?ṩ??????8??ʱ??
				SD_SPI_SpeedHigh();
			}
			break;
//		case EX_FLASH://?ⲿflash
//			for(;count>0;count--)
//			{
//				SPI_Flash_Read(buff,sector*FLASH_SECTOR_SIZE,FLASH_SECTOR_SIZE);
//				sector++;
//				buff+=FLASH_SECTOR_SIZE;
//			}
//			res=0;
//			break;
		default:
			res=1; 
	}
	//????????ֵ????SPI_SD_driver.c?ķ???ֵת??ff.c?ķ???ֵ
    if(res==0x00)return RES_OK;	 
    else return RES_ERROR;	   
}



/*-----------------------------------------------------------------------*/
/* Write Sector(s)                                                       */
/*-----------------------------------------------------------------------*/

DRESULT disk_write (
	BYTE pdrv,			/* Physical drive nmuber to identify the drive */
	const BYTE *buff,	/* Data to be written */
	DWORD sector,		/* Start sector in LBA */
	UINT count			/* Number of sectors to write */
)
{
	DRESULT res;

	if (!count)return RES_PARERR;//count???ܵ???0?????򷵻ز???????		 	 
	switch(pdrv)
	{
		case SD_CARD://SD??
			res=SD_WriteDisk((u8*)buff,sector,count);
			break;
//		case EX_FLASH://?ⲿflash
//			for(;count>0;count--)
//			{										    
//				SPI_Flash_Write((u8*)buff,sector*FLASH_SECTOR_SIZE,FLASH_SECTOR_SIZE);
//				sector++;
//				buff+=FLASH_SECTOR_SIZE;
//			}
//			res=0;
//			break;
		default:
			res=1; 
	}
    //????????ֵ????SPI_SD_driver.c?ķ???ֵת??ff.c?ķ???ֵ
    if(res == 0x00)return RES_OK;	 
    else return RES_ERROR;		 
}



/*-----------------------------------------------------------------------*/
/* Miscellaneous Functions                                               */
/*-----------------------------------------------------------------------*/

DRESULT disk_ioctl (
	BYTE pdrv,		/* Physical drive nmuber (0..) */
	BYTE cmd,		/* Control code */
	void *buff		/* Buffer to send/receive control data */
)
{
	DRESULT res;						  			     
	if(pdrv==SD_CARD)//SD??
	{
	    switch(cmd)
	    {
		    case CTRL_SYNC:
				SD_CS=0;
		        if(SD_WaitReady()==0)res = RES_OK; 
		        else res = RES_ERROR;	  
				SD_CS=1;
		        break;	 
		    case GET_SECTOR_SIZE:        //?????п?????????Ŀ
		        *(WORD*)buff = 512;
		        res = RES_OK;
		        break;	 
		    case GET_BLOCK_SIZE:         //??ÿ???????????ֽ?
		        *(WORD*)buff = 8;
		        res = RES_OK;
		        break;	 
		    case GET_SECTOR_COUNT:
		        *(DWORD*)buff = SD_GetSectorCount();
		        res = RES_OK;
		        break;
		    default:
		        res = RES_PARERR;
		        break;
	    }
	}else res=RES_ERROR;//?????Ĳ?֧??
    return res;
}

//????ʱ??
//User defined function to give a current time to fatfs module      */
//31-25: Year(0-127 org.1980), 24-21: Month(1-12), 20-16: Day(1-31) */                                                                                                                                                                                                                                          
//15-11: Hour(0-23), 10-5: Minute(0-59), 4-0: Second(0-29 *2) */                                                                                                                                                                                                                                                
DWORD get_fattime (void)
{				 
//	u32 ttime;
//	u32 date=0;
//    
//    RTC_Get();		//?õ???ǰʱ??
//    ttime=calendar.w_year-1980;	//?õ?ƫ?ƺ???????
// 	date|=ttime<<25;
//    ttime=calendar.w_month;		//?õ??·?
// 	date|=ttime<<21;
//	ttime=calendar.w_date;		//?õ?????
// 	date|=ttime<<16;
//	ttime=calendar.hour;		//?õ?ʱ??
// 	date|=ttime<<11;
//	ttime=calendar.min;			//?õ?????
// 	date|=ttime<<5;
// 	date|=calendar.min>>1;  	//?õ?????	   			    
//    return date;   
	return 0;
}

