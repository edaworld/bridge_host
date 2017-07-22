/********************************************************************************************************
*   模块名称 : BSP模块
*   文件名称 : bsp.h
*   说    明 : 这是底层驱动模块所有的h文件的汇总文件。 应用程序只需 #include bsp.h 即可，
*             不需要#include 每个模块的 h 文件
*
*   Copyright (C), 2013-2014, 安富莱电子 www.armfly.com
*********************************************************************************************************/
#ifndef _BSP_H_
#define _BSP_H

/* 开关全局中断的宏 */
#define ENABLE_INT()    __set_PRIMASK(0)    /* 使能全局中断 */
#define DISABLE_INT()   __set_PRIMASK(1)    /* 禁止全局中断 */

#include "stm32f10x.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdarg.h>

/* 通过取消注释或者添加注释的方式控制是否包含底层驱动模块 */

#include "bsp_led.h"
//#include "bsp_spi1.h"
#include "bsp_spi2.h"
#include "bsp_tpc.h"
#include "bsp_task.h"
#include "bsp_uartpro.h"
#include "bsp_uart_fifo.h"
#include "bsp_timer.h"
#include "bsp_oledio.h"
#include "bsp_userlib.h"
#include "bsp_sx1276-LoRa.h"
#include "bsp_rf.h"
#include "bsp_systimer.h"

#ifndef TRUE
#define TRUE  1
#endif

#ifndef FALSE
#define FALSE 0
#endif

/* 提供给其他C文件调用的函数 */
void bsp_Init(void);
void bsp_Idle(void);

#endif

