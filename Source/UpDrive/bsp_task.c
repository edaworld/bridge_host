#include "bsp.h"

#define NULL 0

extern uint8_t g_uart1_timeout; //检测串口1接收数据超时的全局变量
//extern uint8_t g_uart2_timeout; //检测串口2接收数据超时的全局变量

extern UARTDATATYPE g_tUart1; //初始化从串口1
//extern UARTDATATYPE g_tUart2; //初始化从串口2

extern uint8_t TPCTaskNum; //任务数量，在bsp_task.c中被初始化，bsp_tpc.c中使用
extern uint8_t IsEnterIRQ;  //main.c的主函数中检测SPI接口的lora芯片是否进入中断标志位

//SLVMSG_T s_tSlaMsg[1] =
//{
//	{ '&', 1, NULL, NULL, NULL, '%' },
//}; //STM32发送从机数据的结构体,见bsp_uartpro.h
//M_STSTUS slavestatus = {
//	0,0,0,0,0,0,0,0
//};//从机状态结构体，表示是否从机有数据发送过来

/************************任务结构体说明*************************************/
/*
typedef struct _TPC_TASK
{
	uint8_t   attrb;  //静态任务：0，动态任务：1
	uint8_t   Run;  // 程序运行标记，0：不运行，1：运行
	uint16_t  Timer;  // 计时器
	uint16_t  ItvTime;  // 任务运行间隔时间
	void      (*Task)(void); // 要运行的任务函数

} TPC_TASK; // 任务定义
*/

TPC_TASK TaskComps[3] =
{
	//添加新任务时，请注意单个任务中改变任务属性的代码，否则将新添加的任务放到最后
	{ 0, 0, 1, 1000, Task_LEDDisplay }, //测试LED程序
    
//	{ 0, 0, 1, 1, Task_RecvfromSlave }, //从从机接收数据
//	{ 1, 0, 1, 1, Task_SendToPC }, //发送到PC上位机 
    
	{ 0, 0, 1, 5, Task_RecvfromPC }, //从主机接收数据     
	{ 1, 0, 1, 5, Task_SendToSlave }, //发送至下位机

};

/*********************************************************************************************************
*   函 数 名: Task_LEDDisplay
*   功能说明: LED闪烁代码
*********************************************************************************************************/
void Task_LEDDisplay(void)
{
    LED1_TOGGLE();
    return;
}

/*********************************************************************************************************
*   函 数 名: Task_RecvfromSlave
*   功能说明: 处理从SPI接口的Lora接收的数据任务
*********************************************************************************************************/
//static uint8_t recvSlavedatbuf[32];   //接收数据缓冲区
//static uint32_t ucrevcount; //记录接收到的数据个数
//static uint8_t slavedataisReady = FALSE;
//void Task_RecvfromSlave(void)
//{
//	if (IsEnterIRQ  == TRUE)
//	{
//		u8 length = 0;
////		printf("\t%d\n", bsp_GetRunTime()); //测试超时时间
//		length = RFM96_LoRaRxPacket(recvSlavedatbuf);
////		RFM96_LoRaRxPacket(recvSlavedatbuf);
//		RFRxMode();
////		printf("\t%d\n", bsp_GetRunTime()); //测试超时时间
//		if (length > 0)
//		{
//			ucrevcount++;
//		}
////		printf("receive data ucrevcount is %d\n", ucrevcount);
////		COMx_SendBuf(COM1, recvdatbuffer, length);
////		COMx_SendBuf(COM1, "\n", 2);
//		if ((recvSlavedatbuf[0] == '&') && (recvSlavedatbuf[8] == '%')) //检测数据包头是否正确
//		{
//            slavedataisReady = TRUE;
//            TaskComps[2].attrb = 0;
//		}
//		IsEnterIRQ = FALSE;
////        LED0 = !LED0;
//	}
//}

/*********************************************************************************************************
*   函 数 名: Task_SendToPC
*   功能说明: 将从机数据发送至PC机的任务
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
*   函 数 名: Task_RecvfromPC
*   功能说明: 
*********************************************************************************************************/
static uint8_t HostdataisReady = FALSE;
void Task_RecvfromPC(void)
{
//   超过3.5个字符时间后执行Uart1_RxTimeOut函数。全局变量 g_uart1_timeout = 1; 通知主程序开始解码
	if (g_uart1_timeout == 0)
	{
		return; // 没有超时，继续接收。不要清零 g_tUart1.RxCount
	}
	if (g_tUart1.RxCount < 12)    // 接收到的数据小于3个字节就认为错误
	{
		return;
	}
//    printf("%d",g_tUart1.RxCount); //测试接收数据个数是否正确
	g_uart1_timeout = 0; // 超时清标志
//    printf("\t%d\n",bsp_GetRunTime());//测试超时时间
	if ((g_tUart1.RxBuf[0] != '%') && (g_tUart1.RxBuf[8] != '&')) //检测数据包头是否正确
	{
		printf("error in head!");
	} 
    else //检测数据包是否都正确
	{
    //数据包接收正确
        HostdataisReady = TRUE;
        TaskComps[2].attrb = 0;
	} 
	g_tUart1.RxCount = 0; // 必须清零计数器，方便下次帧同步    
    return;
}

/*********************************************************************************************************
*   函 数 名: Task_BroadCast
*   功能说明: 发送广播信号的任务
*********************************************************************************************************/
void Task_SendToSlave(void)
{
//	sprintf(brdcstBuf, "BroadT1:%d", bsp_GetRunTime());
//    printf("\t%d\n", bsp_GetRunTime()); //测试超时时间
    if(HostdataisReady == TRUE)
    {    
        RFSendData(g_tUart1.RxBuf, 12);
        TaskComps[2].attrb = 1;
        HostdataisReady = FALSE;
    }
//	sprintf(brdcstBuf, "BroadT2:%d", bsp_GetRunTime());
//	printf("\t%d\n", bsp_GetRunTime()); //测试超时时间    
}

/*********************************************************************************************************
*   函 数 名: TaskInit
*   功能说明: 任务初始化
*********************************************************************************************************/
void TaskInit(void)
{
	TPCTaskNum = (sizeof(TaskComps) / sizeof(TaskComps[0])); // 获取任务数
}
