#pragma once

#include <cstdint>
#include <string>
#include <vector>

namespace UartCmdProc
{
    enum class HandlerStatus
    {
        SUCCESS_MSG = 1,
        SUCCESS = 0,
        INVALID_ARGS = -1,
        ARG_RANGE_ERR = -2,
        ERR_MSG = -3
    };

    // Typedef used for command handler function pointers.
    typedef HandlerStatus (*CmdProcHandler)(
        std::vector<float> args,
        const std::string& inputString,
        std::string& handlerRespString);

    // Command object class.  A command object is created with a command ID and a command handler function.
    // The object is interacted with by calling the Execute public member function.
    class Command
    {
    public:
        Command(std::string&& cmdId, CmdProcHandler handler);
        HandlerStatus Execute(
            std::vector<float> args, 
            const std::string& inputString, 
            std::string& handlerRespString);
        const std::string cmdId;
        
    private:
        const CmdProcHandler _handler;
    };
}