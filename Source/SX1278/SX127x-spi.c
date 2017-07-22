/*********************************************************/
/*File Name:SX127x_spi.c                                 */
/*author:kennan                                          */
/*Date:2015.04.10                                        */
/*version:1.0                                            */
/*description:                                           */
/*  the SX127x spi control functions                     */
/*reversion record:                                      */
/*********************************************************/
#include "sx127X-spi.h"
/*
**************************************
SX127X_SPI_Init()
**************************************
*/
void SX127X_SPI_Init(void)
{
  SPI_InitTypeDef  SPI_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
  /* Enable the SPI periph */
  RCC_APB1PeriphClockCmd(SX127X_SPI_PERIPH_RCC , ENABLE);

  /* Enable SCK, MOSI and MISO GPIO clocks */
  RCC_AHBPeriphClockCmd(SX127X_SPI_PORT_RCC, ENABLE);
  
  GPIO_PinAFConfig(SX127X_SPI_PORT, SX127X_SCK_GPIO_PinSource , SX127X_SPI_GPIO_AF);
  GPIO_PinAFConfig(SX127X_SPI_PORT, SX127X_MISO_GPIO_PinSource, SX127X_SPI_GPIO_AF);
  GPIO_PinAFConfig(SX127X_SPI_PORT, SX127X_MOSI_GPIO_PinSource, SX127X_SPI_GPIO_AF);

  GPIO_InitStructure.GPIO_Pin = SX127X_SCK_PIN | SX127X_MISO_PIN | SX127X_MOSI_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_Init(SX127X_SPI_PORT, &GPIO_InitStructure);
  
  /* SPI configuration -------------------------------------------------------*/
	/* Set SPI interface */
	SPI_DeInit(SX127X_SPI);
	SPI_InitStructure.SPI_Direction=SPI_Direction_2Lines_FullDuplex;
	SPI_InitStructure.SPI_Mode=SPI_Mode_Master;
	SPI_InitStructure.SPI_DataSize=SPI_DataSize_8b;
	SPI_InitStructure.SPI_CPOL=SPI_CPOL_Low;
	SPI_InitStructure.SPI_CPHA=SPI_CPHA_1Edge;
	SPI_InitStructure.SPI_NSS=SPI_NSS_Soft;
	SPI_InitStructure.SPI_BaudRatePrescaler=SPI_BaudRatePrescaler_4;
	SPI_InitStructure.SPI_FirstBit=SPI_FirstBit_MSB;
	SPI_InitStructure.SPI_CRCPolynomial=7;

	SPI_Init(SX127X_SPI , &SPI_InitStructure);
	SPI_Cmd(SX127X_SPI,  ENABLE);					//Enable  SPI3
}
/*
**************************************
SPI_ExchangeByte.
**************************************
*/
uint8_t SX127X_SPI_ExchangeByte( uint8_t input )
{
    //wait for last transmitt finishing
   	while (SPI_I2S_GetFlagStatus(SX127X_SPI, SPI_I2S_FLAG_TXE) == RESET);
    SPI_I2S_SendData(SX127X_SPI, input);
    while (SPI_I2S_GetFlagStatus(SX127X_SPI, SPI_I2S_FLAG_RXNE) == RESET);
    return SPI_I2S_ReceiveData(SX127X_SPI);
}

