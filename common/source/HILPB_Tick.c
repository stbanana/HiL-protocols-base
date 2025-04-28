/********************************************************************************


 **** Copyright (C), 2025, Yuanlong Xu <Yono233@outlook.com>    ****
 **** All rights reserved                                       ****

 ********************************************************************************
 * File Name     : HILPB_Tick.c
 * Author        : yono
 * Date          : 2025-04-25
 * Version       : 1.0
********************************************************************************/
/**************************************************************************/
/*
    周期调用的驱动时钟
*/

/* Includes ------------------------------------------------------------------*/
#include <HiLPB_api.h>
/* Private types -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
_HILPB_SYS_MATRIX HiLSysMatrix; // 单例的系统矩阵数据
/* Private Constants ---------------------------------------------------------*/
/* Private macros ------------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
extern void HiLPB_Frame_Parse(_HILPB_COM *pB);
/* Private functions ---------------------------------------------------------*/

/**
 * @brief 驱动单个解析对象的时钟
 * @param pB 待驱动解析对象 
 */
void HILPB_Tick(_HILPB_COM *pB)
{
    /* 未通信累计审查 */
    switch(pB->Runtime.NoLink)
    {
    case 0:
    case 1:
        pB->Runtime.NoLink++;
        break;
    case 2: // 阈值清理接收
        pB->Runtime.NoLink++;
        pB->RxExist.Len        = 0; // 清理消息空间
        pB->Runtime.ParseState = 0; // 回归帧解析初态
        break;
    case 3: // 静默累计
        break;
    default:
        pB->Runtime.NoLink = 0;
        break;
    }

    /* 进行解析 */
    HiLPB_Frame_Parse(pB);
}