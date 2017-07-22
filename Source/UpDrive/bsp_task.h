#ifndef  __TASK__H__
#define  __TASK__H__
#include "bsp_tpc.h"
#pragma anon_unions             //使用匿名结构体

typedef union
{
    uint8_t msg[8];
    struct
    {
        uint8_t head;//包头
        uint8_t devID;//设备id
        uint8_t Heartdata;//心率值
        uint8_t HrtPowerdata;//心率带电量      
        uint8_t BatPowerdata;//电池电量
        uint8_t tail;//包尾      
    };
} SLVMSG_T; //从机信息结构体

typedef union
{
    uint8_t msg[8];
    struct
    {
        uint8_t N1STATUS;//节点1是否收到数据状态
        uint8_t N2STATUS;//节点2是否收到数据状态
        uint8_t N3STATUS;//节点3是否收到数据状态
        uint8_t N4STATUS;//节点4是否收到数据状态     
        uint8_t N5STATUS;//节点5是否收到数据状态
        uint8_t N6STATUS;//节点6是否收到数据状态
        uint8_t N7STATUS;//节点7是否收到数据状态
        uint8_t N8STATUS;//节点8是否收到数据状态    
    };
} M_STSTUS; //从机信息结构体
/********************************************************************************************************
* Variable definition
********************************************************************************************************/
extern TPC_TASK TaskComps[];

/********************************************************************************************************
* 内部函数
********************************************************************************************************/
static void Task_BroadCast(void);  // 广播任务
static void Task_LEDDisplay(void);  //LED闪烁任务
static void Task_SendToPC(void); //发送数据至PC机
static void Task_RecvfromPC(void);
static void Task_RecvfromSlave(void); //从Lora（SX1278）所连接的串口2读取数据任务
static void Task_SendToSlave(void);
/********************************************************************************************************
* 全局函数
********************************************************************************************************/
extern void TaskInit(void); // 初始化

/*******************************************************************************************************/
#endif

