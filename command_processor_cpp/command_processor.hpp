/*
*   UART Command Processor class
*   
*   This class defines a command processor object to parse ASCII input from a human user to extract commands and 
*   arguments for execution.
*   
*   The HandleInputData() function is called upon receipt of each new byte of data from the host. A state
*   machine uses start and stop characters to delimit command strings, which are then parsed to extract a command ID
*   and arguments.  The command ID is used to call an associated function pointer defined in the command functions
*   module.  Once the command function is executed, the output from the function is used to craft a response to the
*   user.
*/

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
        typedef struct 
        {
            uint8_t startChar;
            uint8_t stopChar;
            uint8_t outputChar;
        } CmdProcConfig;

        CommandProcessor(CmdProcConfig config, std::vector<Command>&& commandList, size_t inputBufferSize);
        CmdProcStatus HandleInputData(uint8_t data, std::string& outputString);

    private:
        enum class AssembleCmdStatus { COMMAND_READY = 0, COMMAND_NOT_READY = -1 };
        enum class AssembleState { CMD_PROC_IDLE, CMD_PROC_ASSEMBLE };

        AssembleCmdStatus AssembleCommand(uint8_t dataIn);
        Command& MatchCommandId(void);
        std::vector<float> ParseArguments(std::string cmdId);
        std::string BuildOutputString(HandlerStatus handlerStatus, std::string& handlerRespString);

        AssembleState _assembleState = AssembleState::CMD_PROC_IDLE;
        int _commandBufIndex = 0;
        const uint8_t _startChar;
        const uint8_t _stopChar;
        const uint8_t _outputChar;

        constexpr static int _maxArgCount = 5;

        const size_t _maxInputBufferSize;
        std::string _inputBuffer;
        std::string _inputString;

        std::vector<Command> _commandList;
    };
}