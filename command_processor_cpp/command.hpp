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

    typedef HandlerStatus (*CmdProcHandler)(
        std::vector<float> args,
        const std::string& inputString,
        std::string& handlerRespString);

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