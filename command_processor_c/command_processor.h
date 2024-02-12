/*
*   UART Command Processor
*   
*   This module defines functions to parse ASCII input from a human user to extract commands and arguments for
*   execution.
*   
*   The CmdProc_HandleInputData() function is called upon receipt of each new byte of data from the host. A state
*   machine uses start and stop characters to delimit command strings, which are then parsed to extract a command ID
*   and arguments.  The command ID is used to call an associated function pointer defined in the command functions
*   module.  Once the command function is executed, the output from the function is used to craft a response to the
*   user.
*/

#pragma once

#include <stdint.h>

#include "cmd_proc_buffers.h"

#ifndef CMD_PROC_START_CHAR
#define CMD_PROC_START_CHAR '@'
#endif

#ifndef CMD_PROC_OUTPUT_CHAR
#define CMD_PROC_OUTPUT_CHAR '#'
#endif

#ifndef CMD_PROC_STOP_CHAR
#define CMD_PROC_STOP_CHAR '\r'
#endif

typedef enum {CMD_PROC_DONE, CMD_PROC_WAITING} CmdProcStatus;

CmdProcStatus CmdProc_HandleInputData(uint8_t data, uint8_t* outputString);