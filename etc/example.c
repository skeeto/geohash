#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../geohash.h"

struct hms { short h, m; unsigned short ms; };

static struct hms
hms_unpack(double a)
{
    double f  = fabs(fmod(a, 1.0));
    int    h  = a;
    int    m  = f*60;
    int    ms = (f*3600 - m*60) * 1000;
    return (struct hms){h, m, ms};
}

static double
hms_pack(struct hms v)
{
    int sign = v.h < 0 ? -1 : +1;
    return sign * (labs(v.h) + v.m/60.0 + (v.ms/1000.0)/3600.0);
}

static int
hms_parse(struct hms *lat, struct hms *lon, const char *s)
{
    unsigned lath, latm, lats;
    unsigned lonh, lonm, lons;
    char latf[4], lonf[4];
    int r, latsign, lonsign;
    char ns, ew;

    r = sscanf(s, "%u@%u'%d.%3[0-9]\" %c %u@%u'%u.%3[0-9]\" %c",
               &lath, &latm, &lats, latf, &ns,
               &lonh, &lonm, &lons, lonf, &ew);
    if (r != 10) {
        return 0;
    }
    if (lath>89 || latm>59 || lats>59 || strlen(latf) < 3) {
        return 0;
    }
    if (lonh>179 || lonm>59 || lons>59 || strlen(lonf) < 3) {
        return 0;
    }

    switch (ns) {
    case 'N': latsign = +1; break;
    case 'S': latsign = -1; break;
    default: return 0;
    }
    switch (ew) {
    case 'E': lonsign = +1; break;
    case 'W': lonsign = -1; break;
    default: return 0;
    }

    lat->h  = latsign*lath;
    lat->m  = latm;
    lat->ms = lats*1000 + atoi(latf);
    lon->h  = lonsign*lonh;
    lon->m  = lonm;
    lon->ms = lons*1000 + atoi(lonf);
    return 1;
}

static void
hms_print(char *buf, struct hms lat, struct hms lon)
{
    sprintf(buf, "%2d@%3d'%3d.%03d\" %c%5d@%3d'%3d.%03d\" %c", // 38 bytes
            (int)labs(lat.h), lat.m, lat.ms/1000, lat.ms%1000, "NS"[lat.h<0],
            (int)labs(lon.h), lon.m, lon.ms/1000, lon.ms%1000, "EW"[lon.h<0]);
}

static void
usage(FILE *f, const char *name)
{
    fprintf(f, "usage: %s <-d|-e> [latlon...]\n", name);
    fprintf(f, "examples:\n");
    fprintf(f, "    $ %s -d dppn59uz86jzd\n", name);
    fprintf(f, "    $ %s -e \"40@26'26.160\\\"N 79@59'45.239\\\"W\"\n", name);
}

static int
decode(int argc, char *argv[])
{
    for (int i = 2; i < argc; i++) {
        double latlon[2];
        size_t len = strlen(argv[i]);
        int r = geohash_decode(latlon, latlon+1, argv[i], len > 14 ? 14 : len);
        if (!r) {
            fprintf(stderr, "%s: invalid geohash, %s\n", argv[0], argv[i]);
            return 1;
        }

        char repr[64];
        struct hms lat = hms_unpack(latlon[0]);
        struct hms lon = hms_unpack(latlon[1]);
        hms_print(repr, lat, lon);
        printf("%-15.14s%s\n", argv[i], repr);
    }
    return 0;
}

static int
encode(int argc, char *argv[])
{
    for (int i = 2; i < argc; i++) {
        struct hms lat, lon;
        if (!hms_parse(&lat, &lon, argv[i])) {
            fprintf(stderr, "%s: invalid lat/lon, %s\n", argv[0], argv[i]);
            return 1;
        }

        char target[64];
        hms_print(target, lat, lon);

        char buf[32];
        double latlon[] = { hms_pack(lat), hms_pack(lon) };
        geohash_encode(buf, latlon[0], latlon[1]);

        // Find shortest geohash with the same printout
        int best = 14;
        for (int n = 13; n >= 0; n--) {
            char repr[64];
            geohash_decode(latlon, latlon+1, buf, n);
            lat = hms_unpack(latlon[0]);
            lon = hms_unpack(latlon[1]);
            hms_print(repr, lat, lon);
            if (!strcmp(target, repr)) {
                best = n;
            }
        }
        buf[best] = 0;

        printf("%-15.14s%s\n", buf, target);
    }
    return 0;
}

int
main(int argc, char *argv[])
{
    if (argc < 2 || argv[1][0] != '-') {
        usage(stderr, argv[0]);
        return 1;
    }
    switch (argv[1][1]) {
    case 'd': return decode(argc, argv);
    case 'e': return encode(argc, argv);
    case 'h': usage(stdout, argv[0]); return 0;
    default:  usage(stderr, argv[0]); return 2;
    }
}
