#include "unity_fixture.h"
#include "stdio_redirect.h"


TEST_GROUP(StdioRedirect);

TEST_SETUP(StdioRedirect)
{
}

TEST_TEAR_DOWN(StdioRedirect)
{
}

TEST(StdioRedirect,RedirectInString)
{
    const char *str_expected = "This is a test of redirecting stdin\n";
    char str_actual[80];
    int n;
    char * s;

    push_stdin();
    n = fputs(str_expected,fstdin());
    rewind(fstdin());
    s = n >= 0 ? fgets(str_actual, sizeof(str_actual), stdin) : NULL;
    pop_stdin();

    TEST_ASSERT_TRUE(n >= 0);
    TEST_ASSERT_EQUAL_STRING(str_expected,str_actual);

    TEST_ASSERT_NOT_NULL(s);
    TEST_ASSERT_EQUAL_PTR(str_actual, s);
    TEST_ASSERT_EQUAL_STRING(str_expected, str_actual);
}

TEST(StdioRedirect,SkipInputChar)
{
    push_stdin();
    fputc('A',fstdin());
    fputc('B',fstdin());
    fputc('C',fstdin());
    rewind(fstdin()); // Don't rewind stdin
    int c1 = fgetc(stdin);
    fseek(stdin, 1L, SEEK_CUR); // Don't forward fstdin()
    int c2 = fgetc(stdin);
    int c3 = fgetc(stdin);
    clearerr(stdin); // <-- Secret sauce
    pop_stdin();

    TEST_ASSERT_EQUAL_HEX('A',c1);
    TEST_ASSERT_EQUAL_HEX('C',c2);
    TEST_ASSERT_EQUAL_HEX(EOF,c3);    
}

TEST(StdioRedirect,AddInputChar)
{
    push_stdin();
    fputc('A',fstdin());
    rewind(fstdin());
    // clearerr(stdin); // <-- Secret sauce
    int c1 = fgetc(stdin);
    // Do we need to forward fstdin() here?
    int n2 = fputc('B',fstdin()); // returns 'B'
    int n_ = fseek(fstdin(), -1L, SEEK_CUR); // returns 0 
    int c2 = fgetc(stdin);
    int c3 = fgetc(stdin);
    clearerr(stdin); // <-- Secret sauce
    pop_stdin();

    TEST_ASSERT_EQUAL_HEX('B',n2);
    TEST_ASSERT_EQUAL_HEX(0,n_);
    TEST_ASSERT_EQUAL_HEX('A',c1);
    TEST_ASSERT_EQUAL_HEX('B',c2);
    TEST_ASSERT_EQUAL_HEX(EOF,c3);    
}

TEST(StdioRedirect,RedirectOut)
{
    const char *str_expected = "This is a test of redirecting stdout";
    char str_actual[80];
    int ret;
    char * ret2;

    push_stdout();
    ret = fputs(str_expected, stdout);
    // fflush(stdout);
    rewind(stdout);
    ret2 = ret >= 0 ? fgets(str_actual, sizeof(str_actual), fstdout()) : NULL;
    pop_stdout();

    TEST_ASSERT_TRUE(ret >= 0);
    TEST_ASSERT_EQUAL_STRING(str_expected,str_actual);

    TEST_ASSERT_NOT_NULL(ret2);
    TEST_ASSERT_EQUAL_PTR(str_actual, ret2);
    TEST_ASSERT_EQUAL_STRING(str_expected, str_actual);
}

TEST(StdioRedirect,RedirectInOut)
{
    const char *in = "This is a test of redirecting stdin and stdout\n";
    static char buf[80], out[80];
    int n;

    push_stdio();
    fputs(in,fstdin());
    rewind(fstdin());
    char *s = fgets(buf, sizeof(buf), stdin);
    n = s ? fputs(buf, stdout) : -1;
    rewind(stdout);
    fgets(out,sizeof(out),fstdout());
    pop_stdio();

    TEST_ASSERT_NOT_NULL(s);
    TEST_ASSERT_EQUAL_PTR(buf, s);
    TEST_ASSERT_EQUAL_STRING(in, buf);

    TEST_ASSERT_TRUE(n >= 0);
    TEST_ASSERT_EQUAL_STRING(buf,out);

    TEST_ASSERT_EQUAL_STRING(in,out);
}

TEST_GROUP_RUNNER(StdioRedirect)
{

    RUN_TEST_CASE(StdioRedirect,RedirectInString);
    RUN_TEST_CASE(StdioRedirect,SkipInputChar);
    RUN_TEST_CASE(StdioRedirect,AddInputChar);
    RUN_TEST_CASE(StdioRedirect,RedirectOut);
    RUN_TEST_CASE(StdioRedirect,RedirectInOut);
}
