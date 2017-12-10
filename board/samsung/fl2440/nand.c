#include "nand.h"

extern unsigned long delay(void);

void NAND_Init()
{
	__IO GPACON |= 0x0007E000;

	__IO NFCONF |= (1 << 12) | (4 << 8) | (1 << 4);
	__IO NFCONT &=~ (1 << 12);
	__IO NFCONT |= (1 << 0);
	__IO NFSTAT &=~ 0xFFFFFFFF;
}

void NAND_Reset()
{
	NAND_CS_L();
	NAND_ENABLE_RB();
	__IO NFCMMD = NAND_CMD_RESET;
	NAND_WAIT_BUSY();
	NAND_CS_H();
}

void NAND_ReadPage(unsigned int block, unsigned int pageIndex, unsigned long *buffer)
{
	unsigned long page = (block << 6) + pageIndex;
	unsigned int i;
	NAND_Reset();
	NAND_CS_L();
	NAND_ENABLE_RB();
	__IO NFCMMD = NAND_CMD_READ1;
	__IO NFADDR = 0x00;
	__IO NFADDR = 0x00;
	__IO NFADDR = page & 0xFF;
	__IO NFADDR = (page >> 8) & 0xFF;
	__IO NFADDR = (page >> 16) & 0x01;
	__IO NFCMMD = NAND_CMD_READ2;
	NAND_WAIT_BUSY();
	for(i = 0; i  < PAGE_SIZE; i++)
	{
		*buffer++ = __IO NFDATA;
	}
	NAND_CS_H();
}

void NAND_MovingFromNANDToRAM(unsigned long currentAddress, unsigned long targetAddress, unsigned long size)
{
	unsigned char i = 0;
	unsigned int counterPage = size/2048 + 1;
	unsigned long *dst = targetAddress;
	NAND_Init();
	for(i = 0 ; i < counterPage; i++ )
	{
		NAND_ReadPage(0, i, dst);
		dst += 512;
	}
}
