#include "bsp.h" 

uint8_t IsEnterIRQ = FALSE;

static uint8_t recvSlavedatbuf[32];   //�������ݻ�����
static uint32_t ucrevcount; //��¼���յ������ݸ���
static uint8_t slavedataisReady = FALSE;


int main(void)
{
	bsp_Init();  //��ʼ��Ӳ���豸
    bsp_DelayMS(10);
    TaskInit(); //��ʼ������,���ǻ�ȡ�������������������bsp_idle�е���task_processʵ��    
	while (1)
	{
		if (GPIO_ReadInputDataBit(GPIOB, RF_IRQ_PIN)) //SPI��SX1278�����յ�����ʱ��IRQ����Ϊ��
		{
			IsEnterIRQ = TRUE;
		} 

        if (IsEnterIRQ  == TRUE)
        {
            u8 length = 0;
            length = RFM96_LoRaRxPacket(recvSlavedatbuf);
            RFRxMode();
            if ((recvSlavedatbuf[0] == '&') && (recvSlavedatbuf[8] == '%')) //������ݰ�ͷ�Ƿ���ȷ
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
