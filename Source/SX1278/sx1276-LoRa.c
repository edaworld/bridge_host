#include "radio.h"
#include "sx1276-Hal.h"
#include "sx1276-LoRaMisc.h"
#include "sx1276-LoRa.h"
#include "Sx127x-spi.h"
#include "string.h"             //for memcpy 
#include "defines.h"
#include "lora_types.h"

// Default settings
LoRaGeneralSettings_t LoRaSettings =
{
    434000000,        // RFFrequency
    20,               // Power
    7,                // SignalBw [0: 7.8kHz, 1: 10.4 kHz, 2: 15.6 kHz, 3: 20.8 kHz, 4: 31.2 kHz,
                      // 5: 41.6 kHz, 6: 62.5 kHz, 7: 125 kHz, 8: 250 kHz, 9: 500 kHz, other: Reserved]
    10,                // SpreadingFactor [6: 64, 7: 128, 8: 256, 9: 512, 10: 1024, 11: 2048, 12: 4096  chips]
    1,                // ErrorCoding [1: 4/5, 2: 4/6, 3: 4/7, 4: 4/8]
    true,             // CrcOn [0: OFF, 1: ON]
    false,            // ImplicitHeaderOn [0: OFF, 1: ON]
    0,                // RxSingleOn [0: Continuous, 1 Single]
    0,                // FreqHopOn [0: OFF, 1: ON]
    4,                // HopPeriod Hops every frequency hopping period symbols
    200,              // TxPacketTimeout
    200,              // RxPacketTimeout
    20,               // PayloadLength (used for implicit header mode)
};

/*!
 * SX1276 LoRa registers variable
 */
SX1276LoRaReg_t       *SX1276LR;
uint8_t               SX1276Regs[0x70];
extern                RadioNodeObject_t RadioNodeObject;

/*!
 * Rx management support variables
 */


