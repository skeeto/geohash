#define _POSIX_C_SOURCE 200112L
#include <math.h>
#include <stdio.h>
#include <time.h>
#include "../geohash.h"

static double
now(void)
{
    struct timespec tv[1];
    clock_gettime(CLOCK_MONOTONIC, tv);
    return tv->tv_sec + tv->tv_nsec/1e9;
}

static double
benchmark_encode(int n)
{
    char buf[21] = {0};
    unsigned tally = 0;

    double beg = now();
    for (long i = 0; i < 1<<n; i++) {
        unsigned long long a = i ^ 0x313198a2e0370734;
        a *= 0x3243f6a8885a308d;
        a ^= a >> 32;
        double lat = ldexp(a>>32, -32)*180 - 90;
        double lon = ldexp(a&0xffffffff, -32)*360 - 180;

        geohash_encode(buf, lat, lon);
        tally += buf[11];
    }
    volatile unsigned sink = tally; (void)sink;
    return (1<<n) / (now() - beg);
}

static double
benchmark_decode(int n)
{
    static const char b32[32] = "0123456789bcdefghjkmnpqrstuvwxyz";
    char buf[13] = {0};
    double tally = 0;

    double beg = now();
    for (long i = 0; i < 1<<n; i++) {
        unsigned long long a = i ^ 0x313198a2e0370734;
        a *= 0x3243f6a8885a308d;
        a ^= a >> 32;
        buf[ 0] = b32[a >>  0 & 0x1f];
        buf[ 1] = b32[a >>  5 & 0x1f];
        buf[ 2] = b32[a >> 10 & 0x1f];
        buf[ 3] = b32[a >> 15 & 0x1f];
        buf[ 4] = b32[a >> 20 & 0x1f];
        buf[ 5] = b32[a >> 25 & 0x1f];
        buf[ 6] = b32[a >> 30 & 0x1f];
        buf[ 7] = b32[a >> 35 & 0x1f];
        buf[ 8] = b32[a >> 40 & 0x1f];
        buf[ 9] = b32[a >> 45 & 0x1f];
        buf[10] = b32[a >> 50 & 0x1f];
        buf[11] = b32[a >> 55 & 0x1f];

        double lat, lon;
        geohash_decode(&lat, &lon, buf, 12);
        tally += 2*lat - lon;
    }
    volatile double sink = tally; (void)sink;
    return (1<<n) / (now() - beg);
}

int
main(void)
{
    printf("encode %.3f M-ops/sec\n", benchmark_encode(26)/1e6);
    printf("decode %.3f M-ops/sec\n", benchmark_decode(25)/1e6);
}
