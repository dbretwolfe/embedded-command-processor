#pragma once

#include "stdint.h"

#define CMD_PROC_ID_LEN (4)
#define CMD_PROC_MAX_NUM_CMDS (10)

typedef enum 
{
    CMD_PROC_SUCCESS_MSG = 1,
    CMD_PROC_SUCCESS = 0,
    CMD_PROC_INVALID_ARGS = -1,
    CMD_PROC_ARG_RANGE_ERR = -2,
    CMD_PROC_ERR_MSG = -3
} CmdProc_HandlerStatus;

typedef CmdProc_HandlerStatus (*CmdFunc_Handler)(uint16_t argCount, void* argValues, uint8_t* inputString, uint8_t* outputString);

typedef struct
{
    char cmdId[CMD_PROC_ID_LEN];
    CmdFunc_Handler cmdFunc;
}CmdProc_Command;

extern const CmdProc_Command commandList[];