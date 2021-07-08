# Geohash encoder/decoder in C

A small, minimalistic geohash encoder and decoder library implemented in
C. It does not depend on the C standard library and is perfect for use in
embedded systems. Care has been taken to ensure results are as accurate as
possible within the limitations of IEEE 754 double precision. It's also
fast, decoding and encoding at around 15 million lat/lon pairs per second
on modern desktop/laptop hardware.

```c
// Decode a geohash buffer into a lat/lon pair. Returns 1 on success, or 0
// if the buffer does not contain a valid geohash.
int geohash_decode(double *lat, double *lon, const char *buf, int len);

// Populate the buffer with an geohash encoding of a lat/lon pair. Does
// not write a terminating null byte. Always succeeds.
void geohash_encode(char *buf, int len, double lat, double lon);
```
