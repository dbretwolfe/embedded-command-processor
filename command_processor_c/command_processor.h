#pragma once

#include <stdint.h>

#include "cmd_proc_buffers.h"

#ifndef CMD_PROC_INPUT_CHAR
#define CMD_PROC_INPUT_CHAR '@'
#endif

#ifndef CMD_PROC_OUTPUT_CHAR
#define CMD_PROC_OUTPUT_CHAR '#'
#endif

#ifndef CMD_PROC_STOP_CHAR
#define CMD_PROC_STOP_CHAR '\r'
#endif

typedef enum {CMD_PROC_DONE, CMD_PROC_WAITING} CmdProcStatus;

CmdProcStatus CmdProc_HandleData(uint8_t data, uint8_t* outputString);