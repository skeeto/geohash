# Geohash encoder/decoder in C

A lean, efficient geohash encoder and decoder library implemented in C. It
does not depend on the C standard library and is well-suited for use in
embedded systems. Care has been taken to ensure results are as accurate as
possible within the limitations of IEEE 754 double precision. It's also
very fast, decoding and encoding ~50 million latlons per second on modern
desktop and laptop hardware, making it likely the fastest geohash decoder
and encoder currently available.

```c
// Validate and decode a geohash into a lat/lon pair. Returns 1 on success,
// or 0 if the buffer does not contain a valid geohash.
int geohash_decode(double *lat, double *lon, const char *buf, int len);

// Store a 21-byte geohash encoding of a lat/lon pair. Does not write a
// terminating null byte. Truncate as needed.
void geohash_encode(char *buf, double lat, double lon);
```

## Examples

This program decodes geohashes from command line arguments.

```c
#include <math.h>
#include <stdio.h>
#include <string.h>
#include "geohash.h"

static struct hms {short h, m; int ms;}
convert(double a)
{
    double f  = fabs(fmod(a, 1.0));
    int    h  = fabs(a);
    int    m  = f*60;
    int    ms = (f*3600 - m*60) * 1000;
    return (struct hms){h, m, ms};
}

int main(int argc, char *argv[])
{
    for (int i = 1; i < argc; i++) {
        double lat, lon;
        size_t len = strlen(argv[i]);
        int r = geohash_decode(&lat, &lon, argv[i], len > 14 ? 14 : len);
        if (!r) {
            fprintf(stderr, "%s: invalid geohash, %s\n", argv[0], argv[i]);
            return 1;
        }

        struct hms ns = convert(lat);
        struct hms ew = convert(lon);
        printf("%-15.14s%2d°%3d'%3d.%03d\" %c%5d°%3d'%3d.%03d\" %c\n", argv[i],
               ns.h, ns.m, ns.ms/1000, ns.ms%1000, "NS"[lat<0],
               ew.h, ew.m, ew.ms/1000, ew.ms%1000, "EW"[lon<0]);
    }
}
```

Usage example:

    $ ./example ezs42 rgv11mqxe8cc kxtsr7j1fn3qm9 00000000000000 zzzzzzzzzzzzzz
    ezs42          42° 36' 17.929" N    5° 36' 10.898" W
    rgv11mqxe8cc   23° 42'  1.216" S  175° 50' 42.938" E
    kxtsr7j1fn3qm9  2°  2' 55.014" S   30° 33' 35.757" E
    00000000000000 89° 59' 59.999" S  179° 59' 59.999" W
    zzzzzzzzzzzzzz 89° 59' 59.999" N  179° 59' 59.999" E
