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

// Store a 20-byte geohash encoding of a lat/lon pair. Does not write a
// terminating null byte. Truncate as needed.
void geohash_encode(char *buf, double lat, double lon);
```
