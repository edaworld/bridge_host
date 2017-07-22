#ifndef __LORA_TYPES_H__
#define __LORA_TYPES_H__
#include <rtthread.h>
typedef rt_uint8_t	uint8_t;
typedef rt_uint16_t	uint16_t;
typedef rt_uint8_t	uint8;
typedef rt_uint16_t uint16;
typedef rt_uint32_t uint32;
typedef rt_uint8_t	bool;
#define true 1
#define false 0
/*!
 * SX1276 LoRa register 
 */
typedef struct 
{
    uint8_t RegFifo;                                // 0x00 
    // Common settings
    uint8_t RegOpMode;                              // 0x01 
    uint8_t RegRes02;                               // 0x02 
    uint8_t RegRes03;                               // 0x03 
    uint8_t RegBandSetting;                         // 0x04 
    uint8_t RegRes05;                               // 0x05 
    uint8_t RegFrfMsb;                              // 0x06 
    uint8_t RegFrfMid;                              // 0x07 
    uint8_t RegFrfLsb;                              // 0x08 
    // Tx settings
    uint8_t RegPaConfig;                            // 0x09 
    uint8_t RegPaRamp;                              // 0x0A 
    uint8_t RegOcp;                                 // 0x0B 
    // Rx settings
    uint8_t RegLna;                                 // 0x0C 
    // LoRa registers
    uint8_t RegFifoAddrPtr;                         // 0x0D 
    uint8_t RegFifoTxBaseAddr;                      // 0x0E 
    uint8_t RegFifoRxBaseAddr;                      // 0x0F 
    uint8_t RegFifoRxCurrentAddr;                   // 0x10 
    uint8_t RegIrqFlagsMask;                        // 0x11 
    uint8_t RegIrqFlags;                            // 0x12 
    uint8_t RegNbRxBytes;                           // 0x13 
    uint8_t RegRxHeaderCntValueMsb;                 // 0x14 
    uint8_t RegRxHeaderCntValueLsb;                 // 0x15 
    uint8_t RegRxPacketCntValueMsb;                 // 0x16 
    uint8_t RegRxPacketCntValueLsb;                 // 0x17 
    uint8_t RegModemStat;                           // 0x18 
    uint8_t RegPktSnrValue;                         // 0x19 
    uint8_t RegPktRssiValue;                        // 0x1A 
    uint8_t RegRssiValue;                           // 0x1B 
    uint8_t RegHopChannel;                          // 0x1C 
    uint8_t RegModemConfig1;                        // 0x1D 
    uint8_t RegModemConfig2;                        // 0x1E 
    uint8_t RegSymbTimeoutLsb;                      // 0x1F 
    uint8_t RegPreambleMsb;                         // 0x20 
    uint8_t RegPreambleLsb;                         // 0x21 
    uint8_t RegPayloadLength;                       // 0x22 
    uint8_t RegMaxPayloadLength;                    // 0x23 
    uint8_t RegHopPeriod;                           // 0x24 
    uint8_t RegFifoRxByteAddr;                      // 0x25
    uint8_t RegModemConfig3;                        // 0x26
    uint8_t RegTestReserved27;                      // 0x27
    uint8_t RegFeiMsb;                              // 0x28
    uint8_t RegFeiMib;                              // 0x29 
    uint8_t RegFeiLsb;                              // 0x2A
    uint8_t RegTestReserved2B[0x30 - 0x2B];         // 0x2B-0x30 
    uint8_t RegDetectOptimize;                      // 0x31    
    uint8_t RegTestReserved32;                      // 0x32   
    uint8_t RegInvertIQ;                            // 0x33 
    uint8_t RegTestReserved34[0x36 - 0x34];         // 0x34-0x36 
    uint8_t RegDetectionThreshold;                  // 0x37
    uint8_t RegTestReserved38[0x3F - 0x38];         // 0x38-0x3F
    // I/O settings                
    uint8_t RegDioMapping1;                         // 0x40 
    uint8_t RegDioMapping2;                         // 0x41 
    // Version
    uint8_t RegVersion;                             // 0x42
    // Test   
    uint8_t RegTestReserved43;                      // 0x43
    // Additional settings
    uint8_t RegPllHop;                              // 0x44 
    // Test
    uint8_t RegTestReserved45[0x4A - 0x45];         // 0x45-0x4A   
    // Additional settings    
    uint8_t RegTcxo;                                // 0x4B
    // Test    
    uint8_t RegTestReserved4C;                      // 0x4C  
    // Additional settings    
    uint8_t RegPaDac;                               // 0x4D 
    // Test    
    uint8_t RegTestReserved4E[0x5A - 0x4E];         // 0x4E-0x5A  
    // Additional settings
    uint8_t RegFormerTemp;                          // 0x5B
    // Test    
    uint8_t RegTestReserved5C;                      // 0x5C
    // Additional settings    
    uint8_t RegBitrateFrac;                         // 0x5D   
    // Additional settings    
    uint8_t RegTestReserved5E[0x60 - 0x5E];         // 0x5E-0x60
    // Additional settings
    uint8_t RegAgcRef;                              // 0x60
    uint8_t RegAgcThresh1;                          // 0x61
    uint8_t RegAgcThresh2;                          // 0x62
    uint8_t RegAgcThresh3;                          // 0x63
    // Test
    uint8_t RegTestReserved64[0x70 - 0x64];         // 0x64-0x70
}SX1276LoRaReg_t;
/*!
 * SX1276 LoRa General parameters definition
 */
