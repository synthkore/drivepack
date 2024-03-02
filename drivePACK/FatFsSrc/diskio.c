/*-----------------------------------------------------------------------*/
/* Low level disk I/O module SKELETON for FatFs     (C)ChaN, 2019        */
/*-----------------------------------------------------------------------*/
/* If a working storage control module is available, it should be        */
/* attached to the FatFs via a glue function rather than modifying it.   */
/* This is an example of glue functions to attach various exsisting      */
/* storage control modules to the FatFs module with a defined API.       */
/*-----------------------------------------------------------------------*/

#include "ff.h"			/* Obtains integer types */
#include "diskio.h"		/* Declarations of disk functions */
#include "..\sdmmc.h"   /* JBR 2020-08-05 Reference to custom SD MMC card low level access functions*/

/* Definitions of physical drive number for each drive */
#define DEV_RAM		0	/* Example: Map Ramdisk to physical drive 0 */
#define DEV_MMC		1	/* Example: Map MMC/SD card to physical drive 1 */
#define DEV_USB		2	/* Example: Map USB MSD to physical drive 2 */
#define DEV_MMC	    0
// JBR 2020-07-06 These are the functions that encapsulate physical media access:
// IO Disk Controls:
//    disk_status - Get device status
//    disk_initialize - Initialize device
//    disk_read - Read data
//    disk_write - Write data
//    disk_ioctl - Control device dependent functions
// Real Time Clock
//    get_fattime - Get current time


/*-----------------------------------------------------------------------*/
/* Get Drive Status                                                      */
/*-----------------------------------------------------------------------*/

DSTATUS disk_status (
	BYTE pdrv		/* Physical drive nmuber to identify the drive */
)
{
	DSTATUS stat;
	int result;

	switch (pdrv) {
		// JBR 2020-08-05 Commented because not used
		// case DEV_RAM :
		//      result = RAM_disk_status();
		// translate the result code here
		//      return stat;

	case DEV_MMC :
			result = SDMMC_card_status(SDMMC_IDX_0);
			// translate the result code here:
			// begin JBR 2020-08-05 added translation from SDMMC_disk_initialize return type to DSTATUS
			if (result>=0){
				stat  = RES_OK;
			}else{
				stat  = RES_ERROR;
			}
			// end JBR 2020-08-05			
		return stat;

	    //JBR 2020-08-05 Commented because not used
		// case DEV_USB :
		//      result = USB_disk_status();
		// translate the result code here
		//      return stat;
	}

	return STA_NOINIT;
}



/*-----------------------------------------------------------------------*/
/* Inidialize a Drive                                                    */
/*-----------------------------------------------------------------------*/

DSTATUS disk_initialize (
	BYTE pdrv				/* Physical drive nmuber to identify the drive */
)
{
	DSTATUS stat;
	int result;

	switch (pdrv) {
		// JBR 2020-08-05 Commented because not used		
		// case DEV_RAM :
		//      result = RAM_disk_initialize();
		// translate the result code here
		//      return stat;

	case DEV_MMC :
		result =  SDMMC_card_initialize(SDMMC_IDX_0);		
		// translate the result code here:
		// begin JBR 2020-08-05 added translation from SDMMC_disk_initialize return type to DSTATUS
		if (result>0){
			stat  = RES_OK;
		}else{
			stat  = RES_ERROR;
		}
		// end JBR 2020-08-05 
		return stat;

		// JBR 2020-08-05 Commented because not used	
		// case DEV_USB :
		// 	   result = USB_disk_initialize();
		// translate the reslut code here
		//      return stat;
	}

	return STA_NOINIT;
}



/*-----------------------------------------------------------------------*/
/* Read Sector(s)                                                        */
/*-----------------------------------------------------------------------*/

DRESULT disk_read (
	BYTE pdrv,		/* Physical drive nmuber to identify the drive */
	BYTE *buff,		/* Data buffer to store read data */
	LBA_t sector,	/* Start sector in LBA */
	UINT count		/* Number of sectors to read */
)
{
	DRESULT res;
	int result;


	switch (pdrv) {
		// JBR 2020-08-05 Commented because not used			
		// case DEV_RAM :		
		// translate the arguments here
		//		result = RAM_disk_read(buff, sector, count);
		// translate the result code here
		//		return res;

	case DEV_MMC :
		// translate the arguments here
		// result = MMC_disk_read(buff, sector, count);
		// translate the result code here
		result =  SDMMC_card_read( SDMMC_IDX_0, (uint8_t*) buff, (uint64_t) sector, (uint16_t) count );

		// RES_OK = 0,		/* 0: Successful */
		// RES_ERROR,		/* 1: R/W Error */
		// RES_WRPRT,		/* 2: Write Protected */
		// RES_NOTRDY,		/* 3: Not Ready */
		// RES_PARERR		/* 4: Invalid Parameter */		

		// translate the result code here:
		// begin JBR 2020-08-24 added translation from SDMMC_disk_read return type to DSTATUS
		if (result>=0){
			res  = RES_OK;
		}else{
			res  = RES_ERROR;
		}
		// end JBR 2020-08-05
		return res;

		// JBR 2020-08-05 Commented because not used	
		// case DEV_USB :
		// translate the arguments here
		//		result = USB_disk_read(buff, sector, count);
		// translate the reslut code here
		//		return res;
	}

	return RES_PARERR;
}



/*-----------------------------------------------------------------------*/
/* Write Sector(s)                                                       */
/*-----------------------------------------------------------------------*/

#if FF_FS_READONLY == 0

DRESULT disk_write (
	BYTE pdrv,			/* Physical drive nmuber to identify the drive */
	const BYTE *buff,	/* Data to be written */
	LBA_t sector,		/* Start sector in LBA */
	UINT count			/* Number of sectors to write */
)
{
	DRESULT res;
	int result;

	switch (pdrv) {
		// JBR 2020-08-05 Commented because not used			
		// case DEV_RAM :
		// translate the arguments here
		//		result = RAM_disk_write(buff, sector, count);
		// translate the reslut code here
		//		return res;

	case DEV_MMC :
		// translate the arguments here
			result = SDMMC_card_write(SDMMC_IDX_0, (uint8_t*) buff, (uint64_t) sector, (uint16_t) count );

			// RES_OK = 0,		/* 0: Successful */
			// RES_ERROR,		/* 1: R/W Error */
			// RES_WRPRT,		/* 2: Write Protected */
			// RES_NOTRDY,		/* 3: Not Ready */
			// RES_PARERR		/* 4: Invalid Parameter */

		// translate the reslut code here
			// begin JBR 2021-04-26 added translation from SDMMC_disk_write return type to DSTATUS
			if (result>=0){
				res  = RES_OK;
			}else{
				res  = RES_ERROR;
			}
			// end JBR 2021-04-26
		return res;

		// JBR 2020-08-05 Commented because not used	
		// case DEV_USB :
		// translate the arguments here
		//		result = USB_disk_write(buff, sector, count);
		// translate the reslut code here
		//		return res;

	}

	return RES_PARERR;
}

#endif


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
	int result;

	switch (pdrv) {
		// JBR 2020-08-05 Commented because not used	
		// case DEV_RAM :
		// Process of the command for the RAM drive
		//		return res;

	case DEV_MMC :
		// Process of the command for the MMC/SD card
		return res;

		// JBR 2020-08-05 Commented because not used	
		// case DEV_USB :
		// Process of the command the USB drive
		//		return res;

	}

	return RES_PARERR;
}

