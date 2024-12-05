/**
  ******************************************************************************
  * @file    STM32F0xx_IAP/src/main.c 
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    29-May-2012
  * @brief   Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2012 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "w5100.h"
#include "socket.h"
#include "m25p.h"
#include "stm32f0xx.h"

/** @addtogroup STM32F0xx_IAP
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
uint32_t JumpAddress;
pFunction Jump_To_Application;
uint32_t FlashProtection = 0;
uint8_t buffer[4][256];	
uint32_t buff32[256];

volatile uint32_t reg;

/* Private function prototypes -----------------------------------------------*/
static void IAP_Init(void);

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program.
  * @param  None
  * @retval None
  */





void gotoApp (void)
{
				/* Test if user code is programmed starting from address "APPLICATION_ADDRESS" */
			if (((*(__IO uint32_t*)APPLICATION_ADDRESS) & 0x2FFE0000 ) == 0x20000000)
			{ 
				/* Jump to user application */
				JumpAddress = *(__IO uint32_t*) (APPLICATION_ADDRESS + 4);
				Jump_To_Application = (pFunction) JumpAddress;
				
				/* Initialize user application's Stack Pointer */
				__set_MSP(*(__IO uint32_t*) APPLICATION_ADDRESS);
				
				/* Jump to application */
				Jump_To_Application();
			}	
	
}




int main(void)
{
  SPI_InitTypeDef spi2con;
	GPIO_InitTypeDef spi2;
	GPIO_InitTypeDef spi2nssm;	
	GPIO_InitTypeDef bootPin;	
	
	/*!< At this stage the microcontroller clock setting is already configured, 
       this is done through SystemInit() function which is called from startup
       file (startup_stm32f0xx.s) before to branch to application main.
       To reconfigure the default setting of SystemInit() function, refer to
       system_stm32f0xx.c file
     */ 
  
  /* Initialize input */ 
		RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOC, ENABLE);	
		bootPin.GPIO_Mode = GPIO_Mode_IN;
		bootPin.GPIO_PuPd = GPIO_PuPd_NOPULL;
		bootPin.GPIO_Pin = GPIO_Pin_4;
		GPIO_Init(GPIOC,&bootPin); 		
	
		reg=RTC_ReadBackupRegister(RTC_BKP_DR0);
	
		if (reg==0xAAAAAAAA) 
		{
			// 	RCC_DeInit();		
			//	RCC_PLLConfig(RCC_PLLSource_HSI_Div2, RCC_PLLMul_16);
			//	RCC_PLLCmd(ENABLE);
			//	RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);
			RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2 , ENABLE);
			RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);
			
			spi2.GPIO_Mode = GPIO_Mode_AF;
			spi2.GPIO_Speed = GPIO_Speed_Level_3;
			spi2.GPIO_OType = GPIO_OType_PP;
			spi2.GPIO_PuPd = GPIO_PuPd_NOPULL;
			spi2.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
			GPIO_Init(GPIOB,&spi2);		
			spi2nssm.GPIO_Mode = GPIO_Mode_OUT;
			spi2nssm.GPIO_Speed = GPIO_Speed_Level_3;
			spi2nssm.GPIO_OType = GPIO_OType_PP;
			spi2nssm.GPIO_PuPd = GPIO_PuPd_NOPULL;
			spi2nssm.GPIO_Pin = GPIO_Pin_11;
			GPIO_Init(GPIOB,&spi2nssm);	
			GPIO_SetBits(GPIOB, GPIO_Pin_11);		
			SPI_I2S_DeInit(SPI2);
			spi2con.SPI_Direction = SPI_Direction_2Lines_FullDuplex; // Initially Tx
			spi2con.SPI_Mode	= SPI_Mode_Master;
			spi2con.SPI_DataSize	= SPI_DataSize_8b;
			spi2con.SPI_CPOL	= SPI_CPOL_Low ;
			spi2con.SPI_CPHA	= SPI_CPHA_1Edge;
			spi2con.SPI_NSS	= SPI_NSS_Soft ;		
			spi2con.SPI_BaudRatePrescaler	= SPI_BaudRatePrescaler_2;		
			spi2con.SPI_FirstBit	= SPI_FirstBit_MSB;
			spi2con.SPI_CRCPolynomial	= SPI_CRCLength_8b;
			SPI_Init(SPI2, &spi2con);
			SPI_RxFIFOThresholdConfig (SPI2, SPI_RxFIFOThreshold_QF);
			SPI_Cmd(SPI2, ENABLE); 	
			//bootPin.GPIO_Mode = GPIO_Mode_IN;
			//bootPin.GPIO_PuPd = GPIO_PuPd_NOPULL;
			//bootPin.GPIO_Pin = GPIO_Pin_4;
			//GPIO_Init(GPIOC,&bootPin); 
			
			IAP_Init();
		}
		else
		{ /* Keep the user application running */
			gotoApp();
		}
		
  /* Infinite loop */
  while (1)
  {		
  }
}








