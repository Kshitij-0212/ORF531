/**
@file  pymodule.cpp
@brief Python callable function registration and module definition
*/

#include <orflib/defines.hpp>
#include "pytestfunc.hpp"
#include "pyfunctions0.hpp"
#include "pyfunctions1.hpp"

static PyMethodDef PyOrflibMethods[] = 
{
// test functions
  { "__echoBool__", echoBool, METH_VARARGS, "echoes a boolean" },
  { "__echoInt__", echoInt, METH_VARARGS, "echoes an integer" },
  { "__echoDouble__", echoDouble, METH_VARARGS, "echoes a double" },
  { "__echoString__", echoString, METH_VARARGS, "echoes a string" },
  { "__echoIntList__", echoIntList, METH_VARARGS, "echoes a list of integers" },
  { "__echoDblList__", echoDblList, METH_VARARGS, "echoes a list of doubles" },
  { "__echoStrList__", echoStrList, METH_VARARGS, "echoes a list of strings" },
  { "__echoIntArray__", echoIntArray, METH_VARARGS, "echoes an 1D array of integers" },
  { "__echoDblArray__", echoDblArray, METH_VARARGS, "echoes an 1D array of doubles" },
  { "__echoStrArray__", echoStrArray, METH_VARARGS, "echoes an 1D array of strings" },
  { "__echoDblArray2D__", echoDblArray2D, METH_VARARGS, "echoes a 2D array of doubles" },
  { "__echoDblVecField__", echoDblVecField, METH_VARARGS, "echoes an 1D array of doubnles in an object field" },
// functions 0
  { "version", pyOrfVersion, METH_VARARGS, "orflib library version"},
  { "sayHello", pyOrfSayHello, METH_VARARGS, "says hello"},
  { "outerProd", pyOrfOuterProd, METH_VARARGS, "outproduct of two vectors"},
  { "erf", pyOrfErf, METH_VARARGS, "the value of the error function." },
  { "invErf", pyOrfInvErf, METH_VARARGS, "the value of the inverse error function." },
  { "normalCdf", pyOrfNormalCdf, METH_VARARGS, "the value of the standard normal cumulative distribution." },
  { "normalInvCdf", pyOrfNormalInvCdf, METH_VARARGS, "the value of the inverse standard normal cumulative distribution." },
  { "echoMatrix", pyOrfEchoMatrix, METH_VARARGS, "echoes a matrix" },
  { "ppolyEval", pyOrfPPolyEval, METH_VARARGS, "evaluation of derivatives of a piecewise polynomial curve" },
  { "ppolyIntegral", pyOrfPPolyIntegral, METH_VARARGS, "evaluation of integral of a piecewise polynomial curve" },
// functions 1
  { "fwdPrice", pyOrfFwdPrice, METH_VARARGS, "the forward price of an asset" },
  { "digiBS", pyOrfDigiBS, METH_VARARGS, "price of a digital option in the Black-Scholes model." },
  { "euroBS", pyOrfEuroBS, METH_VARARGS, "price of a European option in the Black-Scholes model." },
  {NULL, NULL, 0, NULL}
};

// Definition of module
static struct PyModuleDef modef =
{
  PyModuleDef_HEAD_INIT,
  "orflib",
  "Python interface to the orflib library",
  -1,
  PyOrflibMethods
};

// Creation of module
PyMODINIT_FUNC
PyInit_pyorflib(void)
{
  PyObject* m = PyModule_Create(&modef);
  if (m == NULL)
    return NULL;
  import_array();
  return m;
}
