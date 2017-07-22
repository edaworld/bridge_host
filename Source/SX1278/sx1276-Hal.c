#include "Sx127x-spi.h"
#include "sx1276-Hal.h"
/*
=================================================================================
SX127X_PortInit( );
Function : Init the IO port for controlling the SX1278.
INTPUT   : none
OUTPUT   : NONE
=================================================================================
*/
void SX1276PortInit(void)
{
  
  GPIO_InitTypeDef  GPIO_InitStructure;
	//DIO port init.
	RCC_AHBPeriphClockCmd(SX127X_DIO_PORT_RCC , ENABLE);
	
	GPIO_InitStructure.GPIO_Pin   = SX127X_DIO0_PIN | SX127X_DIO1_PIN;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(SX127X_DIO_PORT, &GPIO_InitStructure);
	
	//SPI chip select port init.
	RCC_AHBPeriphClockCmd(SX127X_nSEL_PORT_RCC , ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = SX127X_nSEL_PIN ;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
 	GPIO_Init(SX127X_nSEL_PORT, &GPIO_InitStructure);
	
	SX_CSN_LOW( );	//deselect the SX1278
	//SX1278 reset port init.
	RCC_AHBPeriphClockCmd(SX127X_RST_PORT_RCC , ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = SX127X_RST_PIN ;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_Init(SX127X_RST_PORT, &GPIO_InitStructure);
	
	SX_RST_LOW(); //when power on ,reset the SX1278
	
}
/*
********************************************
Init the port for control SI446X
********************************************
*/
void SX1276_InterruptInit(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	EXTI_InitTypeDef  EXTI_InitStructure;
	

  /* Enable GPIOB clock */
  RCC_AHBPeriphClockCmd(SX127X_DIO_PORT_RCC , ENABLE);
  /* Configure PB0 pin as input floating */
	GPIO_InitStructure.GPIO_Pin  = SX127X_DIO0_PIN | SX127X_DIO1_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(SX127X_DIO_PORT, &GPIO_InitStructure);

  /* Enable SYSCFG clock */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
 /* Connect EXTI0 Line to PB0 pin */
  SYSCFG_EXTILineConfig(SX127X_DIO_EXTI_PortSoruce , SX127X_DIO0_EXTI_PinSource);

  /* Configure EXTI0 line */
  EXTI_InitStructure.EXTI_Line = SX127X_DIO0_EXTI_Line;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;  
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure);

		/* Enable the EXTI0 Interrupt */
	EXTI_ClearITPendingBit(SX127X_DIO0_EXTI_Line); //if pending..,clear interrupt first.
  
	NVIC_InitStructure.NVIC_IRQChannel = SX127X_DIO0_NVIC_IRQ_Channel;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0F;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x0F;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);	
}
/**********************************************/
void SX1276_Open(void)
{
	SX1276PortInit();
	SX127X_SPI_Init();
	SX1276_InterruptInit();
}
/**********************************************/
void SX1276SetReset( uint8_t state )
{
 if(state)
   SX_RST_LOW();
 else
  SX_RST_HIGH();
}

/**********************************************/
void SX1276WriteBuffer( uint8_t addr, uint8_t *buffer, uint8_t size )
{
    uint8_t i;
    //NSS = 0;
    SX_CSN_LOW( );
    SX127X_SPI_ExchangeByte( addr | 0x80 );
    for( i = 0; i < size; i++ )
    {
        SX127X_SPI_ExchangeByte( buffer[i] );
    }
    //NSS = 1;
    SX_CSN_HIGH( );
}
/**********************************************/
void SX1276ReadBuffer( uint8_t addr, uint8_t *buffer, uint8_t size )
{
    uint8_t i;

    //NSS = 0;
    SX_CSN_LOW( );
    SX127X_SPI_ExchangeByte( addr & 0x7F );

    for( i = 0; i < size; i++ )
    {
        buffer[i] = SX127X_SPI_ExchangeByte( 0 );
    }
    //NSS = 1;
    SX_CSN_HIGH( );
}
/**********************************************/
void SX1276Write( uint8_t addr, uint8_t data )
{
    SX1276WriteBuffer( addr, &data, 1 );
}
/**********************************************/
void SX1276Read( uint8_t addr, uint8_t *data )
{
    SX1276ReadBuffer( addr, data, 1 );
}
/**********************************************/
void SX1276WriteFifo( uint8_t *buffer, uint8_t size )
{
    SX1276WriteBuffer( 0, buffer, size );
}
/**********************************************/
void SX1276ReadFifo( uint8_t *buffer, uint8_t size )
{
    SX1276ReadBuffer( 0, buffer, size );
}



