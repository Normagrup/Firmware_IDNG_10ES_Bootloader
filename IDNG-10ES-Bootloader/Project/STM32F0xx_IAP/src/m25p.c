#include "stm32f0xx.h"
#include "spi2.h"



void m25pWriteEnable (void)
{
	spiIINCHIP_CSoff_m25p();                        // CS=0, SPI start
	spiIINCHIP_SpiSendData(0x06);										// WRITE ENABLE OP CODE
	spiIINCHIP_CSon_m25p(); 
}

void m25pWriteDisable (void)
{
	spiIINCHIP_CSoff_m25p();                        // CS=0, SPI start
	spiIINCHIP_SpiSendData(0x04);										// WRITE DISABLE OP CODE
	spiIINCHIP_CSon_m25p(); 
}

void m25pBulkErase (void)
{
	spiIINCHIP_CSoff_m25p();                        // CS=0, SPI start
	spiIINCHIP_SpiSendData(0xC7);										// BULK ERASE OP CODE
	spiIINCHIP_CSon_m25p(); 
}

void m25pSectorErase (unsigned char startAddrH, unsigned char startAddrM, unsigned char startAddrL)
{
	spiIINCHIP_CSoff_m25p();                        // CS=0, SPI start
	spiIINCHIP_SpiSendData(0xD8);										// SECTOR ERASE OP CODE
	spiIINCHIP_SpiSendData(startAddrH);
	spiIINCHIP_SpiSendData(startAddrM);
	spiIINCHIP_SpiSendData(startAddrL);
	spiIINCHIP_CSon_m25p(); 
}

void m25pWriteBytes (unsigned char startAddrH, unsigned char startAddrM, unsigned char startAddrL, unsigned char *buffer, unsigned int size)
{
	unsigned int n;
	spiIINCHIP_CSoff_m25p();                        // CS=0, SPI start
	spiIINCHIP_SpiSendData(0x02);										// PAGE PROGRAM OP CODE
	spiIINCHIP_SpiSendData(startAddrH);
	spiIINCHIP_SpiSendData(startAddrM);
	spiIINCHIP_SpiSendData(startAddrL);
	for (n=0;n<size;n++)
	{
		spiIINCHIP_SpiSendData(buffer[n]);			
	}
	spiIINCHIP_CSon_m25p(); 
}




void m25pReadBytes (unsigned char startAddrH, unsigned char startAddrM, unsigned char startAddrL, unsigned char *buffer, unsigned int size)
{
	unsigned int n;
	spiIINCHIP_CSoff_m25p();                        // CS=0, SPI start
	spiIINCHIP_SpiSendData(0x03);										// Read Byte Command
	spiIINCHIP_SpiSendData(startAddrH);
	spiIINCHIP_SpiSendData(startAddrM);
	spiIINCHIP_SpiSendData(startAddrL);
	for (n=0;n<size;n++)
	{	
		buffer[n]=spiIINCHIP_SpiRecvData();
	}
	spiIINCHIP_CSon_m25p(); 	
}

unsigned char m25pStatusRead (void)
{
	unsigned char data;
	spiIINCHIP_CSoff_m25p();                        // CS=0, SPI start
	spiIINCHIP_SpiSendData(0x05);										// Read Byte Command
	data = spiIINCHIP_SpiRecvData();
	spiIINCHIP_CSon_m25p(); 
	return data;
}
