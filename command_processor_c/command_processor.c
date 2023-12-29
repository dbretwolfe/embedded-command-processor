#include <stdio.h>
#include <string.h>
#include <assert.h>

#include "command_processor.h"
#include "command_functions.h"

#define CMD_PROC_MAX_ARGS (5)

typedef enum { COMMAND_READY = 0, COMMAND_NOT_READY = -1 } AssembleCmdStatus;
typedef enum { COMMAND_VALID = 0, COMMAND_INVALID = -1 } ParseCmdStatus;
typedef enum { CMD_PROC_IDLE, CMD_PROC_ASSEMBLE } CmdProc_AssembleState;

extern const CmdProc_Command commandList[CMD_PROC_MAX_NUM_CMDS];

static AssembleCmdStatus CmdProc_AssembleCmd(uint8_t dataIn, uint8_t* inputString)
{
    static uint8_t commandBuffer[CMD_PROC_INPUT_STR_LEN] = {0};
    static int commandBufIndex = 0;
    static CmdProc_AssembleState cmdProcState = CMD_PROC_IDLE;

    if (cmdProcState == CMD_PROC_IDLE)
    {
        if (dataIn == CMD_PROC_START_CHAR)
        {
            cmdProcState = CMD_PROC_ASSEMBLE;
        }

        return COMMAND_NOT_READY;
    }
    else if (cmdProcState == CMD_PROC_ASSEMBLE)
    {
        if (dataIn == CMD_PROC_STOP_CHAR)
        {
            memcpy(inputString, commandBuffer, (commandBufIndex + 1));
            commandBufIndex = 0;
            cmdProcState = CMD_PROC_IDLE;
            return COMMAND_READY;
        }
        else
        {
            if (commandBufIndex == (CMD_PROC_INPUT_STR_LEN - 1))
            {
                commandBufIndex = 0;
                cmdProcState = CMD_PROC_IDLE;
            }
            else
            {
                commandBuffer[++commandBufIndex] = dataIn;
            }
            
            return COMMAND_NOT_READY;
        }
    }
    else
    {
        assert(0);
        cmdProcState = CMD_PROC_IDLE;
        return COMMAND_NOT_READY;
    }
}

static ParseCmdStatus CmdProc_ParseInputString(const uint8_t* inputString, const CmdProc_Command* commandOut, int* argCountOut, float* argValuesOut)
{
    for (int i = 0; i < CMD_PROC_MAX_NUM_CMDS; i++)
    {
        if (strncmp((const char*)inputString, commandList[i].cmdId, CMD_PROC_ID_LEN) == 0)
        {
            commandOut = &commandList[i];

            char scanString[30] = {0};
            sprintf(scanString, "\%%dC %f, %f, %f, %f, %f");

            char cmdString[5];
            sscanf((const char*)inputString, scanString, cmdString, argValuesOut[0], argValuesOut[1], argValuesOut[2], argValuesOut[3], argValuesOut[4]);
            return COMMAND_VALID;
        }
    }

    return COMMAND_INVALID;
}

void BuildOutputString(ParseCmdStatus parseStatus, const uint8_t* inputString, uint8_t* outputString)
{

}

CmdProcStatus CmdProc_HandleData(uint8_t data, uint8_t* outputString)
{
    uint8_t inputString[CMD_PROC_INPUT_STR_LEN] = { 0 };
    AssembleCmdStatus assembleStatus = CmdProc_AssembleCmd(data, inputString);

    if (assembleStatus == COMMAND_READY)
    {
        const CmdProc_Command* command;
        int argCount = 0;
        float argValues[CMD_PROC_MAX_ARGS];
        ParseCmdStatus parseStatus = CmdProc_ParseInputString(inputString, command, &argCount, argValues);
        if (parseStatus == COMMAND_VALID)
        {
            command->cmdFunc(argCount, argValues, inputString, outputString);
            return CMD_PROC_DONE;
        }
    }

    return CMD_PROC_WAITING;
}