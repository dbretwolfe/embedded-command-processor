#include "test_fixture.h"

static void SendCommand(const uint8_t* inputString, int len, uint8_t* outputString)
{
    /*
    CmdProcStatus cmdProcStatus = CMD_PROC_WAITING;

    for (int i = 0; i < (len - 1); i++)
    {
        cmdProcStatus = CmdProc_HandleData(inputString[i], outputString);
        EXPECT_EQ(cmdProcStatus, CMD_PROC_WAITING);
    }

    cmdProcStatus = CmdProc_HandleData(inputString[(len - 1)], outputString);
    EXPECT_EQ(cmdProcStatus, CMD_PROC_DONE);
    */
}

TEST_F(CmdProcCppTest, TestArgs)
{
    
    //SendCommand(inputString, strlen((const char*)inputString), outputString);

    //printf((const char*)outputString);
}