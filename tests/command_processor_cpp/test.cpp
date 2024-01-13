#include <string>
#include <iostream>

#include "test_fixture.h"

using namespace UartCmdProc;

void CmdProcCppTest::SendCommand(const std::string inputString, std::string& outputString)
{
    CmdProcStatus cmdProcStatus = CmdProcStatus::WAITING;

    for (int i = 0; i < (inputString.length() - 1); i++)
    {
        cmdProcStatus = cmdProc->HandleData(inputString[i], outputString);
        EXPECT_EQ(cmdProcStatus, CmdProcStatus::WAITING);
    }

    cmdProcStatus = cmdProc->HandleData(inputString[(inputString.length() - 1)], outputString);
    EXPECT_EQ(cmdProcStatus, CmdProcStatus::DONE);
}

TEST_F(CmdProcCppTest, TestArgs)
{
    std::string inputString = "@TST0 1.2, 3, 4.56, 7.8, -9\r";
    std::string outputString;
    SendCommand(inputString, outputString);

    std::cout << outputString << std::endl;
}