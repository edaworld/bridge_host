#include "bsp.h"

#define NULL 0

extern uint8_t g_uart1_timeout; //��⴮��1�������ݳ�ʱ��ȫ�ֱ���
//extern uint8_t g_uart2_timeout; //��⴮��2�������ݳ�ʱ��ȫ�ֱ���

extern UARTDATATYPE g_tUart1; //��ʼ���Ӵ���1
//extern UARTDATATYPE g_tUart2; //��ʼ���Ӵ���2

extern uint8_t TPCTaskNum; //������������bsp_task.c�б���ʼ����bsp_tpc.c��ʹ��
extern uint8_t IsEnterIRQ;  //main.c���������м��SPI�ӿڵ�loraоƬ�Ƿ�����жϱ�־λ

//SLVMSG_T s_tSlaMsg[1] =
//{
//	{ '&', 1, NULL, NULL, NULL, '%' },
//}; //STM32���ʹӻ����ݵĽṹ��,��bsp_uartpro.h
//M_STSTUS slavestatus = {
//	0,0,0,0,0,0,0,0
//};//�ӻ�״̬�ṹ�壬��ʾ�Ƿ�ӻ������ݷ��͹���

/************************����ṹ��˵��*************************************/
/*
typedef struct _TPC_TASK
{
	uint8_t   attrb;  //��̬����0����̬����1
	uint8_t   Run;  // �������б�ǣ�0�������У�1������
	uint16_t  Timer;  // ��ʱ��
	uint16_t  ItvTime;  // �������м��ʱ��
	void      (*Task)(void); // Ҫ���е�������

} TPC_TASK; // ������
*/

TPC_TASK TaskComps[3] =
{
	//���������ʱ����ע�ⵥ�������иı��������ԵĴ��룬��������ӵ�����ŵ����
	{ 0, 0, 1, 1000, Task_LEDDisplay }, //����LED����
    
//	{ 0, 0, 1, 1, Task_RecvfromSlave }, //�Ӵӻ���������
//	{ 1, 0, 1, 1, Task_SendToPC }, //���͵�PC��λ�� 
    
	{ 0, 0, 1, 5, Task_RecvfromPC }, //��������������     
	{ 1, 0, 1, 5, Task_SendToSlave }, //��������λ��

};

/*********************************************************************************************************
*   �� �� ��: Task_LEDDisplay
*   ����˵��: LED��˸����
*********************************************************************************************************/
void Task_LEDDisplay(void)
{
    LED1_TOGGLE();
    return;
}

/*********************************************************************************************************
*   �� �� ��: Task_RecvfromSlave
*   ����˵��: �����SPI�ӿڵ�Lora���յ���������
*********************************************************************************************************/
//static uint8_t recvSlavedatbuf[32];   //�������ݻ�����
//static uint32_t ucrevcount; //��¼���յ������ݸ���
//static uint8_t slavedataisReady = FALSE;
//void Task_RecvfromSlave(void)
//{
//	if (IsEnterIRQ  == TRUE)
//	{
//		u8 length = 0;
////		printf("\t%d\n", bsp_GetRunTime()); //���Գ�ʱʱ��
//		length = RFM96_LoRaRxPacket(recvSlavedatbuf);
////		RFM96_LoRaRxPacket(recvSlavedatbuf);
//		RFRxMode();
////		printf("\t%d\n", bsp_GetRunTime()); //���Գ�ʱʱ��
//		if (length > 0)
//		{
//			ucrevcount++;
//		}
////		printf("receive data ucrevcount is %d\n", ucrevcount);
////		COMx_SendBuf(COM1, recvdatbuffer, length);
////		COMx_SendBuf(COM1, "\n", 2);
//		if ((recvSlavedatbuf[0] == '&') && (recvSlavedatbuf[8] == '%')) //������ݰ�ͷ�Ƿ���ȷ
//		{
//            slavedataisReady = TRUE;
//            TaskComps[2].attrb = 0;
//		}
//		IsEnterIRQ = FALSE;
////        LED0 = !LED0;
//	}
//}

/*********************************************************************************************************
*   �� �� ��: Task_SendToPC
*   ����˵��: ���ӻ����ݷ�����PC��������
*********************************************************************************************************/
//void Task_SendToPC(void)
//{
//    if(slavedataisReady == TRUE)
//    {
//        COMx_SendBuf(COM1,recvSlavedatbuf,12);
//        memset(recvSlavedatbuf,0,12);
//        slavedataisReady = FALSE;
//        TaskComps[2].attrb = 1;
//    }
//}


/*********************************************************************************************************
*   �� �� ��: Task_RecvfromPC
*   ����˵��: 
*********************************************************************************************************/
static uint8_t HostdataisReady = FALSE;
void Task_RecvfromPC(void)
{
//   ����3.5���ַ�ʱ���ִ��Uart1_RxTimeOut������ȫ�ֱ��� g_uart1_timeout = 1; ֪ͨ������ʼ����
	if (g_uart1_timeout == 0)
	{
		return; // û�г�ʱ���������ա���Ҫ���� g_tUart1.RxCount
	}
	if (g_tUart1.RxCount < 12)    // ���յ�������С��3���ֽھ���Ϊ����
	{
		return;
	}
//    printf("%d",g_tUart1.RxCount); //���Խ������ݸ����Ƿ���ȷ
	g_uart1_timeout = 0; // ��ʱ���־
//    printf("\t%d\n",bsp_GetRunTime());//���Գ�ʱʱ��
	if ((g_tUart1.RxBuf[0] != '%') && (g_tUart1.RxBuf[8] != '&')) //������ݰ�ͷ�Ƿ���ȷ
	{
		printf("error in head!");
	} 
    else //������ݰ��Ƿ���ȷ
	{
    //���ݰ�������ȷ
        HostdataisReady = TRUE;
        TaskComps[2].attrb = 0;
	} 
	g_tUart1.RxCount = 0; // ��������������������´�֡ͬ��    
    return;
}

/*********************************************************************************************************
*   �� �� ��: Task_BroadCast
*   ����˵��: ���͹㲥�źŵ�����
*********************************************************************************************************/
void Task_SendToSlave(void)
{
//	sprintf(brdcstBuf, "BroadT1:%d", bsp_GetRunTime());
//    printf("\t%d\n", bsp_GetRunTime()); //���Գ�ʱʱ��
    if(HostdataisReady == TRUE)
    {    
        RFSendData(g_tUart1.RxBuf, 12);
        TaskComps[2].attrb = 1;
        HostdataisReady = FALSE;
    }
//	sprintf(brdcstBuf, "BroadT2:%d", bsp_GetRunTime());
//	printf("\t%d\n", bsp_GetRunTime()); //���Գ�ʱʱ��    
}

/*********************************************************************************************************
*   �� �� ��: TaskInit
*   ����˵��: �����ʼ��
*********************************************************************************************************/
void TaskInit(void)
{
	TPCTaskNum = (sizeof(TaskComps) / sizeof(TaskComps[0])); // ��ȡ������
}
