#include "stdio_redirect.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <assert.h>

static int fd_stdin_bak, fd_stdin_org;
static FILE *fp_stdin;

FILE * fstdin(void)
{
    return fp_stdin;
}

void push_stdin(void)
{
    fp_stdin = tmpfile(); // This will be deleted on fclose() or when the process exits normally
    if (fp_stdin == NULL)
    {
        perror("Unable to open temp file");
        abort();
    }

    fflush(stdin);

    // Save stdin file descriptors
    fd_stdin_org = fileno(stdin); // should be 0, but save in case something else redirects it
    fd_stdin_bak = dup(fd_stdin_org);

    // Overwrite stdin file descriptors
    assert(dup2(fileno(fp_stdin), fd_stdin_org) != -1);
}

void pop_stdin(void)
{
    fflush(stdin);
    if (feof(stdin)) clearerr(stdin);

    // Restore stdin file descriptors
    assert(dup2(fd_stdin_bak, fd_stdin_org) != -1);
    close(fd_stdin_bak);
}

static int fd_stdout_bak, fd_stdout_org;
static FILE *fp_stdout;

FILE * fstdout(void)
{
    return fp_stdout;
}

void push_stdout(void)
{
    fflush(stdout);

    fp_stdout = tmpfile(); // This will be deleted on fclose() or when the process exits normally
    if (fp_stdout == NULL)
    {
        perror("Unable to open temp file");
        abort();
    }

    // Save stdout file descriptors
    fd_stdout_org = fileno(stdout); // should be 1, but save in case something else redirects it
    fd_stdout_bak = dup(fd_stdout_org);

    // Overwrite stdout file descriptors
    assert(dup2(fileno(fp_stdout), fd_stdout_org) != -1);
}

void pop_stdout(void)
{
    fflush(stdout);

    // Restore stdout file descriptors
    assert(dup2(fd_stdout_bak, fd_stdout_org) != -1);
    close(fd_stdout_bak);

    fclose(fp_stdout);
}

void push_stdio(void)
{
    push_stdout();
    push_stdin();
}

void pop_stdio(void)
{
    pop_stdin();
    pop_stdout();
}