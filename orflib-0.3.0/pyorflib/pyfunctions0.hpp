/** 
@file  pyfunctions0.hpp
@brief Implementation of Python callable functions
*/
#include "pyutils.hpp"
#include <orflib/defines.hpp>
#include <orflib/math/stats/errorfunction.hpp>
#include <orflib/math/stats/normaldistribution.hpp>
#include <orflib/math/interpol/piecewisepolynomial.hpp>
#include <string>

static 
PyObject*  pyOrfVersion(PyObject* pyDummy, PyObject* pyArgs)
{
PY_BEGIN;
  return asPyScalar(std::string(ORF_VERSION_STRING));
PY_END;
}

static 
PyObject* pyOrfSayHello(PyObject* pyDummy, PyObject* pyArgs)
{
PY_BEGIN;
  PyObject* pyArg1(NULL);
  if (!PyArg_ParseTuple(pyArgs, "O", &pyArg1))
    return NULL;
  std::string name = asString(pyArg1);        // read in the name
  std::string greet = "Hello " + name + "!";  // compose the greeting  
  return asPyScalar(greet);
PY_END;
}


static 
PyObject* pyOrfOuterProd(PyObject* pyDummy, PyObject* pyArgs)
{
PY_BEGIN;
  PyObject* pyArg1(NULL);
  PyObject* pyArg2(NULL);
  if (!PyArg_ParseTuple(pyArgs, "OO", &pyArg1, &pyArg2))
    return NULL;

  std::vector<double> vec1 = asDblVec(pyArg1);
  std::vector<double> vec2 = asDblVec(pyArg2);
  size_t nrow = vec1.size();
  size_t ncol = vec2.size();
  
  // allocate and compute the answer
  std::vector<std::vector<double>> outprod(nrow);
  for (size_t i = 0; i < nrow; ++i) {
	  outprod[i].resize(ncol);
	  for (size_t j = 0; j < ncol; ++j) {
		  outprod[i][j] = vec1[i] * vec2[j];
	  };
  };
	
  return asPyArray(outprod);

PY_END;
}


static
PyObject*  pyOrfErf(PyObject* pyDummy, PyObject* pyArgs)
{
PY_BEGIN;

  PyObject* pyX(NULL);
  if (!PyArg_ParseTuple(pyArgs, "O", &pyX))
    return NULL;

  double x = asDouble(pyX);
  return asPyScalar(orf::ErrorFunction::erf(x));
PY_END;
}


static
PyObject*  pyOrfInvErf(PyObject* pyDummy, PyObject* pyArgs)
{
PY_BEGIN;

  PyObject* pyX(NULL);
  if (!PyArg_ParseTuple(pyArgs, "O", &pyX))
    return NULL;

  double x = asDouble(pyX);
  return asPyScalar(orf::ErrorFunction::inverf(x));
PY_END;
}


static
PyObject*  pyOrfNormalCdf(PyObject* pyDummy, PyObject* pyArgs)
{
PY_BEGIN;

  PyObject* pyX(NULL);
  if (!PyArg_ParseTuple(pyArgs, "O", &pyX))
    return NULL;

  double x = asDouble(pyX);
  orf::NormalDistribution f;

  return asPyScalar(f.cdf(x));
PY_END;
}

static
PyObject*  pyOrfNormalInvCdf(PyObject* pyDummy, PyObject* pyArgs)
{
PY_BEGIN;

  PyObject* pyX(NULL);
  if (!PyArg_ParseTuple(pyArgs, "O", &pyX))
    return NULL;

  double x = asDouble(pyX);
  orf::NormalDistribution f;

  return asPyScalar(f.invcdf(x));
PY_END;
}

static 
PyObject*  pyOrfEchoMatrix(PyObject* pyDummy, PyObject* pyArgs)
{
PY_BEGIN;
  PyObject* pyMat(NULL);
  if (!PyArg_ParseTuple(pyArgs, "O", &pyMat))
    return NULL;

  orf::Matrix mat = asMatrix(pyMat);
  return asNumpy(mat);
PY_END;
}

static 
PyObject*  pyOrfPPolyEval(PyObject* pyDummy, PyObject* pyArgs)
{
PY_BEGIN;
  PyObject* pyBkPoints(NULL);
  PyObject* pyValues(NULL);
  PyObject* pyPolyOrder(NULL);
  PyObject* pyXVec(NULL);
  PyObject* pyDerivOrder(NULL);
  if (!PyArg_ParseTuple(pyArgs, "OOOOO", &pyBkPoints, &pyValues, &pyPolyOrder, &pyXVec, &pyDerivOrder))
    return NULL;

  orf::Vector bkpts = asVector(pyBkPoints);
  orf::Vector vals = asVector(pyValues);
  int degree = asInt(pyPolyOrder);
  orf::Vector xx = asVector(pyXVec);
  ORF_ASSERT(bkpts.size() == vals.size(), "unequal number of breakpoints and vals");
  int derivOrder = asInt(pyDerivOrder);

  // create the curve
  orf::PiecewisePolynomial pp(bkpts.begin(), bkpts.end(), vals.begin(), degree);

  orf::Vector yy(xx.size());
  pp.eval(xx.begin(), xx.end(), yy.begin(), derivOrder);

  return asNumpy(yy);
PY_END;
}

static 
PyObject*  pyOrfPPolyIntegral(PyObject* pyDummy, PyObject* pyArgs)
{
PY_BEGIN;
  PyObject* pyBkPoints(NULL);
  PyObject* pyValues(NULL);
  PyObject* pyPolyOrder(NULL);
  PyObject* pyXStart(NULL);
  PyObject* pyXVecEnd(NULL);
  if (!PyArg_ParseTuple(pyArgs, "OOOOO", &pyBkPoints, &pyValues, &pyPolyOrder, &pyXStart, &pyXVecEnd))
    return NULL;

  orf::Vector bkpts = asVector(pyBkPoints);
  orf::Vector vals = asVector(pyValues);
  ORF_ASSERT(bkpts.size() == vals.size(), "unequal number of breakpoints and vals");
  int degree = asInt(pyPolyOrder);
  double x0 = asDouble(pyXStart);
  orf::Vector xx = asVector(pyXVecEnd);
  // create the curve
  orf::PiecewisePolynomial pp(bkpts.begin(), bkpts.end(), vals.begin(), degree);
  // integrate
  orf::Vector yy(xx.size());
  pp.integral(x0, xx.begin(), xx.end(), yy.begin());

  return asNumpy(yy);
PY_END;
}
