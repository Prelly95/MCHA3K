#include "unity_fixture.h"

static void RunAllTests(void)
{
    RUN_TEST_GROUP(LED);
    RUN_TEST_GROUP(Encoder);
    RUN_TEST_GROUP(Dimmer);
    RUN_TEST_GROUP(Potentiometer);
}

int main(int argc, const char * argv[])
{
    return UnityMain(argc, argv, RunAllTests);
}