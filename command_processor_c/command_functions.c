#include "command_functions.h"

static CmdProc_HandlerStatus GetFirmwareVersion(uint16_t argCount, void* argValues, uint8_t* inputString, uint8_t* outputString);

const CmdProc_Command commandList[CMD_PROC_MAX_NUM_CMDS] = 
{ 
    { .cmdId = "GFWV", .cmdFunc = GetFirmwareVersion }
};

CmdProc_HandlerStatus GetFirmwareVersion(uint16_t argCount, void* argValues, uint8_t* inputString, uint8_t* outputString)
{

}