/**
  * @brief  Initialize the IAP.
  * @param  None
  * @retval None
  */

void storeBuff32 (void)
{
	unsigned int n;	
	
	volatile unsigned char f[256]={19,1,2,3,4,5,6,12,8,13,24,11,7,9,14,15,16,17,18,0,20,21,22,23,10,25,26,27,28,39,30,44,32,33,34,41,36,37,38,29,40,35,42,43,31,45,46,47,48,49,62,57,52,53,54,55,56,51,58,59,60,61,50,63,64,65,254,67,68,69,81,71,72,73,74,75,76,77,78,79,80,70,82,83,84,85,86,87,88,89,90,91,92,93,101,95,96,97,98,99,100,94,102,103,104,105,106,107,108,109,110,111,112,113,114,115,116,117,118,119,120,237,122,126,124,125,123,127,128,129,130,141,132,133,138,135,136,137,134,139,140,131,142,143,144,145,213,147,155,149,150,151,152,153,154,148,250,157,158,159,160,161,246,163,164,165,166,167,168,169,185,171,172,173,174,175,176,177,178,179,180,181,182,240,184,170,186,187,188,189,190,191,192,193,194,195,196,197,198,199,200,201,202,203,204,205,206,207,208,209,210,211,212,146,219,215,216,217,218,214,220,221,222,223,224,225,226,227,228,229,230,231,232,233,234,235,236,121,255,239,183,241,249,243,244,245,162,247,248,242,156,251,252,253,66,238};
	
	//unsigned char f[256];
	
	//for (n=0;n<256;n++)
	//{
	//	f[n]=n;
	//}
	
	
	for (n=0;n<64;n++)
	{	
		buff32[n]=buffer[0][f[(4*n)]] + ((buffer[0][f[((4*n)+1)]])<<8) + ((buffer[0][f[((4*n)+2)]])<<16) + ((buffer[0][f[((4*n)+3)]])<<24);
	}
	
	for (n=0;n<64;n++)
	{	
		buff32[n+64]=buffer[1][f[(4*n)]] + ((buffer[1][f[((4*n)+1)]])<<8) + ((buffer[1][f[((4*n)+2)]])<<16) + ((buffer[1][f[((4*n)+3)]])<<24);
	}
	
	for (n=0;n<64;n++)
	{	
		buff32[n+128]=buffer[2][f[(4*n)]] + ((buffer[2][f[((4*n)+1)]])<<8) + ((buffer[2][f[((4*n)+2)]])<<16) + ((buffer[2][f[((4*n)+3)]])<<24);
	}
	
	for (n=0;n<64;n++)
	{	
		buff32[n+192]=buffer[3][f[(4*n)]] + ((buffer[3][f[((4*n)+1)]])<<8) + ((buffer[3][f[((4*n)+2)]])<<16) + ((buffer[3][f[((4*n)+3)]])<<24);
	}
	

	
}


void IAP_Init(void)
{
	uint32_t flashdestination=APPLICATION_ADDRESS;
	unsigned char m25CntPage;
	unsigned int n;
	unsigned int j;
	
	/* Unlock the Flash Program Erase controller */
	FLASH_If_Init();		
	
	// INICIALIZACIÓN DEL SPI2	
	
	/* Test if any sector of Flash memory where user application will be loaded is write protected */
  if (FLASH_If_GetWriteProtectionStatus() != 0)  
  {
		FlashProtection = 1;
  }
  else
  {
    FlashProtection = 0;
  }
	
	if (FlashProtection == 1)
	{
		FLASH_OB_Launch();
	}
	else
	{
			FLASH_If_Erase(APPLICATION_ADDRESS);		
			m25CntPage=0;
			for (n=0;n<60;n++)
			{
				for (j=0;j<4;j++)
				{				
					m25pReadBytes(5,m25CntPage,0,&buffer[j][0],256);					
					m25CntPage++;
				}
				storeBuff32();			
				FLASH_If_Write(&flashdestination, buff32 ,256);
		
			}				
			

			gotoApp();
			
			

			
			
	}
	
	

	
	

}


#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif

/**
  * @}
  */


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
