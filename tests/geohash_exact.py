# Used to create the tests so that they were exact.
from fractions import Fraction

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
    return 1.0 * _decode(bins[0], [Fraction( 90), Fraction( -90)]), \
           1.0 * _decode(bins[1], [Fraction(180), Fraction(-180)])
