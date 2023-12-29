#pragma once

#include <stdint.h>

#define CMD_PROC_INPUT_STR_LEN  (256)
#define CMD_PROC_OUTPUT_STR_LEN (256)

#ifndef CMD_PROC_START_CHAR
#define CMD_PROC_START_CHAR '@'
#endif

#ifndef CMD_PROC_RESPONSE_CHAR
#define CMD_PROC_RESPONSE_CHAR '#'
#endif

#ifndef CMD_PROC_STOP_CHAR
#define CMD_PROC_STOP_CHAR '\r'
#endif

typedef enum {CMD_PROC_DONE, CMD_PROC_WAITING} CmdProcStatus;

CmdProcStatus CmdProc_HandleData(uint8_t data, uint8_t* outputString);