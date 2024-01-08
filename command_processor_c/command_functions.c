#include <assert.h>
#include <string.h>
#include <stdio.h>

#include "command_functions.h"
#include "cmd_proc_buffers.h"

static CmdProc_HandlerStatus TestFunc0(uint16_t argCount, float* argValues, uint8_t* inputString, uint8_t* handlerRespString);

const CmdProc_Command commandList[CMD_PROC_MAX_NUM_CMDS] = 
{ 
    { .cmdId = "TST0", .cmdFunc = TestFunc0 }
};

CmdProc_HandlerStatus TestFunc0(uint16_t argCount, float* argValues, uint8_t* inputString, uint8_t* handlerRespString)
{
    snprintf(
        handlerRespString, 
        CMD_PROC_HANDLER_RESP_STR_LEN,
        "argc = %hu, args = %f, %f, %f, %f, %f",
        argCount,
        argValues[0],
        argValues[1],
        argValues[2],
        argValues[3],
        argValues[4]
        );

    return CMD_PROC_SUCCESS_MSG;
}
