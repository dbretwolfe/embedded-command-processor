#pragma once

#include <cstdint>
#include <vector>
#include <string>

#include "command_processor.hpp"

UartCmdProc::HandlerStatus TestArgsFunc(std::vector<float> args, std::string inputString, std::string handlerRespString);

std::vector<UartCmdProc::Command> GetTestCommandList(void);