typedef struct
{
    rt_uint32_t   RFFrequency;
    rt_int8_t     Power;
    rt_uint8_t    SignalBw;                   // LORA [0: 7.8 kHz, 1: 10.4 kHz, 2: 15.6 kHz, 3: 20.8 kHz, 4: 31.2 kHz,
                                        // 5: 41.6 kHz, 6: 62.5 kHz, 7: 125 kHz, 8: 250 kHz, 9: 500 kHz, other: Reserved]  
    uint8_t     	SpreadingFactor;            // LORA [6: 64, 7: 128, 8: 256, 9: 512, 10: 1024, 11: 2048, 12: 4096  chips]
    uint8_t     	ErrorCoding;                // LORA [1: 4/5, 2: 4/6, 3: 4/7, 4: 4/8]
    bool        	CrcOn;                         // [0: OFF, 1: ON]
    bool        	ImplicitHeaderOn;              // [0: OFF, 1: ON]
    bool        	RxSingleOn;                    // [0: Continuous, 1 Single]
    bool        	FreqHopOn;                     // [0: OFF, 1: ON]
    uint8_t     	HopPeriod;                  // Hops every frequency hopping period symbols
    rt_uint32_t   TxPacketTimeout;
    rt_uint32_t   RxPacketTimeout;
    uint8_t     	PayloadLength;
}LoRaGeneralSettings_t;

/*!
 * SX1276 Lora status.
 */
typedef enum
{
  STATUS_LORA_SLEEP,
  STATUS_LORA_STANDBY,
  STATUS_LORA_FSTX,
  STATUS_LORA_TX,
  STATUS_LORA_FSRX,
  STATUS_LORA_RXCONTINUOUS,
  STATUS_LORA_RXSINGLE,
  STATUS_LORA_CAD
}LoraStatus_t;

typedef enum
{
  LORA_DIO0 = 0,
  LORA_DIO1,
  LORA_DIO2,
  LORA_DIO3,
  LORA_DIO4,
  LORA_DIO5
}LoRaDio_t;
/*!
 * Radio driver structure defining the different function pointers.
 */
typedef struct
{
  void ( *Reset )        (void);
  void ( *Init  )        (void);
  void ( *ModeReq )      (uint8_t new_mode);
	void ( *IrqClear)      (uint8_t IrqSrc);
  void ( *TransPkt)      (uint8_t *pSrc , uint8_t size);
  bool ( *RecPkt)        (uint8_t *pDst , uint8_t *len);
}RadioDriver_t;
/*!
 * Radio object structure
 */
typedef struct
{
  rt_uint8_t    CurrentStatus;
  RadioDriver_t *pRadioDriver;
}RadioNodeObject_t;

//these two variable are declared in sx1276-lora.c
extern RadioNodeObject_t     LoRaRDO;                      
extern SX1276LoRaReg_t       *SX1276LR;
extern LoRaGeneralSettings_t LoRaSettings;
#endif
