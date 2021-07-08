// This is free and unencumbered software released into the public domain.
#ifndef GEOHASH_H
#define GEOHASH_H

// Decode a geohash buffer into a lat/lon pair. Returns 1 on success, or 0
// if the buffer does not contain a valid geohash.
int geohash_decode(double *lat, double *lon, const char *buf, int len);

// Populate the buffer with an geohash encoding of a lat/lon pair. Does not
// write a terminating null byte. Always succeeds.
void geohash_encode(char *buf, int len, double lat, double lon);

#endif
