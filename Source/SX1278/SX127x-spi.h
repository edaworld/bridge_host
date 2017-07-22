/*********************************************************/
/*File Name:SX127x_spi.h                                 */
/*author:kennan                                          */
/*Date:2015.4.10                                         */
/*version:1.0                                            */
/*description:                                           */
/*  the SX1278 SPI control functions and defines         */
/*reversion record:                                      */
/*********************************************************/
#ifndef __SX127X_SPI_H
#define __SX127X_SPI_H
#include "defines.h"
#include "sx1276-hal.h"
/*
*******************************************
*functions definitions.
*******************************************
*/
void    SX127X_SPI_Init(void);
uint8_t SX127X_SPI_ExchangeByte( uint8_t input );
#endif
