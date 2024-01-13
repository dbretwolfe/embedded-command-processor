#include <format>

#include "command_functions.hpp"

using namespace UartCmdProc;

HandlerStatus TestArgsFunc(std::vector<float> args, const std::string& inputString, std::string& handlerRespString)
{
    handlerRespString += std::format("argc = {}", args.size());

    if (args.size() > 0)
    {
        handlerRespString += std::format(" args = {}", args[0]);
    }

    for(int i = 1; i < args.size(); i++)
    {
        handlerRespString += std::format(", {}", args[i]);
    }
    
    return HandlerStatus::SUCCESS_MSG;
}

std::vector<UartCmdProc::Command> GetTestCommandList(void)
{
    std::vector<UartCmdProc::Command> commandList;

    Command testArgsCommand("TST0", TestArgsFunc);

    commandList.push_back(testArgsCommand);

    return commandList;
}