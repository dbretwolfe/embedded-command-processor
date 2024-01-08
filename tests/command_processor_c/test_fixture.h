#pragma once

#include <stdint.h>

#include <gtest/gtest.h>

extern "C"
{
    #include "command_processor.h"
}

class CmdProcCTest : public testing::Test
{
protected:
    void SetUp() override
    {
    }

    uint8_t outputString[CMD_PROC_OUTPUT_STR_LEN];
};