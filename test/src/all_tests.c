#include "unity_fixture.h"

static void RunAllTests(void)
{
    /* Lab 3 */
    RUN_TEST_GROUP(CircBuffer);
    RUN_TEST_GROUP(CmdLineBuffer);
    RUN_TEST_GROUP(IOSpy);
    RUN_TEST_GROUP(CmdParse);
    RUN_TEST_GROUP(CmdProcess);


    /* Lab 4 */
    RUN_TEST_GROUP(Ctrl);
    RUN_TEST_GROUP(CmdCtrl);
}

int main(int argc, const char * argv[])
{
    return UnityMain(argc, argv, RunAllTests);
}