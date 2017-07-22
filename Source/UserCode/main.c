#include "bsp.h" 

uint8_t IsEnterIRQ = FALSE;

static uint8_t recvSlavedatbuf[32];   //接收数据缓冲区
static uint32_t ucrevcount; //记录接收到的数据个数
static uint8_t slavedataisReady = FALSE;


int main(void)
{
	bsp_Init();  //初始化硬件设备
    bsp_DelayMS(10);
    TaskInit(); //初始化任务,仅是获取任务数量，任务调度在bsp_idle中调用task_process实现    
	while (1)
	{
		if (GPIO_ReadInputDataBit(GPIOB, RF_IRQ_PIN)) //SPI的SX1278，接收到数据时，IRQ引脚为高
		{
			IsEnterIRQ = TRUE;
		} 

        if (IsEnterIRQ  == TRUE)
        {
            u8 length = 0;
            length = RFM96_LoRaRxPacket(recvSlavedatbuf);
            RFRxMode();
            if ((recvSlavedatbuf[0] == '&') && (recvSlavedatbuf[8] == '%')) //检测数据包头是否正确
            {
                slavedataisReady = TRUE;
            }
        }
        if(slavedataisReady == TRUE)
        {
            COMx_SendBuf(COM1,recvSlavedatbuf,12);
            memset(recvSlavedatbuf,0,12);
            slavedataisReady = FALSE;
            IsEnterIRQ = FALSE;            
        }
        
		bsp_Idle();    
	}
}
