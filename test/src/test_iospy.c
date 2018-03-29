#include "unity_fixture.h"
#include "iospy.h"


TEST_GROUP(IOSpy);

TEST_SETUP(IOSpy)
{
}

TEST_TEAR_DOWN(IOSpy)
{
    iospy_unhook();
}

TEST(IOSpy,RedirectInString)
{
    const char *str_expected = "This is a test of redirecting stdin\n";
    char str_actual[80];
    int n;
    char * s;

    iospy_hook_in();
    n = fputs(str_expected,iospy_get_fp_in());
    rewind(iospy_get_fp_in());
    s = n >= 0 ? fgets(str_actual, sizeof(str_actual), stdin) : NULL;
    iospy_unhook_in();

    TEST_ASSERT_TRUE(n >= 0);
    TEST_ASSERT_EQUAL_STRING(str_expected, str_actual);

    TEST_ASSERT_NOT_NULL(s);
    TEST_ASSERT_EQUAL_PTR(str_actual, s);
    TEST_ASSERT_EQUAL_STRING(str_expected, str_actual);
}

TEST(IOSpy,SkipInputChar)
{
    iospy_hook_in();
    fputc('A',iospy_get_fp_in());
    fputc('B',iospy_get_fp_in());
    fputc('C',iospy_get_fp_in());
    rewind(iospy_get_fp_in()); // Don't rewind stdin
    int c1 = fgetc(stdin);
    fseek(stdin, 1L, SEEK_CUR); // Don't forward iospy_get_fp_in()
    int c2 = fgetc(stdin);
    int c3 = fgetc(stdin);
    clearerr(stdin); // <-- Secret sauce
    iospy_unhook_in();

    TEST_ASSERT_EQUAL_HEX('A',c1);
    TEST_ASSERT_EQUAL_HEX('C',c2);
    TEST_ASSERT_EQUAL_HEX(EOF,c3);    
}

TEST(IOSpy,AddInputChar)
{
    iospy_hook_in();
    fputc('A',iospy_get_fp_in());
    rewind(iospy_get_fp_in());
    int c1 = fgetc(stdin);
    // Do we need to forward iospy_get_fp_in() here?
    int n2 = fputc('B',iospy_get_fp_in()); // returns 'B'
    int n_ = fseek(iospy_get_fp_in(), -1L, SEEK_CUR); // returns 0 
    int c2 = fgetc(stdin);
    int c3 = fgetc(stdin);
    clearerr(stdin); // <-- Secret sauce
    iospy_unhook_in();

    TEST_ASSERT_EQUAL_HEX('B',n2);
    TEST_ASSERT_EQUAL_HEX(0,n_);
    TEST_ASSERT_EQUAL_HEX('A',c1);
    TEST_ASSERT_EQUAL_HEX('B',c2);
    TEST_ASSERT_EQUAL_HEX(EOF,c3);    
}

TEST(IOSpy,RedirectOut)
{
    const char *str_expected = "This is a test of redirecting stdout";
    char str_actual[80];
    int ret;
    char * ret2;

    iospy_hook_out();
    ret = fputs(str_expected, stdout);
    rewind(stdout);
    ret2 = ret >= 0 ? fgets(str_actual, sizeof(str_actual), iospy_get_fp_out()) : NULL;
    iospy_unhook_out();

    TEST_ASSERT_TRUE(ret >= 0);
    TEST_ASSERT_EQUAL_STRING(str_expected,str_actual);

    TEST_ASSERT_NOT_NULL(ret2);
    TEST_ASSERT_EQUAL_PTR(str_actual, ret2);
    TEST_ASSERT_EQUAL_STRING(str_expected, str_actual);
}

