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
    RUN_TEST_GROUP(IOSpy);
    RUN_TEST_GROUP(CmdParse);
    RUN_TEST_GROUP(CmdProcess);
    RUN_TEST_GROUP(CmdLED);
    RUN_TEST_GROUP(CmdEnc);
    RUN_TEST_GROUP(CmdPot);

    /* Lab 4 */
    RUN_TEST_GROUP(CmdXY);
    RUN_TEST_GROUP(Ctrl);
    RUN_TEST_GROUP(CmdCtrl);

    /* Lab 5 */
    RUN_TEST_GROUP(Task);
    RUN_TEST_GROUP(SinTable);
}

int main(int argc, const char * argv[])
{
    return UnityMain(argc, argv, RunAllTests);
}