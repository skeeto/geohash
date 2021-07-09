// This is free and unencumbered software released into the public domain.
#ifndef GEOHASH_H
#define GEOHASH_H

// Validate and decode a geohash into a lat/lon pair. Returns 1 on success,
// or 0 if the buffer does not contain a valid geohash.
int geohash_decode(double *lat, double *lon, const char *buf, int len);

// Store a 21-byte geohash encoding of a lat/lon pair. Does not write a
// terminating null byte. Truncate as needed.
void geohash_encode(char *buf, double lat, double lon);

#endif
