#include <cassert>
#include <stdexcept>
#include <format>

#include "command_processor.hpp"

namespace UartCmdProc
{
    CommandProcessor::CommandProcessor(std::vector<Command>&& commandList, size_t bufferSize) : 
    _commandList(commandList),
    _bufferSize(bufferSize)
    {
        _inputBuffer.resize(bufferSize);
        _inputString.resize(bufferSize);
        _outputString.resize(bufferSize);
    }

    CmdProcStatus CommandProcessor::HandleData(uint8_t data, const std::string& outputString)
    {
        AssembleCmdStatus assembleStatus = AssembleCommand(data);

        if (assembleStatus == AssembleCmdStatus::COMMAND_READY)
        {
            try
            {
                std::vector<float> args;
                Command command = ParseInputString(args);
                std::string handlerRespString;
                HandlerStatus handlerStatus = command.Execute(args, _inputString, handlerRespString);
                BuildOutputString(handlerStatus, handlerRespString);
            }
            catch(const std::invalid_argument& e)
            {
                return CmdProcStatus::WAITING;
            }
            
            return CmdProcStatus::DONE;
        }

        return CmdProcStatus::WAITING;
    }

    CommandProcessor::AssembleCmdStatus CommandProcessor::AssembleCommand(uint8_t dataIn)
    {
        if (_assembleState == AssembleState::CMD_PROC_IDLE)
        {
            if (dataIn == _inputChar)
            {
                _assembleState = AssembleState::CMD_PROC_ASSEMBLE;
            }

            return AssembleCmdStatus::COMMAND_NOT_READY;
        }
        else if (_assembleState == AssembleState::CMD_PROC_ASSEMBLE)
        {
            if (dataIn == _stopChar)
            {
                _inputString = _inputBuffer;
                _commandBufIndex = 0;
                _assembleState = AssembleState::CMD_PROC_IDLE;
                return AssembleCmdStatus::COMMAND_READY;
            }
            else
            {
                if (_commandBufIndex == (_bufferSize - 1))
                {
                    _commandBufIndex = 0;
                    _assembleState = AssembleState::CMD_PROC_IDLE;
                }
                else
                {
                    _inputBuffer[_commandBufIndex++] = dataIn;
                }
                
                return AssembleCmdStatus::COMMAND_NOT_READY;
            }
        }
        else
        {
            assert(0);
            _assembleState = AssembleState::CMD_PROC_IDLE;
            return AssembleCmdStatus::COMMAND_NOT_READY;
        }
    }

    Command& CommandProcessor::ParseInputString(std::vector<float>& args)
    {
        for (int i = 0; i < _commandList.size(); i++)
        {
            int cmdIdLen = _commandList[i].cmdId.length();

            if (_commandList[i].cmdId.compare(0, cmdIdLen, _inputString) == 0);
            {
                char scanString[30] = {0};
                // Floats are scanned because they can accurately represent and be casted to the supported argument types.
                sprintf(scanString, "%s %%f, %%f, %%f, %%f, %%f", _commandList[i].cmdId);

                float tempArgs[_maxArgCount];

                int numScanned = sscanf(
                    (const char*)_inputString.data(),
                    scanString,
                    &tempArgs[0], 
                    &tempArgs[1], 
                    &tempArgs[2], 
                    &tempArgs[3], 
                    &tempArgs[4]); // TODO: make arg list variable?

                std::vector<float> tempArgVec;
                for (int i = 0; i < numScanned; i++)
                {
                    tempArgVec.push_back(tempArgs[i]);
                }

                args = std::move(tempArgVec);
                return _commandList[i];
            }
        }

        throw std::invalid_argument("Invalid command!");
    }

    void CommandProcessor::BuildOutputString(HandlerStatus handlerStatus, std::string& handlerRespString)
    {
        _outputString[0] = _outputChar;
    
        switch (handlerStatus)
        {
            case HandlerStatus::SUCCESS_MSG:
                _outputString += std::format("%s %s ACK\r\n", _inputString, handlerRespString);
                break;

            case HandlerStatus::SUCCESS:
                _outputString += std::format("%s ACK\r\n", _inputString);
                break;

            case HandlerStatus::INVALID_ARGS:
                _outputString += std::format("%s INVALID ARGS ERR\r\n", _inputString);
                break;

            case HandlerStatus::ARG_RANGE_ERR:
                _outputString += std::format("%s ARG RANGE ERR\r\n", _inputString);
                break;

            case HandlerStatus::ERR_MSG:
                _outputString += std::format("%s %s ERR\r\n", _inputString, handlerRespString);
                break;

            default:
                assert(0);
                _outputString += std::format("%s unexpected function error code ERR\r\n", _inputString);
        }
    }
}