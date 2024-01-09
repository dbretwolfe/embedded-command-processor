#pragma once

#include <cstdint>
#include <vector>

#include "command.hpp"

namespace UartCmdProc
{
    enum class CmdProcStatus {WAITING, DONE};

    class CommandProcessor
    {
    public:
        CommandProcessor(std::vector<Command>&& commandList, size_t inputBufferSize);
        CmdProcStatus HandleData(uint8_t data, std::string& outputString);
        void SetInputChar(char inputChar){ _inputChar = inputChar; };
        void SetOutputChar(char outputChar){ _outputChar = outputChar; };
        void SetStopChar(char stopChar){ _stopChar = stopChar; };

    private:
        enum class AssembleCmdStatus { COMMAND_READY = 0, COMMAND_NOT_READY = -1 };
        enum class AssembleState { CMD_PROC_IDLE, CMD_PROC_ASSEMBLE };

        AssembleCmdStatus AssembleCommand(uint8_t dataIn);
        Command& MatchCommandId(void);
        std::vector<float> ParseArguments(std::string cmdId);
        std::string BuildOutputString(HandlerStatus handlerStatus, std::string& handlerRespString);

        AssembleState _assembleState = AssembleState::CMD_PROC_IDLE;
        int _commandBufIndex = 0;
        uint8_t _inputChar = '@';
        uint8_t _outputChar = '#';
        uint8_t _stopChar = '\r';

        constexpr static int _maxArgCount = 5;

        const size_t _inputBufferSize;
        std::string _inputBuffer;
        std::string _inputString;

        std::vector<Command> _commandList;
    };
}