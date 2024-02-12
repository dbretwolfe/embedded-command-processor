#pragma once

#include <cstdint>
#include <vector>

#include <gtest/gtest.h>

#include "command_processor.hpp"
#include "command_functions.hpp"

class CmdProcCppTest : public testing::Test
{
protected:
    void SetUp() override
    {
        UartCmdProc::CommandProcessor::CmdProcConfig config = { .startChar = '@', .stopChar = '\r', .outputChar = '#' };
        cmdProc = new UartCmdProc::CommandProcessor(config, GetTestCommandList(), inputBufferSize);
    }

    void SendCommand(const std::string inputString, std::string& outputString);

    constexpr static int inputBufferSize = 256;
    UartCmdProc::CommandProcessor* cmdProc;
};