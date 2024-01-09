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
        cmdProc = new UartCmdProc::CommandProcessor(GetTestCommandList(), inputBufferSize);
    }

    constexpr static int inputBufferSize = 256;
    UartCmdProc::CommandProcessor* cmdProc;
};