#include "iospy.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <assert.h>
#include <stdbool.h>

static int _fd_stdin_bak, _fd_stdin_org;
static FILE *_fp_stdin;
static bool _is_stdin_redirected = false;

FILE * iospy_get_fp_in(void)
{
    return _fp_stdin;
}

void iospy_hook_in(void)
{
    if (_is_stdin_redirected) return;

    _fp_stdin = tmpfile(); // This will be deleted on fclose() or when the process exits normally
    if (_fp_stdin == NULL)
    {
        perror("Unable to open temp file");
        exit(EXIT_FAILURE);
    }

    fflush(stdin);

    // Save stdin file descriptors
    _fd_stdin_org = fileno(stdin); // should be 0, but save in case something else redirects it
    _fd_stdin_bak = dup(_fd_stdin_org);

    // Overwrite stdin file descriptors
    assert(dup2(fileno(_fp_stdin), _fd_stdin_org) != -1);

    _is_stdin_redirected = true;
}

void iospy_unhook_in(void)
{
    if (!_is_stdin_redirected) return;

    fflush(stdin);
    if (feof(stdin)) clearerr(stdin);

    // Restore stdin file descriptors
    assert(dup2(_fd_stdin_bak, _fd_stdin_org) != -1);
    close(_fd_stdin_bak);

    _is_stdin_redirected = false;
}

static int _fd_stdout_bak, _fd_stdout_org;
static FILE *_fp_stdout;
static bool _is_stdout_redirected = false;
static fpos_t _stdout_last_pos;

FILE * iospy_get_fp_out(void)
{
    return _fp_stdout;
}

void iospy_hook_out(void)
{
    if (_is_stdout_redirected) return;

    fflush(stdout);

    _fp_stdout = tmpfile(); // This will be deleted on fclose() or when the process exits normally
    if (_fp_stdout == NULL)
    {
        perror("Unable to open temp file");
        exit(EXIT_FAILURE);
    }

    // Save stdout file descriptors
    _fd_stdout_org = fileno(stdout); // should be 1, but save in case something else redirects it
    _fd_stdout_bak = dup(_fd_stdout_org);

    // Overwrite stdout file descriptors
    assert(dup2(fileno(_fp_stdout), _fd_stdout_org) != -1);

    fgetpos(stdout, &_stdout_last_pos);

    _is_stdout_redirected = true;
}

void iospy_unhook_out(void)
{
    if (!_is_stdout_redirected) return;

    fflush(stdout);

    // Restore stdout file descriptors
    assert(dup2(_fd_stdout_bak, _fd_stdout_org) != -1);
    close(_fd_stdout_bak);

    fclose(_fp_stdout);

    _is_stdout_redirected = false;
}

void iospy_hook(void)
{
    iospy_hook_out();
    iospy_hook_in();
}

void iospy_unhook(void)
{
    iospy_unhook_in();
    iospy_unhook_out();
}

// Write to input stream
void iospy_push_in(const void * p, size_t size, size_t count)
{
    if (!_is_stdin_redirected) return;

    if (feof(stdin)) clearerr(stdin);
    size_t n = fwrite(p, size, count, _fp_stdin);
    fseek(iospy_get_fp_in(), -(long int)n, SEEK_CUR);
}

void iospy_push_in_str(const char * s)
{
    if (!_is_stdin_redirected) return;

    if (feof(stdin)) clearerr(stdin);
    size_t n = strlen(s);
    fputs(s, _fp_stdin);
    fseek(iospy_get_fp_in(), -(long int)n, SEEK_CUR);
}

// Read from output stream
size_t iospy_pop_out(void * p, size_t size, size_t count)
{
    if (!_is_stdout_redirected) return 0;

    // Rewind to last position
    fsetpos(stdout, &_stdout_last_pos);

    if (feof(_fp_stdout)) clearerr(_fp_stdout);
    size_t ret = fread(p, size, count, _fp_stdout);

    // Save position
    fgetpos(stdout, &_stdout_last_pos);
    return ret;
}

size_t iospy_pop_out_str(char * s, size_t n)
{
    if (!_is_stdout_redirected) return 0;

    // Rewind to last position
    fsetpos(stdout, &_stdout_last_pos);

    size_t ret = 0;
    if (n > 0)
    {
        if (feof(_fp_stdout)) clearerr(_fp_stdout);
        ret = fread(s, sizeof(s[0]), n - 1, _fp_stdout);
        s[ret] = '\0';

        // Save position
        fgetpos(stdout, &_stdout_last_pos);
    }
    return ret;
}