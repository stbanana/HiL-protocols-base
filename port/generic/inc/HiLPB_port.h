/*
 * Copyright (C), 2025, Yuanlong Xu <Yono233@outlook.com> 
 * All rights reserved
 */

/**
 * @file HiLPB_port.h
 *     HiLPB Component
 *     Port Specific
 * Generic
 */

#ifndef _H_UTILITY_H_
#define _H_UTILITY_H_
#ifdef __cplusplus
extern "C"
{
#endif

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>

#ifndef HILPB_MATRIX_MAX_SIZE
#define HILPB_MATRIX_MAX_SIZE 20 // 矩阵最大尺寸,最大支路数同
#endif

#ifndef HILPB_IGBT_MAX_NUM
#define HILPB_IGBT_MAX_NUM 20 // 最大开关管数量
#endif

#ifndef HILPB_DIODE_MAX_NUM
#define HILPB_DIODE_MAX_NUM 12 // 最大二级管数量
#endif

#ifndef HILPB_COMB_MAX_NUM
#define HILPB_COMB_MAX_NUM 60 // 最大组合数量
#endif

/* Exported constants --------------------------------------------------------*/
/* Exported macros -----------------------------------------------------------*/
#define HILPB_API extern

/* 临界段相关 */
// Embedded 和 GCC
#if((defined(__SES_ARM) || defined(__SES_RISCV) || defined(__CROSSWORKS_ARM) || defined(__GNUC__) || defined(__clang__)) && !defined(__CC_ARM) && !defined(WIN32))
#if(defined(__ARM_ARCH_6M__) || defined(__ARM_ARCH_8M_BASE__))
#define HiLPB_LOCK( )                            \
    {                                            \
        unsigned int _HiLPB__LockState;          \
        __asm volatile("mrs   %0, primask  \n\t" \
                       "movs  r1, #1       \n\t" \
                       "msr   primask, r1  \n\t" \
                       : "=r"(_HiLPB__LockState) \
                       :                         \
                       : "r1", "cc");

#define HiLPB_UNLOCK( )                                                     \
    __asm volatile("msr   primask, %0  \n\t" : : "r"(_HiLPB__LockState) :); \
    }
#elif(defined(__ARM_ARCH_7M__) || defined(__ARM_ARCH_7EM__) || defined(__ARM_ARCH_8M_MAIN__))
#ifndef SEGGER_RTT_MAX_INTERRUPT_PRIORITY
#define SEGGER_RTT_MAX_INTERRUPT_PRIORITY (0x20)
#endif
#define HiLPB_LOCK( )                                           \
    {                                                           \
        unsigned int _HiLPB__LockState;                         \
        __asm volatile("mrs   %0, basepri  \n\t"                \
                       "mov   r1, %1       \n\t"                \
                       "msr   basepri, r1  \n\t"                \
                       : "=r"(_HiLPB__LockState)                \
                       : "i"(SEGGER_RTT_MAX_INTERRUPT_PRIORITY) \
                       : "r1", "cc");

#define HiLPB_UNLOCK( )                                                     \
    __asm volatile("msr   basepri, %0  \n\t" : : "r"(_HiLPB__LockState) :); \
    }

#elif(defined(__ARM_ARCH_7A__) || defined(__ARM_ARCH_7R__))
#define HiLPB_LOCK( )                            \
    {                                            \
        unsigned int _HiLPB__LockState;          \
        __asm volatile("mrs r1, CPSR \n\t"       \
                       "mov %0, r1 \n\t"         \
                       "orr r1, r1, #0xC0 \n\t"  \
                       "msr CPSR_c, r1 \n\t"     \
                       : "=r"(_HiLPB__LockState) \
                       :                         \
                       : "r1", "cc");

#define HiLPB_UNLOCK( )                     \
    __asm volatile("mov r0, %0 \n\t"        \
                   "mrs r1, CPSR \n\t"      \
                   "bic r1, r1, #0xC0 \n\t" \
                   "and r0, r0, #0xC0 \n\t" \
                   "orr r1, r1, r0 \n\t"    \
                   "msr CPSR_c, r1 \n\t"    \
                   :                        \
                   : "r"(_HiLPB__LockState) \
                   : "r0", "r1", "cc");     \
    }
#elif defined(__riscv) || defined(__riscv_xlen)
#define HiLPB_LOCK( )                            \
    {                                            \
        unsigned int _HiLPB__LockState;          \
        __asm volatile("csrr  %0, mstatus  \n\t" \
                       "csrci mstatus, 8   \n\t" \
                       "andi  %0, %0,  8   \n\t" \
                       : "=r"(_HiLPB__LockState) \
                       :                         \
                       :);

#define HiLPB_UNLOCK( )                      \
    __asm volatile("csrr  a1, mstatus  \n\t" \
                   "or    %0, %0, a1   \n\t" \
                   "csrs  mstatus, %0  \n\t" \
                   :                         \
                   : "r"(_HiLPB__LockState)  \
                   : "a1");                  \
    }
#else
#define HiLPB_LOCK( )
#define HiLPB_UNLOCK( )
#endif
#endif

// windows
#if defined(WIN32)
void OS_SIM_EnterCriticalSection(void);
void OS_SIM_LeaveCriticalSection(void);

#define HiLPB_LOCK( ) \
    {                 \
        OS_SIM_EnterCriticalSection( );

#define HiLPB_UNLOCK( )             \
    OS_SIM_LeaveCriticalSection( ); \
    }
#endif

/* Exported types ------------------------------------------------------------*/

/* 定义发送port函数的类型 */
typedef uint32_t (*HILPB_SEND_PTR)(const void *Data, size_t Len);

/* 定义接收port函数的类型 */
typedef uint32_t (*HILPB_GTEC_PTR)(uint8_t *Data);

/* Exported variables ---------------------------------------------------------*/
/* Exported functions ---------------------------------------------------------*/

#ifdef __cplusplus
}
#endif
#endif /* _H_UTILITY_H_ */
