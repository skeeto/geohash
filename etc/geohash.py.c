// Binary geohash module for Python 3.7+
//
// Linux build:
//   $ cc -shared $(python-config --includes) -fPIC -DNDEBUG \
//        -O3 -s -ffreestanding -nostdlib -o geohash.so geohash.py.c
//
// Windows build (w64devkit):
//   $ cc -shared -I"$PYTHONHOME/include" -L"$PYTHONHOME/libs" -DNDEBUG \
//        -O3 -s -o geohash.pyd geohash.py.c -lpython3
//
// This is free and unencumbered software released into the public domain.
#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include "../geohash.c"

static PyObject *
encode(PyObject *self, PyObject **args, Py_ssize_t nargs)
{
    (void)self;

    double lat, lon;
    if (nargs != 2) {
        PyErr_SetString(PyExc_TypeError, "takes exactly two arguments");
        return 0;
    }
    lat = PyFloat_AsDouble(args[0]);
    if (PyErr_Occurred()) {
        return 0;
    }
    lon = PyFloat_AsDouble(args[1]);
    if (PyErr_Occurred()) {
        return 0;
    }

    if (!(lat >= -90.0 && lat < 90.0)) {
        PyErr_SetString(PyExc_ValueError, "latitude must be in [-90, 90)");
        return 0;
    }
    if (!(lon >= -180.0 && lon < 180.0)) {
        PyErr_SetString(PyExc_ValueError, "longitude must be in [-180, 180)");
        return 0;
    }

    char buf[21];
    geohash_encode(buf, lat, lon);
    return Py_BuildValue("s#", buf, (int)sizeof(buf));
}

static PyObject *
decode(PyObject *self, PyObject **args, Py_ssize_t nargs)
{
    (void)self;
    (void)nargs;

    if (nargs != 1) {
        PyErr_SetString(PyExc_TypeError, "takes exactly one argument");
        return 0;
    }

    int len;
    char tmp[22];
    char *buf = tmp;

    if (Py_TYPE(args[0]) == &PyUnicode_Type) {
        Py_ssize_t slen;
        wchar_t *u = PyUnicode_AsWideCharString(args[0], &slen);
        len = slen > (Py_ssize_t)sizeof(tmp) ? (int)sizeof(tmp) : (int)slen;
        for (int i = 0; i < len; i++) {
            tmp[i] = u[i] > 127 ? 0 : u[i];
        }

    } else if (Py_TYPE(args[0]) == &PyBytes_Type) {
        Py_ssize_t slen = PyBytes_GET_SIZE(args[0]);
        buf = PyBytes_AsString(args[0]);
        len = slen > (Py_ssize_t)sizeof(buf) ? (int)sizeof(buf) : (int)slen;

    } else {
        PyErr_SetString(PyExc_TypeError, "must be bytes or str");
        return 0;
    }

    double lat, lon;
    if (!geohash_decode(&lat, &lon, buf, len)) {
        PyErr_SetString(PyExc_ValueError, "invalid geohash");
        return 0;
    }
    return Py_BuildValue("ff", lat, lon);
}

static PyMethodDef methods[] = {
    {"encode", (PyCFunction)encode, METH_FASTCALL, "Encodes a geohash"},
    {"decode", (PyCFunction)decode, METH_FASTCALL, "Decodes a geohash"},
    {0, 0, 0, 0}
};

static struct PyModuleDef module = {
    PyModuleDef_HEAD_INIT,
    .m_name = "geohash",
    .m_doc = "Encoder and decoder for geohash",
    .m_methods = methods,
};

PyMODINIT_FUNC
PyInit_geohash(void)
{
    return PyModule_Create(&module);
}

#if defined(_WIN32) && __STDC_HOSTED__ == 0
int DllMainCRTStartup(void) { return 1; }
#endif
