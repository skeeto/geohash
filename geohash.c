// This is free and unencumbered software released into the public domain.
#include "geohash.h"

int
geohash_decode(double *lat, double *lon, const char *buf, int len)
{
    static const signed char value[256] = {
        -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
        -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
        -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
        +0, +1, +2, +3, +4, +5, +6, +7, +8, +9, -1, -1, -1, -1, -1, -1,
        -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
        -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
        -1, -1, 10, 11, 12, 13, 14, 15, 16, -1, 17, 18, -1, 19, 20, -1,
        21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, -1, -1, -1, -1, -1,
        -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
        -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
        -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
        -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
        -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
        -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
        -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
        -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    };
    unsigned long long bits[2] = {0, 0};
    int lens[2] = {0, 0};

    len = len > 21 ? 21 : len;  // any further is pointless
    for (int i = 0; i < len; i++) {
        int a = i & 1;
        int b = !a;
        int c = value[buf[i]&0xff];
        if (c < 0) {
            return 0;
        }
        // deinterleave bits
        bits[b] = bits[b]<<1 | (c>>4 & 1);
        bits[a] = bits[a]<<1 | (c>>3 & 1);
        bits[b] = bits[b]<<1 | (c>>2 & 1);
        bits[a] = bits[a]<<1 | (c>>1 & 1);
        bits[b] = bits[b]<<1 | (c>>0 & 1);
        lens[b] += 3;
        lens[a] += 2;
    }

    // Note: Floating point operation order has been carefully arranged to
    // minimize rounding errors. Re-ordering (or using -ffast-math) will
    // cause tests to fail.
    double lat0 = (double)(bits[0] + 0) / (1ULL << lens[0]);
    double lat1 = (double)(bits[0] + 1) / (1ULL << lens[0]);
    *lat = (lat0 + lat1 - 1) * 90;
    double lon0 = (double)(bits[1] + 0) / (1ULL << lens[1]);
    double lon1 = (double)(bits[1] + 1) / (1ULL << lens[1]);
    *lon = (lon0 + lon1 - 1) * 180;

    return 1;
}

void
geohash_encode(char *buf, int len, double lat, double lon)
{
    len = len > 21 ? 21 : len;
    int lens[] = {len * 5 / 2, (len * 5 + 1) / 2};
    unsigned long long bits[] = {
        (lat +  90) / 180 * (1ULL << lens[0]),
        (lon + 180) / 360 * (1ULL << lens[1]),
    };

    for (int i = 0; i < len; i++) {
        int a = i & 1;
        int b = !a;
        int c = 0;
        // interleave bits
        c |= (bits[b] >> --lens[b] & 1) << 4;
        c |= (bits[a] >> --lens[a] & 1) << 3;
        c |= (bits[b] >> --lens[b] & 1) << 2;
        c |= (bits[a] >> --lens[a] & 1) << 1;
        c |= (bits[b] >> --lens[b] & 1) << 0;
        buf[i] = "0123456789bcdefghjkmnpqrstuvwxyz"[c];
    }
}
