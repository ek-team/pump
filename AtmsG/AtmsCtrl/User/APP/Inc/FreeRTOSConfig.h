#ifndef FREERTOS_CONFIG_H
#define FREERTOS_CONFIG_H

//针对不同的编译器调用不同的stdint.h文件
#if defined(__ICCARM__) || defined(__CC_ARM) || defined(__GNUC__)
#include <stdint.h>
extern uint32_t SystemCoreClock; 
#endif

/*********************************************************************
		FreeRTOS 基础配置配置选项
*********************************************************************/
/* 
	置 1：RTOS 使用抢占式调度器；置 0：RTOS 使用协作式调度器（时间片）

	注：在多任务管理机制上，操作系统可以分为抢占式和协作式两种。
	协作式操作系统是任务主动释放 CPU 后，切换到下一个任务。
	任务切换的时机完全取决于正在运行的任务。
*/
#define configUSE_PREEMPTION  1

//1 使能时间片调度(默认式使能的)
#define configUSE_TIME_SLICING 1

/* 
		某些运行 FreeRTOS 的硬件有两种方法选择下一个要执行的任务：
		通用方法和特定于硬件的方法（以下简称“特殊方法”）。

		通用方法：
		1.configUSE_PORT_OPTIMISED_TASK_SELECTION 为 0 或者硬件不支持这种特殊方法。
		2.可以用于所有 FreeRTOS 支持的硬件
		3.完全用 C 实现，效率略低于特殊方法。
		4.不强制要求限制最大可用优先级数目

		特殊方法：
		1.必须将 configUSE_PORT_OPTIMISED_TASK_SELECTION 设置为 1。
		2.依赖一个或多个特定架构的汇编指令（一般是类似计算前导零[CLZ]指令）。
		3.比通用方法更高效
		4.一般强制限定最大可用优先级数目为 32

		一般是硬件计算前导零指令，如果所使用的，MCU 没有这些硬件指令的话此宏应该设置为 0！
*/
#define configUSE_PORT_OPTIMISED_TASK_SELECTION 1

//置 1：使能低功耗 tickless 模式；置 0：保持系统节拍（tick）中断一直运行
#define configUSE_TICKLESS_IDLE  0
/*
		写入实际的 CPU 内核时钟频率，也就是 CPU 指令执行频率，通常称为 Fclk
		Fclk 为供给 CPU 内核的时钟信号，我们所说的 cpu 主频为 XX MHz，
		就是指的这个时钟信号，相应的，1/Fclk 即为 cpu 时钟周期；
*/
#define configCPU_CLOCK_HZ  (SystemCoreClock)

//RTOS系统节拍中断的频率,即一秒中断的次数,每次中断 RTOS 都会进行任务调度
#define configTICK_RATE_HZ  (( TickType_t )1000)

//可使用的最大优先级
#define configMAX_PRIORITIES  (32)

//空闲任务使用的堆栈大小
#define configMINIMAL_STACK_SIZE  ((unsigned short)128)

//任务名字字符串长度
#define configMAX_TASK_NAME_LE (16)

//系统节拍计数器变量数据类型，1 表示为 16 位无符号整形, 0 表示为 32 位无符号整形
#define configUSE_16_BIT_TICKS 0

//空闲任务放弃CPU使用权给其他同优先级的用户任务
#define configIDLE_SHOULD_YIELD  1
 
//启用队列
#define configUSE_QUEUE_SETS  1

//开启任务通知功能,默认开启
#define configUSE_TASK_NOTIFICATIONS 1

//使用互斥信号量
#define configUSE_MUTEXES  1

//使用递归互斥信号量
#define configUSE_RECURSIVE_MUTEXES  1

//为1时使用计数信号量
#define configUSE_COUNTING_SEMAPHORES  1

//设置可以注册的信号量和消息队列个数 
#define configQUEUE_REGISTRY_SIZE  10

#define configUSE_APPLICATION_TASK_TAG 0 

/*****************************************************************
		FreeRTOS 与内存申请有关配置选项
*****************************************************************/
//支持动态内存申请
#define configSUPPORT_DYNAMIC_ALLOCATION 1
//支持静态内存
#define configSUPPORT_STATIC_ALLOCATION 0
//系统所有总的堆大小
#define configTOTAL_HEAP_SIZE ((size_t)(10*1024))
/***************************************************************
		FreeRTOS 与钩子函数有关的配置选项
**************************************************************/
/* 
置 1：使用空闲钩子（Idle Hook 类似于回调函数）；置 0：忽略空闲钩子

空闲任务钩子是一个函数，这个函数由用户来实现，
FreeRTOS 规定了函数的名字和参数：void vApplicationIdleHook(void )，
这个函数在每个空闲任务周期都会被调用
对于已经删除的 RTOS 任务，空闲任务可以释放分配给它们的堆栈内存。
因此必须保证空闲任务可以被 CPU 执行
使用空闲钩子函数设置 CPU 进入省电模式是很常见的
不可以调用会引起空闲任务阻塞的 API 函数
*/
#define configUSE_IDLE_HOOK  0