TEST(IOSpy,RedirectOutTwice)
{
    const char *str_expected1 = "String1\n";
    const char *str_expected2 = "STRING2\n";
    char str_actual1[80];
    char str_actual2[80];
    int n1;
    int n2;
    char * p1;
    char * p2;

    fpos_t pos;

    iospy_hook_out();
    fgetpos(stdout, &pos);
    n1 = fputs(str_expected1, stdout);
    fsetpos(stdout, &pos);
    if (feof(iospy_get_fp_out())) clearerr(iospy_get_fp_out());
    p1 = n1 >= 0 ? fgets(str_actual1, sizeof(str_actual1), iospy_get_fp_out()) : NULL;
    fgetpos(stdout, &pos);
    n2 = fputs(str_expected2, stdout);
    fsetpos(stdout, &pos);
    if (feof(iospy_get_fp_out())) clearerr(iospy_get_fp_out());
    p2 = n2 >= 0 ? fgets(str_actual2, sizeof(str_actual2), iospy_get_fp_out()) : NULL;
    fgetpos(stdout, &pos);
    iospy_unhook_out();

    TEST_ASSERT_TRUE(n1 >= 0);
    TEST_ASSERT_NOT_NULL(p1);
    TEST_ASSERT_EQUAL_PTR(str_actual1, p1);
    TEST_ASSERT_EQUAL_STRING(str_expected1, str_actual1);

    TEST_ASSERT_TRUE(n2 >= 0);
    TEST_ASSERT_NOT_NULL(p2);
    TEST_ASSERT_EQUAL_PTR(str_actual2, p2);
    TEST_ASSERT_EQUAL_STRING(str_expected2, str_actual2);    
}

TEST(IOSpy,RedirectOutTwiceAgain)
{
    const char *str_expected1 = "String1\n";
    const char *str_expected2 = "STRING2\n";
    char str_actual1[80];
    char str_actual2[80];
    int n1;
    int n2;

    iospy_hook_out();
    n1 = fputs(str_expected1, stdout);
    iospy_pop_out_str(str_actual1, sizeof(str_actual1)/sizeof(str_actual1[0]));
    n2 = fputs(str_expected2, stdout);
    iospy_pop_out_str(str_actual2, sizeof(str_actual2)/sizeof(str_actual2[0]));
    iospy_unhook_out();

    TEST_ASSERT_TRUE(n1 >= 0);
    TEST_ASSERT_EQUAL_STRING(str_expected1, str_actual1);

    TEST_ASSERT_TRUE(n2 >= 0);
    TEST_ASSERT_EQUAL_STRING(str_expected2, str_actual2);    
}

TEST(IOSpy,RedirectInOut)
{
    const char *in = "This is a test of redirecting stdin and stdout\n";
    static char buf[80], out[80];
    int n;

    iospy_hook();
    fputs(in,iospy_get_fp_in());
    rewind(iospy_get_fp_in());
    char *s = fgets(buf, sizeof(buf), stdin);
    n = s ? fputs(buf, stdout) : -1;
    rewind(stdout);
    fgets(out,sizeof(out),iospy_get_fp_out());
    iospy_unhook();

    TEST_ASSERT_NOT_NULL(s);
    TEST_ASSERT_EQUAL_PTR(buf, s);
    TEST_ASSERT_EQUAL_STRING(in, buf);

    TEST_ASSERT_TRUE(n >= 0);
    TEST_ASSERT_EQUAL_STRING(buf,out);

    TEST_ASSERT_EQUAL_STRING(in,out);
}

TEST_GROUP_RUNNER(IOSpy)
{

    RUN_TEST_CASE(IOSpy,RedirectInString);
    RUN_TEST_CASE(IOSpy,SkipInputChar);
    RUN_TEST_CASE(IOSpy,AddInputChar);
    RUN_TEST_CASE(IOSpy,RedirectOut);
    RUN_TEST_CASE(IOSpy,RedirectOutTwice);
    RUN_TEST_CASE(IOSpy,RedirectOutTwiceAgain);
    RUN_TEST_CASE(IOSpy,RedirectInOut);
}
