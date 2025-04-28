/*
 * Copyright (C), 2025, Yuanlong Xu <Yono233@outlook.com> 
 * All rights reserved
 */

/**
 * @file HiLPB_api.h
 *     HiLPB Component
 *     Application Interface (API)
 *     矩阵对象使用单例
 *     通信对象使用实例化和注册
 */

#ifndef _HILPB_API_H_
#define _HILPB_API_H_
#ifdef __cplusplus
extern "C"
{
#endif

/* Includes ------------------------------------------------------------------*/
#include <HiLPB_port.h>
/* Exported constants --------------------------------------------------------*/
/* Exported macros -----------------------------------------------------------*/

/* HILPB空参数定义 */
#define HILPB_PARA_NULL 0

/* API返回集 */
/* 通用返回 */
#define HILPB_API_RETURN_DEFAULT        0x00 //默认的无错误
#define HILPB_API_RETURN_ERR_INDEFINITE 0x01 //默认的未指定错误
#define HILPB_API_RETURN_ERR_PARAM      0x02 //传递参数错误

/* Exported types ------------------------------------------------------------*/

/**
 * @brief 读消息buffer定义 
 * 线性buffer
 */
typedef struct
{
    uint8_t  Buffer[HILPB_MATRIX_MAX_SIZE * 4 * HILPB_MATRIX_MAX_SIZE * 4]; // 大的数据buffer
    uint16_t Len;                                                           // 数据长度变量
} _HILPB_RX_EXIST;

/**
 * @brief HILPB驱动的运行时变量
 */
typedef struct
{
    uint32_t NoLink    :2;  // 无通信计数，用于断续帧检查
    uint32_t ParseState:4;  // 解析态
    uint32_t reg       :10; // 补位
    uint32_t CkSum     :16; // 计算校验和
} _HILPB_RUNTIME;

/**
 * @brief 解析栈
 */
typedef struct
{
    uint8_t txn; // 解析到的事务号

    union
    {
        uint16_t cmd; // 解析到的CMD
        struct
        {
            uint8_t cmd_h; // CMD高八位
            uint8_t cmd_l; // CMD低八位
        };
    };

    union
    {
        uint16_t ExMess; // 拓展信息
        struct
        {
            uint8_t ExMess_h; // 拓展信息高八位
            uint8_t ExMess_l; // 拓展信息低八位
        };
    };

    union
    {
        uint16_t LenMess; // 长度信息
        struct
        {
            uint8_t LenMess_h; // 长度信息高八位
            uint8_t LenMess_l; // 长度信息低八位
        };
    };

    union
    {
        uint16_t CkSum; // 校验和
        struct
        {
            uint8_t CkSum_h; // 校验和高八位
            uint8_t CkSum_l; // 校验和低八位
        };
    };
} _HILPB_PARSE_VALUE;

/**
 * @brief modbus通信发送接收函数的配置
 */
typedef struct
{
    HILPB_SEND_PTR Send; // 数据发送函数绑定
    HILPB_GTEC_PTR Getc; // 数据接收函数绑定
} _HILPB_FUNCTION;

/**
 * @brief 系统矩阵定义
 */
typedef struct _HILPB_SYS_MATRIX
{
    float   pp_R[HILPB_COMB_MAX_NUM][HILPB_MATRIX_MAX_SIZE][HILPB_MATRIX_MAX_SIZE]; // 二值R模型的综合阻抗参数矩阵
    float   pp_LC[HILPB_MATRIX_MAX_SIZE][HILPB_MATRIX_MAX_SIZE];                    // LC模型的综合阻抗参数矩阵
    float   YL[HILPB_MATRIX_MAX_SIZE];                                              // 支路感导纳
    float   YC[HILPB_MATRIX_MAX_SIZE];                                              // 支路容导纳
    float   YR[HILPB_MATRIX_MAX_SIZE];                                              // 支路阻导纳
    uint8_t attr[HILPB_MATRIX_MAX_SIZE];                                            // 支路属性

    float   J[HILPB_MATRIX_MAX_SIZE];
} _HILPB_SYS_MATRIX;

/**
 * @brief 定义HILPB通信对象 
 */
typedef struct _HILPB_COM
{
    /* 初始化时需赋固定值的部分 */
    _HILPB_RUNTIME Runtime; // 运行时变量
    /* 解析过程使用 保持对象独立性 */
    _HILPB_PARSE_VALUE Parse;   // 解析栈
    _HILPB_RX_EXIST    RxExist; // 消息空间
    /* 需传入初始化函数进行配置的部分 */
    _HILPB_FUNCTION Func; // 函数绑定
} _HILPB_COM;

/* Exported variables ---------------------------------------------------------*/
extern _HILPB_SYS_MATRIX HiLSysMatrix; // 单例的系统矩阵数据
/* Exported functions ---------------------------------------------------------*/

/***** 用户实用API *****/
HILPB_API void     HILPB_Tick(_HILPB_COM *pB);
HILPB_API uint32_t HILPB_Init(_HILPB_COM *pB, HILPB_SEND_PTR PBSend, HILPB_GTEC_PTR PBGetc);

#ifdef __cplusplus
}
#endif
#endif /* _HILPB_API_H_ */
