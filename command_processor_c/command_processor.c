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

// Assemble input bytes into a string.  The state machine starts assembling when it detects the start character,
// and stops when it detects the stop character.  The completed string is copied out to the inputString argument once
// the stop character is detected.  The return value is an enum indicating whether a completed command is ready.
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
                commandBuffer[commandBufIndex++] = dataIn;
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

// From the input string, determine what command function should be called from the command list, and parse any arguments.
static ParseCmdStatus CmdProc_ParseInputString(
    const uint8_t* inputString, 
    const CmdProc_Command** commandOut, 
    int* argCountOut, 
    float* argValuesOut)
{
    for (int i = 0; i < CMD_PROC_MAX_NUM_CMDS; i++)
    {
        if (strncmp((const char*)inputString, commandList[i].cmdId, CMD_PROC_ID_LEN) == 0)
        {
            *commandOut = &commandList[i];

            char scanString[30] = {0};
            // Floats are scanned because they can accurately represent and be casted to the supported argument types.
            sprintf(scanString, "%c%uc %%f, %%f, %%f, %%f, %%f", '%', CMD_PROC_ID_LEN);

            char cmdString[5];
            *argCountOut = sscanf(
                (const char*)inputString,
                scanString,
                cmdString,
                &argValuesOut[0], 
                &argValuesOut[1], 
                &argValuesOut[2], 
                &argValuesOut[3], 
                &argValuesOut[4]);
            
            if (*argCountOut > 0)
            {
                *argCountOut -= 1;
            }

            return COMMAND_VALID;
        }
    }

    return COMMAND_INVALID;
}

// Craft a string incorporating the output from the command function plus any error information.
void CmdProc_BuildOutputString(
    CmdProc_HandlerStatus handlerStatus, 
    const uint8_t* inputString, 
    const uint8_t* handlerRespString, 
    uint8_t* outputString)
{
    outputString[0] = CMD_PROC_OUTPUT_CHAR;
    
    switch (handlerStatus)
    {
        case CMD_PROC_SUCCESS_MSG:
            snprintf(&outputString[1], (CMD_PROC_OUTPUT_STR_LEN - 1), "%s %s ACK\r\n", inputString, handlerRespString);
            break;

        case CMD_PROC_SUCCESS:
            snprintf(&outputString[1], (CMD_PROC_OUTPUT_STR_LEN - 1), "%s ACK\r\n", inputString);
            break;

        case CMD_PROC_INVALID_ARGS:
            snprintf(&outputString[1], (CMD_PROC_OUTPUT_STR_LEN - 1), "%s INVALID ARGS ERR\r\n", inputString);
            break;

        case CMD_PROC_ARG_RANGE_ERR:
            snprintf(&outputString[1], (CMD_PROC_OUTPUT_STR_LEN - 1), "%s ARG RANGE ERR\r\n", inputString);
            break;

        case CMD_PROC_ERR_MSG:
            snprintf(&outputString[1], (CMD_PROC_OUTPUT_STR_LEN - 1), "%s %s ERR\r\n", inputString, handlerRespString);
            break;

        default:
            assert(0);
            snprintf(&outputString[1], (CMD_PROC_OUTPUT_STR_LEN - 1), "%s unexpected function error code ERR\r\n", inputString);
    }
}

// Process a byte of input data.  This function is called once for each incoming byte.  The return value is an enum
// indicating whether a command was executed on this call of the function.
CmdProcStatus CmdProc_HandleInputData(uint8_t data, uint8_t* outputString)
{
    uint8_t inputString[CMD_PROC_INPUT_STR_LEN] = { 0 };
    AssembleCmdStatus assembleStatus = CmdProc_AssembleCmd(data, inputString);

    if (assembleStatus == COMMAND_READY)
    {
        const CmdProc_Command* command;
        int argCount = 0;
        float argValues[CMD_PROC_MAX_ARGS] = { 0 };
        ParseCmdStatus parseStatus = CmdProc_ParseInputString(inputString, &command, &argCount, argValues);
        if (parseStatus == COMMAND_VALID)
        {
            uint8_t handlerRespString[CMD_PROC_HANDLER_RESP_STR_LEN] = { 0 };
            CmdProc_HandlerStatus handlerStatus = command->cmdFunc(argCount, argValues, inputString, handlerRespString);
            CmdProc_BuildOutputString(handlerStatus, inputString, handlerRespString, outputString);
            
            return CMD_PROC_DONE;
        }
    }

    return CMD_PROC_WAITING;
}