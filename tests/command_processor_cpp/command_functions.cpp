#include "command_functions.hpp"
#include <format>

using namespace UartCmdProc;

HandlerStatus TestArgsFunc(std::vector<float> args, std::string inputString, std::string handlerRespString)
{
    handlerRespString += std::format("%s argc = %u", inputString, args.size());

    if (args.size() > 1)
    {
        handlerRespString += std::format("args = %f", args[0]);
    }

    for(int i = 0; i < args.size(); i++)
    {
        handlerRespString += std::format(", %f", args[i]);
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