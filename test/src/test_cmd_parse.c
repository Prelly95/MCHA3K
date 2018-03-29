#include "unity_fixture.h"
#include "iospy.h"

#include "cmd_parser.h"
#include <string.h>

TEST_GROUP(CmdParse);

TEST_SETUP(CmdParse)
{
}

TEST_TEAR_DOWN(CmdParse)
{
}

TEST(CmdParse, ParseNull)
{
    char str_actual[80];

    iospy_hook_out();
    cmd_parse(NULL);
    rewind(stdout);
    fgets(str_actual,sizeof(str_actual),iospy_get_fp_out());
    iospy_unhook_out();

    TEST_ASSERT_EQUAL_STRING("ERROR: Tried to parse NULL command pointer\n",str_actual);
}

TEST(CmdParse, ParseEmpty)
{
    char cmd[] = "";
    char str_actual[80];

    iospy_hook_out();
    cmd_parse(cmd);
    rewind(stdout);
    char * s = fgets(str_actual,sizeof(str_actual),iospy_get_fp_out());
    int is_eof = feof(iospy_get_fp_out());
    iospy_unhook_out();

    TEST_ASSERT_TRUE(is_eof);
    TEST_ASSERT_EQUAL_STRING(NULL, s);
}

TEST(CmdParse, ParseUnknown)
{
    char cmd[] = "not a known command string";
    char str_actual[80];

    iospy_hook_out();
    cmd_parse(cmd);
    iospy_pop_out_str(str_actual, sizeof(str_actual)/sizeof(str_actual[0]));
    // rewind(stdout);
    // fgets(str_actual,sizeof(str_actual),iospy_get_fp_out());
    iospy_unhook_out();

    char str_expected[1024];
    const char * fmt = "Unknown command: \"%s\"\n";
    snprintf(str_expected, sizeof(str_expected), fmt, cmd);
    TEST_ASSERT_EQUAL_STRING(str_expected,str_actual);
}

TEST(CmdParse, ParseUnknownTwice)
{
    char cmd[] = "not a known command string";
    char str_actual1[80];
    char str_actual2[80];

    iospy_hook_out();
    cmd_parse(cmd);
    iospy_pop_out_str(str_actual1, sizeof(str_actual1)/sizeof(str_actual1[0]));
    cmd_parse(cmd);
    iospy_pop_out_str(str_actual2, sizeof(str_actual2)/sizeof(str_actual2[0]));
    iospy_unhook_out();

    char str_expected[1024];
    const char * fmt = "Unknown command: \"%s\"\n";
    snprintf(str_expected, sizeof(str_expected), fmt, cmd);
    TEST_ASSERT_EQUAL_STRING(str_expected, str_actual1);
    TEST_ASSERT_EQUAL_STRING(str_expected, str_actual2);
}

TEST_GROUP_RUNNER(CmdParse)
{
    RUN_TEST_CASE(CmdParse, ParseNull);
    RUN_TEST_CASE(CmdParse, ParseEmpty);
    RUN_TEST_CASE(CmdParse, ParseUnknown);
    RUN_TEST_CASE(CmdParse, ParseUnknownTwice);
}