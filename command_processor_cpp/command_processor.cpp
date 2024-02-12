#include <cassert>
#include <stdexcept>
#include <format>

#include "command_processor.hpp"

namespace UartCmdProc
{
    CommandProcessor::CommandProcessor(CmdProcConfig config, std::vector<Command>&& commandList, size_t bufferSize) :
    _startChar(config.startChar),
    _stopChar(config.stopChar),
    _outputChar(config.outputChar),
    _commandList(commandList),
    _maxInputBufferSize(bufferSize)
    {
    }

    // Process a byte of input data.  This function is called once for each incoming byte.  The return value is an enum
    // indicating whether a command was executed on this call of the function.
    CmdProcStatus CommandProcessor::HandleInputData(uint8_t data, std::string& outputString)
    {
        AssembleCmdStatus assembleStatus = AssembleCommand(data);

        if (assembleStatus == AssembleCmdStatus::COMMAND_READY)
        {
            try
            {
                Command command = MatchCommandId();
                std::vector<float> args = ParseArguments(command.cmdId);
                std::string handlerRespString;
                HandlerStatus handlerStatus = command.Execute(args, _inputString, handlerRespString);
                outputString = BuildOutputString(handlerStatus, handlerRespString);

                return CmdProcStatus::DONE;
            }
            catch(const std::invalid_argument& e)
            {
                return CmdProcStatus::WAITING;
            }
        }
        else
        {
            return CmdProcStatus::WAITING;
        }
    }

    // Assemble input bytes into a string.  The state machine starts assembling when it detects the start character,
    // and stops when it detects the stop character.  The completed string is copied out to the inputString argument once
    // the stop character is detected.  The return value is an enum indicating whether a completed command is ready.
    CommandProcessor::AssembleCmdStatus CommandProcessor::AssembleCommand(uint8_t dataIn)
    {
        if (_assembleState == AssembleState::CMD_PROC_IDLE)
        {
            if (dataIn == _startChar)
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
                _inputBuffer.resize(0);
                //_commandBufIndex = 0;
                _assembleState = AssembleState::CMD_PROC_IDLE;
                return AssembleCmdStatus::COMMAND_READY;
            }
            else
            {
                if (_inputBuffer.size() == _maxInputBufferSize)
                {
                    _inputBuffer.resize(0);
                    //_commandBufIndex = 0;
                    _assembleState = AssembleState::CMD_PROC_IDLE;
                }
                else
                {
                    //_inputBuffer[_commandBufIndex++] = dataIn;
                    _inputBuffer.push_back(dataIn);
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

    // Search the command list for a command with an ID matching the one found in the input string.
    // If a match is found, return a reference to the command, otherwise throw.
    Command& CommandProcessor::MatchCommandId(void)
    {
        for (int i = 0; i < _commandList.size(); i++)
        {
            int cmdIdLen = _commandList[i].cmdId.length();

            if (_commandList[i].cmdId.compare(0, cmdIdLen, _inputString) == 0);
            {
                return _commandList[i];
            }
        }

        throw std::invalid_argument("Invalid command!");
    }

    // Parse the input string to extract arguments.  Returns a vector containing any argument values.
    std::vector<float> CommandProcessor::ParseArguments(std::string cmdId)
    {
        std::string scanString;
        // Floats are scanned because they can accurately represent and be casted to the supported argument types.
        scanString = std::format("{} %f, %f, %f, %f, %f", cmdId);

        float tempArgs[_maxArgCount];

        int numScanned = sscanf(
            _inputString.c_str(),
            scanString.c_str(),
            &tempArgs[0], 
            &tempArgs[1], 
            &tempArgs[2], 
            &tempArgs[3], 
            &tempArgs[4]); // TODO: make arg list variable?

        if (numScanned <= 0)
        {
            throw std::invalid_argument("Invalid command!");
        }

        std::vector<float> tempArgVec;
        
        for (int i = 0; i < numScanned; i++)
        {
            tempArgVec.push_back(tempArgs[i]);
        }

        return tempArgVec;
    }

    // Craft a string incorporating the output from the command function plus any error information.
    std::string CommandProcessor::BuildOutputString(HandlerStatus handlerStatus, std::string& handlerRespString)
    {
        std::string outputString;
        outputString += _outputChar;
    
        switch (handlerStatus)
        {
            case HandlerStatus::SUCCESS_MSG:
                outputString += std::format("{} {} ACK\r\n", _inputString, handlerRespString);
                break;

            case HandlerStatus::SUCCESS:
                outputString += std::format("{} ACK\r\n", _inputString);
                break;

            case HandlerStatus::INVALID_ARGS:
                outputString += std::format("{} INVALID ARGS ERR\r\n", _inputString);
                break;

            case HandlerStatus::ARG_RANGE_ERR:
                outputString += std::format("{} ARG RANGE ERR\r\n", _inputString);
                break;

            case HandlerStatus::ERR_MSG:
                outputString += std::format("{} {} ERR\r\n", _inputString, handlerRespString);
                break;

            default:
                assert(0);
                outputString += std::format("{} unexpected error code ERR\r\n", _inputString);
        }

        return outputString;
    }
}