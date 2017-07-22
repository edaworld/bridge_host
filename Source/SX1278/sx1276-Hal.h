#ifndef __SX1276_HAL_H__
#define __SX1276_HAL_H__
#include "stm32l1xx.h"
/*
*******************************************
*MSP430F2254 Control port define for the SX1278 
*******************************************
*/
//define the SX1278 control port.
/*****************************************/
//DIO port.
#define SX127X_DIO0_PIN         				GPIO_Pin_0  	//PB0
#define SX127X_DIO1_PIN         				GPIO_Pin_1		//PB1

#define SX127X_DIO_PORT_RCC							RCC_AHBPeriph_GPIOB
#define SX127X_DIO_PORT								  GPIOB
#define SX127X_DIO_EXTI_PortSoruce   		EXTI_PortSourceGPIOB

#define SX127X_DIO0_GPIO_PinSource			GPIO_PinSource0
#define SX127X_DIO0_EXTI_PinSource      EXTI_PinSource0
#define SX127X_DIO0_EXTI_Line						EXTI_Line0
#define SX127X_DIO0_NVIC_IRQ_Channel		EXTI0_IRQn
/*****************************************/
//SPI port
#define SX127X_SCK_PIN          				GPIO_Pin_13		//PB13
#define SX127X_MISO_PIN         				GPIO_Pin_14   //PB14
#define SX127X_MOSI_PIN         				GPIO_Pin_15   //PB15

#define SX127X_SCK_GPIO_PinSource      GPIO_PinSource13
#define SX127X_MISO_GPIO_PinSource     GPIO_PinSource14
#define SX127X_MOSI_GPIO_PinSource     GPIO_PinSource15

#define SX127X_SPI_GPIO_AF						 GPIO_AF_SPI2


#define SX127X_SPI_PORT_RCC						RCC_AHBPeriph_GPIOB
#define SX127X_SPI_PERIPH_RCC					RCC_APB1Periph_SPI2
#define SX127X_SPI_PORT								GPIOB
#define SX127X_SPI										SPI2
/*****************************************/
//NSS PORT
#define SX127X_nSEL_PIN         				GPIO_Pin_12    //PB12

#define SX127X_nSEL_PORT_RCC						RCC_AHBPeriph_GPIOB
#define SX127X_nSEL_PORT							  GPIOB

/*****************************************/
//reset port
#define SX127X_RST_PIN          				GPIO_Pin_2    //PB2

#define SX127X_RST_PORT_RCC							RCC_AHBPeriph_GPIOB
#define SX127X_RST_PORT							  	GPIOB
/*
*******************************************
*macros definitions.
*******************************************
*/
#define SX_CSN_LOW( )   (GPIO_ResetBits(SX127X_nSEL_PORT , SX127X_nSEL_PIN))
#define SX_CSN_HIGH( )  (GPIO_SetBits(SX127X_nSEL_PORT , SX127X_nSEL_PIN))

#define SX_RST_LOW()    (GPIO_ResetBits(SX127X_RST_PORT	 , SX127X_RST_PIN))
#define SX_RST_HIGH()   (GPIO_SetBits(SX127X_RST_PORT	 , SX127X_RST_PIN))

#define GET_DIO0()      (GPIO_ReadInputDataBit(SX127X_DIO_PORT , SX127X_DIO0_PIN))
#define GET_DIO1()      (GPIO_ReadInputDataBit(SX127X_DIO_PORT , SX127X_DIO1_PIN))
#define GET_DIO2()      (GPIO_ReadInputDataBit(SX127X_DIO_PORT , SX127X_DIO2_PIN))
#define GET_DIO3()      (GPIO_ReadInputDataBit(SX127X_DIO_PORT , SX127X_DIO3_PIN))
#define GET_DIO4()      (GPIO_ReadInputDataBit(SX127X_DIO_PORT , SX127X_DIO4_PIN))
#define GET_DIO5()      (GPIO_ReadInputDataBit(SX127X_DIO_PORT , SX127X_DIO5_PIN))
   
/*
************************************************
   we assume the DIO is used as:
   DIO0: 
       RXDONE   in RX mode      00b
       TXDONE   in TX mode,     01b
       CADDONE  in CAD mode     10b
   DIO1:
       RxTimeOut in RX mode     00b
   DIO2:
       FhssChangeChannel        00b~10b
   DIO3:
       ValidHeader              01b
   DIO4:
       default:CadDetected      00b
   DIO5:
       default: Mode Ready      00b
   
************************************************
*/   
#define GET_RXDONE()            GET_DIO0()
#define GET_TXDONE()            GET_DIO0()
#define GET_CADDONE()           GET_DIO0()

#define GET_RXTIMEOUT()         GET_DIO1()
#define GET_VALIDHEAD()         GET_DIO3()

typedef enum
{
    RADIO_RESET_OFF,
    RADIO_RESET_ON,
}tRadioResetState;

/*!
 * \brief Initializes the radio interface I/Os
 */
void SX1276PortInit(void);
/*!
 * \brief open the SX1278.
 */
void SX1276_Open(void);
/*!
 * \brief Set the radio reset pin state
 * 
 * \param state New reset pin state
 */
void SX1276SetReset( uint8_t state );

/*!
 * \brief Writes the radio register at the specified address
 *
 * \param [IN]: addr Register address
 * \param [IN]: data New register value
 */
void SX1276Write( uint8_t addr, uint8_t data );

/*!
 * \brief Reads the radio register at the specified address
 *
 * \param [IN]: addr Register address
 * \param [OUT]: data Register value
 */
void SX1276Read( uint8_t addr, uint8_t *data );

/*!
 * \brief Writes multiple radio registers starting at address
 *
 * \param [IN] addr   First Radio register address
 * \param [IN] buffer Buffer containing the new register's values
 * \param [IN] size   Number of registers to be written
 */
void SX1276WriteBuffer( uint8_t addr, uint8_t *buffer, uint8_t size );

/*!
 * \brief Reads multiple radio registers starting at address
 *
 * \param [IN] addr First Radio register address
 * \param [OUT] buffer Buffer where to copy the registers data
 * \param [IN] size Number of registers to be read
 */
void SX1276ReadBuffer( uint8_t addr, uint8_t *buffer, uint8_t size );

/*!
 * \brief Writes the buffer contents to the radio FIFO
 *
 * \param [IN] buffer Buffer containing data to be put on the FIFO.
 * \param [IN] size Number of bytes to be written to the FIFO
 */
void SX1276WriteFifo( uint8_t *buffer, uint8_t size );

/*!
 * \brief Reads the contents of the radio FIFO
 *
 * \param [OUT] buffer Buffer where to copy the FIFO read data.
 * \param [IN] size Number of bytes to be read from the FIFO
 */
void SX1276ReadFifo( uint8_t *buffer, uint8_t size );

#endif //__SX1276_HAL_H__
