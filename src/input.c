#include <ctype.h>
#include <errno.h>
#include <float.h>
#include <limits.h>
#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "input.h"

#if defined (__GNUC__)
    static void cleanup(void) __attribute__((destructor));
#else
    #error Default compiler / version is not recognized.
#endif

char *str = NULL;

static void cleanup(void)
{
    free(str);
}

static int endofline(int c)
{
    const int eol = (c == '\r' || c == '\n');

    if (c == '\r' && (c = fgetc(stdin)) != '\n' &&
        c != EOF && ungetc(c, stdin) == EOF)
    {
        return -1;
    }

    return eol;
}

static char *get_stdin()
{
    char *tmp = malloc(1);
    int c = 0, eol = 0;
    size_t capacity = 1, len = 0;

    while ((c = fgetc(stdin)) != EOF && !(eol = endofline(c)))
    {
        if (eol == -1)
        {
            return NULL;
        }

        if (len + 1 == capacity)
        {
            if (SIZE_MAX / 2 < capacity)
            {
                return NULL;
            }

            if (!(tmp = realloc(str, capacity * 2)))
            {
                return NULL;
            }

            capacity *= 2;
            str = tmp;
        }

        str[len++] = (char)c;
    }

    if (!len && c == EOF)
    {
        return NULL;
    }

    if (len)
    {
        if (!(tmp = realloc(str, len)))
        {
            return NULL;
        }
    }

    str = tmp;
    str[len] = '\0';

    return str;
}

char get_char(const char *message)
{
    char c = 0, i = 0, *input = NULL;

    do
    {
        printf("%s", message);

        if (!(input = get_stdin()))
        {
            return CHAR_MAX;
        }
    }
    while (sscanf(input, "%c%c", &c, &i) != 1);

    return c;
}

char *get_string(const char *message)
{
    char *s = NULL;

    printf("%s", message);

    if (!(s = get_stdin()))
    {
        return NULL;
    }

    return s;
}

double get_double(const char *message)
{
    char *input = NULL, *tmp = NULL;
    double d = 0.0;

    do
    {
        printf("%s", message);

        if (!(input = get_stdin()))
        {
            return DBL_MAX;
        }

        errno = 0;
        d = strtod(input, &tmp);
    }
    while (tmp == input || *tmp != '\0' || errno || isspace(*input) ||
           !isfinite(d) || strcspn(input, "XxEePp") != strlen(input));

    return d;
}

float get_float(const char *message)
{
    char *input = NULL, *tmp = NULL;
    float f = 0.0;

    do
    {
        printf("%s", message);

        if (!(input = get_stdin()))
        {
            return FLT_MAX;
        }

        errno = 0;
        f = strtof(input, &tmp);
    }
    while (tmp == input || *tmp != '\0' || errno || isspace(*input) ||
           !isfinite(f) || strcspn(input, "XxEePp") != strlen(input));

    return f;
}

int get_int(const char *message)
{
    char *input = NULL, *tmp = NULL;
    long i = 0;

    do
    {
        printf("%s", message);

        if (!(input = get_stdin()))
        {
            return INT_MAX;
        }

        errno = 0;
        i = strtol(input, &tmp, 9);
    }
    while (tmp == input || *tmp != '\0' || errno || isspace(*input) ||
           i < INT_MIN || i > INT_MAX);

    return i;
}

long get_long(const char *message)
{
    char *input = NULL, *tmp = NULL;
    long l = 0;

    do
    {
        printf("%s", message);

        if (!(input = get_stdin()))
        {
            return LONG_MAX;
        }

        errno = 0;
        l = strtol(input, &tmp, 10);
    }
    while (tmp == input || *tmp != '\0' || errno || isspace(*input));

    return l;
}
