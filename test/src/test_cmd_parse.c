#include "cmd_parser.h"

#include "unity_fixture.h"
#include "stdio_redirect.h"

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

    push_stdout();
    cmd_parse(NULL);
    rewind(stdout);
    fgets(str_actual,sizeof(str_actual),fstdout());
    pop_stdout();

    TEST_ASSERT_EQUAL_STRING("ERROR: Tried to parse NULL command pointer\n",str_actual);
}

TEST(CmdParse, ParseEmpty)
{
    char cmd[] = "";
    char str_actual[80];

    push_stdout();
    cmd_parse(cmd);
    rewind(stdout);
    char * s = fgets(str_actual,sizeof(str_actual),fstdout());
    int is_eof = feof(fstdout());
    pop_stdout();

    TEST_ASSERT_TRUE(is_eof);
    TEST_ASSERT_EQUAL_STRING(NULL, s);
}

TEST(CmdParse, ParseUnknown)
{
    char cmd[] = "not a known command string";
    char str_actual[80];

    push_stdout();
    cmd_parse(cmd);
    rewind(stdout);
    fgets(str_actual,sizeof(str_actual),fstdout());
    pop_stdout();

    char str_expected[1024];
    const char * fmt = "Unknown command: \"%s\"\n";
    snprintf(str_expected, sizeof(str_expected), fmt, cmd);
    TEST_ASSERT_EQUAL_STRING(str_expected,str_actual);
}

TEST_GROUP_RUNNER(CmdParse)
{
    RUN_TEST_CASE(CmdParse, ParseNull);
    RUN_TEST_CASE(CmdParse, ParseEmpty);
    RUN_TEST_CASE(CmdParse, ParseUnknown);
}