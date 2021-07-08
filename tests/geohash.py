# Used to create the tests so that they were exact.

import decimal

def _decode(bits, interval):
    for b in bits:
        interval[b] = (interval[0] + interval[1]) / 2
    return (interval[0] + interval[1]) / 2

def decode(h):
    """Decode a geohash to an arbitrary precision."""
    i = 1
    bins = [], []
    for c in h:
        c = "0123456789bcdefghjkmnpqrstuvwxyz".index(c)
        assert c >= 0
        bins[(i+0)%2].append(c>>4 & 1)
        bins[(i+1)%2].append(c>>3 & 1)
        bins[(i+2)%2].append(c>>2 & 1)
        bins[(i+3)%2].append(c>>1 & 1)
        bins[(i+4)%2].append(c>>0 & 1)
        i += 5
    return _decode(bins[0], [decimal.Decimal( 90), decimal.Decimal( -90)]), \
           _decode(bins[1], [decimal.Decimal(180), decimal.Decimal(-180)])

def encode(lat, lon, length=12):
    """Encode a geohash, potentially even from arbitrary precision."""
    lens = [length * 5 // 2, (length * 5 + 1) // 2]
    bits = (
        int((lat +  90) / 180 * (1 << lens[0])) & ((1 << lens[0]) - 1),
        int((lon + 180) / 360 * (1 << lens[1])) & ((1 << lens[1]) - 1),
    )
    geohash = []
    for i in range(length):
        c = ((bits[(i+1)%2] >> (lens[(i+1)%2] - 1) & 1) << 4 |
             (bits[(i+2)%2] >> (lens[(i+2)%2] - 1) & 1) << 3 |
             (bits[(i+3)%2] >> (lens[(i+3)%2] - 2) & 1) << 2 |
             (bits[(i+4)%2] >> (lens[(i+4)%2] - 2) & 1) << 1 |
             (bits[(i+5)%2] >> (lens[(i+5)%2] - 3) & 1) << 0)
        lens[(i+1)%2] -= 3
        lens[(i+2)%2] -= 2
        geohash.append("0123456789bcdefghjkmnpqrstuvwxyz"[c])
    return "".join(geohash)
