import ctypes

# cc -shared -fPIC -O3 -s -o libgeohash.so geohash.c
_SO = ctypes.CDLL("./libgeohash.so")
_SO.geohash_encode.argtypes = (
    ctypes.c_char_p,
    ctypes.c_double,
    ctypes.c_double,
)
_SO.geohash_decode.restype = ctypes.c_int
_SO.geohash_decode.argtypes = (
    ctypes.POINTER(ctypes.c_double),
    ctypes.POINTER(ctypes.c_double),
    ctypes.c_char_p,
    ctypes.c_int
)

def encode(lat, lon):
    if not (-90 <= lat < 90):
        raise ValueError("latitude must be in [90, 90)")
    if not (-180 <= lon < 180):
        raise ValueError("longitude must be in [180, 180)")
    buf = ctypes.create_string_buffer(21)
    _SO.geohash_encode(buf, lat, lon)
    return buf.value.decode()

def decode(geohash):
    lat, lon = ctypes.c_double(), ctypes.c_double()
    encoded = geohash.encode()
    r = _SO.geohash_decode(
        ctypes.byref(lat), ctypes.byref(lon),
        encoded, len(encoded),
    )
    if r != 1:
        raise ValueError("invalid geohash")
    return lat.value, lon.value
