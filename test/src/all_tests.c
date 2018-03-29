#include "unity_fixture.h"

static void RunAllTests(void)
{
    /* Lab 2 */
    RUN_TEST_GROUP(LED);
    RUN_TEST_GROUP(Encoder);
    RUN_TEST_GROUP(Potentiometer);

    /* Lab 3 */
    RUN_TEST_GROUP(CircBuffer);
    RUN_TEST_GROUP(CmdLineBuffer);
    RUN_TEST_GROUP(StdioRedirect);
    RUN_TEST_GROUP(CmdParse);
    RUN_TEST_GROUP(CmdProcess);
    RUN_TEST_GROUP(CmdLED);
    RUN_TEST_GROUP(CmdEnc);
    RUN_TEST_GROUP(CmdPot);
}

int main(int argc, const char * argv[])
{
    return UnityMain(argc, argv, RunAllTests);
}