/******************************************************/
//just a locall software delay function.
static void SX1276_LoraDelayMillisecond(uint8 time)
{
  uint16_t i,j;
	#define SYSTEM_CLK	72
  for(i = 0; i < time; i++)
  {
    for(j = 0 ;j < 60000; j++)
    {
     ;
    }
  }  
}
/*
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
+ only these functions will be called by other files.
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
*/
// reset the SX1276 chip.
void SX1276LoRaReset( void )
{
    SX1276SetReset( RADIO_RESET_ON );
    SX1276_LoraDelayMillisecond(10);
    // Wait 10ms

    SX1276SetReset( RADIO_RESET_OFF );
    SX1276_LoraDelayMillisecond(100);
    // Wait 100ms

}
/******************************************************/
// init the SX1276 in LoRa mode.
void SX1276LoRaInit( void )
{
    SX1276LR =   (SX1276LoRaReg_t *)SX1276Regs;

    SX1276LoRaSetDefaults( );
    
   // set the RF settings 
    SX1276LoRaSetLoRaOn();
    SX1276LoRaSetRFFrequency( LoRaSettings.RFFrequency );
    SX1276LoRaSetSpreadingFactor( LoRaSettings.SpreadingFactor ); // SF6 only operates in implicit header mode.
    SX1276LoRaSetErrorCoding( LoRaSettings.ErrorCoding );
    SX1276LoRaSetPacketCrcOn( LoRaSettings.CrcOn );
    SX1276LoRaSetSignalBandwidth( LoRaSettings.SignalBw );

    SX1276LoRaSetImplicitHeaderOn( LoRaSettings.ImplicitHeaderOn );
    SX1276LoRaSetSymbTimeout( 0x3FF );
    SX1276LoRaSetPayloadLength( LoRaSettings.PayloadLength );
    SX1276LoRaSetLowDatarateOptimize( true );
    
    SX1276LoRaSetPAOutput( RFLR_PACONFIG_PASELECT_PABOOST );
    SX1276LoRaSetPa20dBm( true );
    LoRaSettings.Power = 20;
    SX1276LoRaSetRFPower( LoRaSettings.Power );
    //we only use the RXDONE, TXDONE and CADDONE, all is on the DIO0 Pin.
    SX1276LR->RegIrqFlagsMask = RFLR_IRQFLAGS_RXTIMEOUT_MASK |
                                    //RFLR_IRQFLAGS_RXDONE_MASK |
                                    RFLR_IRQFLAGS_PAYLOADCRCERROR_MASK |
                                    RFLR_IRQFLAGS_VALIDHEADER_MASK |
                                    //RFLR_IRQFLAGS_TXDONE_MASK |
                                    // RFLR_IRQFLAGS_CADDONE_MASK |
                                    RFLR_IRQFLAGS_FHSSCHANGEDCHANNEL_MASK |
                                    RFLR_IRQFLAGS_CADDETECTED_MASK;
		SX1276Write( REG_LR_IRQFLAGSMASK, SX1276LR->RegIrqFlagsMask );
	 //clear all the possible pending interrupt first.
    SX1276LR->RegIrqFlags = 0xFF;
    SX1276Write( REG_LR_IRQFLAGS, SX1276LR->RegIrqFlagsMask ); 
    //after init the SX1276, sleep , for saving power.
    //SX1276LoRaSetOpMode( RFLR_OPMODE_SLEEP );
}
/******************************************************/
void SX1276LoRaModeReq(uint8_t new_mode)
{
    //the SX1276 only have six mode, from 0 to 5.
    if(new_mode > 5)
    {
      return;
    }
    else
    {
      switch(new_mode)
        {
          //if TX will happend, set the DIO0 as TXDONE interrupt
          case RFLR_OPMODE_SYNTHESIZER_TX:
          case RFLR_OPMODE_TRANSMITTER:
               SX1276LoRaDioConfig(LORA_DIO0 , DIO0_TXDONE);
          break;
          //if RX will happend set the DIO0 as RXDONE interrupt.
          case RFLR_OPMODE_SYNTHESIZER_RX:
          case RFLR_OPMODE_RECEIVER:
          case RFLR_OPMODE_RECEIVER_SINGLE:
             SX1276LoRaDioConfig(LORA_DIO0 , DIO0_RXDONE);
          break;
          //if the CAD will happend, set the DIO0 as CADDONE;
          case RFLR_OPMODE_CAD:
            SX1276LoRaDioConfig(LORA_DIO0 , DIO0_CADDONE);
          break;
          default:break;
        }
    }
  
  SX1276LoRaSetOpMode(new_mode);
}
/******************************************************/
void SX1276LoRaIrqClear(uint8_t IrqSrc)
{
  //SX1276LR->RegIrqFlags |= IrqSrc;
  SX1276Write( REG_LR_IRQFLAGS, IrqSrc );
}
/******************************************************/
void SX1276LoRaTransPkt(uint8_t *pSrc , uint8_t size)
{
  uint8_t TransFifoAddr;  
  SX1276LoRaSetOpMode( RFLR_OPMODE_STANDBY );
  SX1276Read(REG_LR_FIFOTXBASEADDR , &TransFifoAddr);
  SX1276Write(REG_LR_FIFOADDRPTR , TransFifoAddr);
  SX1276WriteFifo(pSrc , size);
  SX1276LoRaSetPayloadLength(size);
  SX1276LoRaModeReq(RFLR_OPMODE_TRANSMITTER);
}
/******************************************************/
uint8_t SX1276LoRaRecPkt(uint8_t *pDst , uint8_t *len)
{
  uint8_t RecFifoAddr;
  uint8_t PktSize;
  uint8_t IrqFlag;
  SX1276LoRaSetOpMode( RFLR_OPMODE_STANDBY );
  SX1276Read(REG_LR_FIFORXCURRENTADDR , &RecFifoAddr);      //last packet addr
  SX1276Write(REG_LR_FIFOADDRPTR , RecFifoAddr);  
  SX1276Read(REG_LR_NBRXBYTES , &PktSize);
  *len = PktSize;
  SX1276Read(REG_LR_IRQFLAGS , &IrqFlag);
  //if the received data is crc error?
  SX1276ReadBuffer(0 , pDst , PktSize);
  
  if(IrqFlag & RFLR_IRQFLAGS_PAYLOADCRCERROR)
  {
    SX1276Write( REG_LR_IRQFLAGS, RFLR_IRQFLAGS_PAYLOADCRCERROR);  
    //SX1276LoRaSetOpMode( RFLR_OPMODE_SLEEP );
    return false;
  }
  else
    return true;
}

