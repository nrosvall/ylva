/*
 * Copyright (C) 2019-2020 Niko Rosvall <niko@byteptr.com>
 */

#define _XOPEN_SOURCE 700

#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <string.h>
#include <openssl/err.h>
#include <openssl/rand.h>
#include "utils.h"

/*Generates random number between 0 and max.
 *Function should generate uniform distribution.
 */
static unsigned int rand_between(unsigned int min, unsigned int max)
{
    uint32_t r;
    const unsigned int range = 1 + max - min;
    const unsigned int buckets = RAND_MAX / range;
    const unsigned int limit = buckets * range;

    /*Create equal size buckets all in a row, then fire randomly towards
     *the buckets until you land in one of them. All buckets are equally
     *likely. If you land off the end of the line of buckets, try again.
     */
    do
    {
        if(RAND_bytes((unsigned char *)&r, sizeof r) == 0)
        {
            ERR_print_errors_fp(stderr);
            abort();
        }

    } while (r >= limit);

    return min + (r / buckets);
}

/* Simply generate secure password
 * and output it to the stdout. Uses OpenSSL RAND_bytes.
 *
 * Caller must free the return value.
 */
char *generate_password(int length)
{
    if(length < 1 || length > RAND_MAX)
        return NULL;

    char *pass = NULL;
    char *alpha = "abcdefghijklmnopqrstuvwxyz" \
    "ABCDEFGHIJKLMNOPQRSTUVWXYZ" \
    "0123456789?)(/%#!?)=";
    unsigned int max;
    unsigned int number;

    RAND_poll();

    if(RAND_status() != 1)
        fprintf(stdout, "Warning, random number generator not seeded.\n");

    max = strlen(alpha) - 1;
    pass = tmalloc((length + 1) * sizeof(char));

    for(int j = 0; j < length; j++)
    {
        number = rand_between(0, max);
        pass[j] = alpha[number];
    }

    pass[length] = '\0';

    fprintf(stdout, "%s\n", pass);

    return pass;
}
