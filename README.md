# Geohash encoder/decoder in C

A lean, efficient, accurate [geohash][] encoder and decoder library
implemented in C. It does not depend on the C standard library and is
well-suited for use in embedded systems. Care has been taken to ensure
results are as accurate as possible within the limitations of IEEE 754
double precision. It's also very fast, decoding ~50 million and encoding
~80 million latlons per second on modern desktop and laptop hardware.

This is the fastest, most correct, and most accurate (to double precision)
geohash decoder and encoder currently available.

```c
// Validate and decode a geohash into a lat/lon pair. Returns 1 on success,
// or 0 if the buffer does not contain a valid geohash.
int geohash_decode(double *lat, double *lon, const char *buf, int len);

// Store a 21-byte geohash encoding of a lat/lon pair. Does not write a
// terminating null byte. Truncate as needed.
void geohash_encode(char *buf, double lat, double lon);
```

## Example usage

The source file `tests/example.c` demonstrates decoding and encoding
lat/lon coordinates as command line arguments.

    $ ./example -d ezs42 dppn59uz86jzd 000000000000 zzzzzzzzzzzz
    ezs42          42@ 36' 17.929" N    5@ 36' 10.898" W
    dppn59uz86jzd  40@ 26' 26.160" N   79@ 59' 45.239" W
    000000000000   89@ 59' 59.999" S  179@ 59' 59.999" W
    zzzzzzzzzzzz   89@ 59' 59.999" N  179@ 59' 59.999" E

    $ ./example -e "40@26'26.160\"N 79@59'45.239\"W"
    dppn59uz86jzd  40@ 26' 26.160" N   79@ 59' 45.239" W


[geohash]: https://en.wikipedia.org/wiki/Geohash