/*
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
+ functions from here will not be called outside sx1276-LoRa.c
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
*/
/******************************************************/
void SX1276LoRaDioConfig(LoRaDio_t dio , uint8_t mapping)
{
  SX1276ReadBuffer(REG_LR_DIOMAPPING1 , &SX1276LR->RegDioMapping1 , 2);
  
  if(dio == LORA_DIO0)
    SX1276LR->RegDioMapping1 = (SX1276LR->RegDioMapping1 & RFLR_DIOMAPPING1_DIO0_MASK ) + \
                                (mapping << 6);
  if(dio == LORA_DIO1)
    SX1276LR->RegDioMapping1 = (SX1276LR->RegDioMapping1 & RFLR_DIOMAPPING1_DIO1_MASK ) + \
                                (mapping << 4);
  if(dio == LORA_DIO2)
    SX1276LR->RegDioMapping1 = (SX1276LR->RegDioMapping1 & RFLR_DIOMAPPING1_DIO2_MASK ) + \
                                (mapping << 2);
  if(dio == LORA_DIO3)
    SX1276LR->RegDioMapping1 = (SX1276LR->RegDioMapping1 & RFLR_DIOMAPPING1_DIO3_MASK ) + \
                                mapping;
  if(dio == LORA_DIO4)
    SX1276LR->RegDioMapping2 = (SX1276LR->RegDioMapping2 & RFLR_DIOMAPPING2_DIO4_MASK ) + \
                                (mapping << 6);
  
  if(dio == LORA_DIO5)
    SX1276LR->RegDioMapping2 = (SX1276LR->RegDioMapping2 & RFLR_DIOMAPPING2_DIO5_MASK ) + \
                                (mapping << 4);

  SX1276WriteBuffer(REG_LR_DIOMAPPING1 , &SX1276LR->RegDioMapping1 ,2);
          
}
/******************************************************/
void SX1276LoRaSetDefaults( void )
{
    // REMARK: See SX1276 datasheet for modified default values.

    SX1276Read( REG_LR_VERSION, &SX1276LR->RegVersion );
}
/******************************************************/
void SX1276LoRaSetLoRaOn( void )
{
	   SX1276LoRaSetOpMode( RFLR_OPMODE_SLEEP );
     SX1276LR->RegOpMode = ( SX1276LR->RegOpMode & RFLR_OPMODE_LONGRANGEMODE_MASK ) | \
                              RFLR_OPMODE_LONGRANGEMODE_ON;
     SX1276Write( REG_LR_OPMODE, SX1276LR->RegOpMode );
	   SX1276ReadBuffer( REG_LR_OPMODE, SX1276Regs + 1, 0x70 - 1 );
     SX1276LoRaSetOpMode( RFLR_OPMODE_STANDBY );
}

/******************************************************/
void SX1276LoRaSetOpMode( uint8_t opMode )
{
    uint8_t opModePrev;
    opModePrev = SX1276LR->RegOpMode & ~RFLR_OPMODE_MASK;
    
    if( opMode != opModePrev )
      {
        SX1276LR->RegOpMode = ( SX1276LR->RegOpMode & RFLR_OPMODE_MASK ) | opMode;

        SX1276Write( REG_LR_OPMODE, SX1276LR->RegOpMode );   
     }
}
/******************************************************/
uint8_t SX1276LoRaGetOpMode( void )
{
    SX1276Read( REG_LR_OPMODE, &SX1276LR->RegOpMode );
    
    return SX1276LR->RegOpMode & ~RFLR_OPMODE_MASK;
}
