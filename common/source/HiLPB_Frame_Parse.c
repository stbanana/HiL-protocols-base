/********************************************************************************


 **** Copyright (C), 2025, Yuanlong Xu <Yono233@outlook.com>    ****
 **** All rights reserved                                       ****

 ********************************************************************************
 * File Name     : HiLPB_Frame_Parse.c
 * Author        : yono
 * Date          : 2025-04-25
 * Version       : 1.0
********************************************************************************/
/**************************************************************************/
/*
    实际解析进行，对象编程混合函数式
*/

/* Includes ------------------------------------------------------------------*/
#include <HiLPB_api.h>
/* Private types -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private Constants ---------------------------------------------------------*/
/* Private macros ------------------------------------------------------------*/
#define _PARSE_STATE_TXN_GET   0  // 事务号获得
#define _PARSE_STATE_TXN_VF    1  // 事务号验证
#define _PARSE_STATE_CMD_H     2  // CMD获得高八位
#define _PARSE_STATE_CMD_L     3  // CMD获得低八位
#define _PARSE_STATE_EXMESS_H  4  // EXMESS获得高八位
#define _PARSE_STATE_EXMESS_L  5  // EXMESS获得低八位
#define _PARSE_STATE_LENMESS_H 6  // LENMESS获得高八位
#define _PARSE_STATE_LENMESS_L 7  // LENMESS获得低八位
#define _PARSE_STATE_MAINMESS  8  // 主要消息获得
#define _PARSE_STATE_CKSUM_L   9  // CKSUM获得低八位
#define _PARSE_STATE_CKSUM_H   10 // CKSUM获得高八位
/* Private function prototypes -----------------------------------------------*/
extern void HiLPB_Split_CMD(_HILPB_COM *pB);
/* Private functions ---------------------------------------------------------*/

/**
 * @brief 单个解析对象的实际解析进行
 * @param pB 待驱动解析对象 
 */
void HiLPB_Frame_Parse(_HILPB_COM *pB)
{
    uint8_t getc;
    while(pB->Func.Getc(&getc) == HILPB_API_RETURN_DEFAULT)
    {
        switch(pB->Runtime.ParseState)
        {
        case _PARSE_STATE_TXN_GET:
            pB->Parse.txn          = getc;
            pB->Runtime.CkSum      = getc; // 初始化校验和
            pB->Runtime.ParseState = _PARSE_STATE_TXN_VF;
            break;
        case _PARSE_STATE_TXN_VF:
            pB->Runtime.CkSum += getc; // 校验和计算
            if(pB->Parse.txn + getc != 0xFF)
            {
                // 产生错误
                pB->Runtime.ParseState = _PARSE_STATE_TXN_GET; // 回归状态机
            }
            else
            {
                pB->Runtime.ParseState = _PARSE_STATE_CMD_H;
            }
            break;
        case _PARSE_STATE_CMD_H:
            pB->Runtime.CkSum += getc; // 校验和计算
            pB->Parse.cmd_h        = getc;
            pB->Runtime.ParseState = _PARSE_STATE_CMD_L;
            break;
        case _PARSE_STATE_CMD_L:
            pB->Runtime.CkSum += getc; // 校验和计算
            pB->Parse.cmd_l        = getc;
            pB->Runtime.ParseState = _PARSE_STATE_EXMESS_H;
            break;
        case _PARSE_STATE_EXMESS_H:
            pB->Runtime.CkSum += getc; // 校验和计算
            pB->Parse.ExMess_h     = getc;
            pB->Runtime.ParseState = _PARSE_STATE_EXMESS_L;
            break;
        case _PARSE_STATE_EXMESS_L:
            pB->Runtime.CkSum += getc; // 校验和计算
            pB->Parse.ExMess_l     = getc;
            pB->Runtime.ParseState = _PARSE_STATE_LENMESS_H;
            break;
        case _PARSE_STATE_LENMESS_H:
            pB->Runtime.CkSum += getc; // 校验和计算
            pB->Parse.LenMess_h    = getc;
            pB->Runtime.ParseState = _PARSE_STATE_LENMESS_L;
            break;
        case _PARSE_STATE_LENMESS_L:
            pB->Runtime.CkSum += getc; // 校验和计算
            pB->Parse.LenMess_l    = getc;
            pB->Runtime.ParseState = _PARSE_STATE_MAINMESS;
            break;
        case _PARSE_STATE_MAINMESS:
            pB->Runtime.CkSum += getc; // 校验和计算
            if(pB->RxExist.Len < HILPB_MATRIX_MAX_SIZE)
            {
                pB->RxExist.Buffer[pB->RxExist.Len++] = getc;
            }
            else
            {
                // 产生错误
                pB->Runtime.ParseState = _PARSE_STATE_TXN_GET; // 回归状态机
            }

            if(pB->RxExist.Len >= (pB->Parse.LenMess - 2))
            {
                pB->Runtime.ParseState = _PARSE_STATE_CKSUM_L;
            }
            break;
        case _PARSE_STATE_CKSUM_L:
            pB->Parse.CkSum_l      = getc;
            pB->Runtime.ParseState = _PARSE_STATE_CKSUM_H;
            break;
        case _PARSE_STATE_CKSUM_H:
            pB->Parse.CkSum_h      = getc;

            pB->Runtime.ParseState = _PARSE_STATE_TXN_GET; // 回归状态机
            pB->Runtime.NoLink     = 0;                    // 清理无通信计数
            /* 进行校验和检查 */
            if(pB->Parse.CkSum == pB->Runtime.CkSum)
            {
                HiLPB_Split_CMD(pB); // 进行分流解析
            }
            else
            {
                // 产生错误
            }
            break;
        }
    }
}