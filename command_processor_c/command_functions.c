#include <assert.h>
#include <string.h>
#include <stdio.h>

#include "command_functions.h"
#include "cmd_proc_buffers.h"

static CmdProc_HandlerStatus TestFunc0(uint16_t argCount, void* argValues, uint8_t* inputString, uint8_t* handlerRespString);

const CmdProc_Command commandList[CMD_PROC_MAX_NUM_CMDS] = 
{ 
    { .cmdId = "TST0", .cmdFunc = TestFunc0 }
};

CmdProc_HandlerStatus TestFunc0(uint16_t argCount, void* argValues, uint8_t* inputString, uint8_t* handlerRespString)
{
    float* floatValues = (float*)argValues;
    snprintf(
        handlerRespString, 
        CMD_PROC_HANDLER_RESP_STR_LEN,
        "argc = %hu, args = %f, %f, %f, %f, %f",
        argCount,
        floatValues[0],
        floatValues[1],
        floatValues[2],
        floatValues[3],
        floatValues[4]
        );

    return CMD_PROC_SUCCESS_MSG;
}

CmdProc_HandlerStatus TestFunc1(uint16_t argCount, void* argValues, uint8_t* inputString, uint8_t* handlerRespString)
{

}