/* 
置 1：使用时间片钩子（Tick Hook）；置 0：忽略时间片钩子
时间片钩子是一个函数，这个函数由用户来实现，
FreeRTOS 规定了函数的名字和参数：void vApplicationTickHook(void )
时间片中断可以周期性的调用
函数必须非常短小，不能大量使用堆栈，
不能调用以”FromISR" 或 "FROM_ISR”结尾的 API 函数
*/
#define configUSE_TICK_HOOK  0
 
//使用内存申请失败钩子函数
#define configUSE_MALLOC_FAILED_HOOK  0

/*
大于 0 时启用堆栈溢出检测功能，如果使用此功能
用户必须提供一个栈溢出钩子函数，如果使用的话
此值可以为 1 或者 2，因为有两种栈溢出检测方法
*/
#define configCHECK_FOR_STACK_OVERFLOW 0

/********************************************************************
		FreeRTOS 与运行时间和任务状态收集有关的配置选项
**********************************************************************/
//启用运行时间统计功能
#define configGENERATE_RUN_TIME_STATS 0
//启用可视化跟踪调试
#define configUSE_TRACE_FACILITY  0
/* 
与宏 configUSE_TRACE_FACILITY 同时为 1 时会编译下面 3 个函数
		prvWriteNameToBuffer()
		vTaskList(),
		vTaskGetRunTimeStats()
*/
#define configUSE_STATS_FORMATTING_FUNCTIONS  1 

/********************************************************************
		FreeRTOS 与协程有关的配置选项
*********************************************************************/
//启用协程，启用协程以后必须添加文件 croutine.c
#define configUSE_CO_ROUTINES 0
//协程的有效优先级数目
#define configMAX_CO_ROUTINE_PRIORITIES ( 2 )

/***********************************************************************
		FreeRTOS 与软件定时器有关的配置选项
**********************************************************************/
//启用软件定时器
#define configUSE_TIMERS  1
//软件定时器优先级
#define configTIMER_TASK_PRIORITY  (configMAX_PRIORITIES-1)
//软件定时器队列长度
#define configTIMER_QUEUE_LENGTH  10
//软件定时器任务堆栈大小
#define configTIMER_TASK_STACK_DEPTH  (configMINIMAL_STACK_SIZE*2)

/************************************************************
		FreeRTOS 可选函数配置选项
************************************************************/
#define INCLUDE_xTaskGetSchedulerState	1
#define INCLUDE_vTaskPrioritySet				1
#define INCLUDE_uxTaskPriorityGet				1
#define INCLUDE_vTaskDelete 						1
#define INCLUDE_vTaskCleanUpResources 	1 
#define INCLUDE_vTaskSuspend 						1 
#define INCLUDE_vTaskDelayUntil  				1 
#define INCLUDE_vTaskDelay							1 
#define INCLUDE_eTaskGetState						1 
#define INCLUDE_xTimerPendFunctionCall	1 

/******************************************************************
		FreeRTOS 与中断有关的配置选项
******************************************************************/
#ifdef __NVIC_PRIO_BITS
#define configPRIO_BITS __NVIC_PRIO_BITS
#else
#define configPRIO_BITS 4
#endif
//中断最低优先级
#define configLIBRARY_LOWEST_INTERRUPT_PRIORITY  15

//系统可管理的最高中断优先级
#define configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY  5
#define configKERNEL_INTERRUPT_PRIORITY								( configLIBRARY_LOWEST_INTERRUPT_PRIORITY << (8 - configPRIO_BITS) )
#define configMAX_SYSCALL_INTERRUPT_PRIORITY 					( configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY << (8 - configPRIO_BITS) )
/****************************************************************
		FreeRTOS 与中断服务函数有关的配置选项
****************************************************************/
#define xPortPendSVHandler PendSV_Handler 
#define vPortSVCHandler SVC_Handler 

/* 以下为使用 Percepio Tracealyzer 需要的东西，不需要时将 configUSE_TRACE_FACILITY 定义为 0 */
#if ( configUSE_TRACE_FACILITY == 1 )
#include "trcRecorder.h" 
#define INCLUDE_xTaskGetCurrentTaskHandle 0
// 启用一个可选函数（该函数被 Trace 源码使用，默认该值为 0 表示不用）
#endif

#endif /* FREERTOS_CONFIG_H */
