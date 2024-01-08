#include "command.hpp"

namespace UartCmdProc
{
    Command::Command(std::string&& cmdId, CmdProcHandler handler) : cmdId(cmdId), _handler(handler) {};

    HandlerStatus Command::Execute(
        std::vector<float> args, 
        const std::string& inputString, 
        std::string& handlerRespString)
    {
        return _handler(args, inputString, handlerRespString);
    }
}