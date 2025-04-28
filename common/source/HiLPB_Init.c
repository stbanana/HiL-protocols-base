/********************************************************************************


 **** Copyright (C), 2025, Yuanlong Xu <Yono233@outlook.com>    ****
 **** All rights reserved                                       ****

 ********************************************************************************
 * File Name     : HiLPB_Init.c
 * Author        : yono
 * Date          : 2025-04-25
 * Version       : 1.0
********************************************************************************/
/**************************************************************************/
/*
    解析对象的初始化
*/

/* Includes ------------------------------------------------------------------*/
#include <HiLPB_api.h>
/* Private types -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private Constants ---------------------------------------------------------*/
/* Private macros ------------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
 * @brief 初始化解析对象
 * @param pB 待初始化的解析对象
 * @param PBSend 绑定的发送port函数
 * @param PBGetc 绑定的接收port函数
 * @return uint32_t 标准返回
 */
uint32_t HILPB_Init(_HILPB_COM *pB, HILPB_SEND_PTR PBSend, HILPB_GTEC_PTR PBGetc)
{
    uint32_t State = HILPB_API_RETURN_DEFAULT;
    /* 传参审查 */
    if(pB == HILPB_PARA_NULL || PBSend == HILPB_PARA_NULL || PBGetc == HILPB_PARA_NULL)
        return HILPB_API_RETURN_ERR_PARAM;

    /*  传参无关部分    */
    /* 初始化运行时 */
    pB->Runtime.NoLink     = 0;
    pB->Runtime.ParseState = 0;
    /* 初始化解析栈 */
    pB->Parse.CkSum   = 0;
    pB->Parse.cmd     = 0;
    pB->Parse.ExMess  = 0;
    pB->Parse.LenMess = 0;
    pB->Parse.CkSum   = 0;
    /* 初始化消息空间 */
    pB->RxExist.Len = 0;

    /*  传参有关部分    */
    /* 绑定回调函数部分 */
    pB->Func.Send = PBSend;
    pB->Func.Getc = PBGetc;

    return State;
}