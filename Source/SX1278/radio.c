#include "radio.h"
#include "sx1276-lora.h"

/*
***************************************************
assume a sensor node with SX1278 is a radio node object. 
***************************************************
*/
RadioNodeObject_t       LoRaRDO;        //LoRa Radio Device Object.
RadioDriver_t           LoRaRadioDriver;

RadioDriver_t *RadioDriverInit(void)
{
  LoRaRadioDriver.Reset         =  SX1276LoRaReset;
  LoRaRadioDriver.Init          =  SX1276LoRaInit;
  LoRaRadioDriver.ModeReq       =  SX1276LoRaModeReq;
	LoRaRadioDriver.IrqClear      =  SX1276LoRaIrqClear;
  LoRaRadioDriver.TransPkt      =  SX1276LoRaTransPkt;
  LoRaRadioDriver.RecPkt        =  SX1276LoRaRecPkt;
  
  return &LoRaRadioDriver;
}
