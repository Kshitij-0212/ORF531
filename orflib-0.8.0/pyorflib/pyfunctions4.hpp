/**
@file  pyfunctions4.hpp
@brief Implementation of Python callable functions
*/
#include <pyorflib/pyutils.hpp>

#include <orflib/market/market.hpp>
#include <orflib/products/europeancallput.hpp>
#include <orflib/products/digitalcallput.hpp>
#include <orflib/methods/pde/pde1dsolver.hpp>

using namespace std;

static
PyObject*  pyOrfEuroBSPDE(PyObject* pyDummy, PyObject* pyArgs)
{
PY_BEGIN;

  PyObject* pyPayoffType(NULL);
  PyObject* pySpot(NULL);
  PyObject* pyStrike(NULL);
  PyObject* pyTimeToExp(NULL);
  PyObject* pyDiscountCrv(NULL);
  PyObject* pyDivYield(NULL);
  PyObject* pyVolatility(NULL);
  PyObject* pyPdeParams(NULL);
  PyObject* pyAllResults(NULL);

  if (!PyArg_ParseTuple(pyArgs, "OOOOOOOOO", &pyPayoffType, &pyStrike, &pyTimeToExp, 
    &pySpot, &pyDiscountCrv, &pyDivYield, &pyVolatility, &pyPdeParams, &pyAllResults))
    return NULL;

  int payoffType = asInt(pyPayoffType);
  double spot = asDouble(pySpot);
  double strike = asDouble(pyStrike);
  double timeToExp = asDouble(pyTimeToExp);

  std::string name = asString(pyDiscountCrv);
  orf::SPtrYieldCurve spyc = orf::market().yieldCurves().get(name);
  ORF_ASSERT(spyc, "error: yield curve " + name + " not found");

  double divYield = asDouble(pyDivYield);

  orf::SPtrVolatilityTermStructure spvol;
  if (PyFloat_Check(pyVolatility) || PyLong_Check(pyVolatility)) {
    // Numerical value: create a constant volatility term structure on the fly
    double vol = asDouble(pyVolatility);
    std::vector<double> times = {timeToExp};
    std::vector<double> vols = {vol};
    spvol = std::make_shared<orf::VolatilityTermStructure>(
      times.begin(), times.end(), vols.begin(), vols.end(), 
      orf::VolatilityTermStructure::VolType::SPOTVOL);
  } else {
    // Object handle: get volatility term structure from market
    std::string volname = asString(pyVolatility);
    spvol = orf::market().volatilities().get(volname);
    ORF_ASSERT(spvol, "error: volatility term structure " + volname + " not found");
  }

  // read the PDE parameters
  orf::PdeParams pdeparams = asPdeParams(pyPdeParams);
  // read the allresults flag
  bool allresults = asBool(pyAllResults);

  // create the product
  orf::SPtrProduct spprod(new orf::EuropeanCallPut(payoffType, strike, timeToExp));
  // create the PDE solver
  orf::Pde1DResults results;
  orf::Pde1DSolver solver(spprod, spyc, spot, divYield, spvol, results);
  solver.solve(pdeparams);

  // write results
  PyObject* ret = PyDict_New();
  int ok = PyDict_SetItem(ret, asPyScalar("Price"), asPyScalar(results.prices[0]));

  if (allresults) {
    orf::Vector spots;
    results.getSpotAxis(0, spots);
    orf::Matrix values(results.times.size(), results.values.front().size());
    for (size_t i = 0; i < results.times.size(); ++i)
      for (size_t j = 0; j < results.values.front().size(); ++j)
        values(i, j) = results.values[i](j, 0);

    PyDict_SetItem(ret, asPyScalar("Times"), asNumpy(results.times));
    PyDict_SetItem(ret, asPyScalar("Spots"), asNumpy(spots));
    PyDict_SetItem(ret, asPyScalar("Values"), asNumpy(values));
  }
  return ret;

PY_END;
}


static
PyObject*  pyOrfDigiBSPDE(PyObject* pyDummy, PyObject* pyArgs)
{
PY_BEGIN;

  PyObject* pyPayoffType(NULL);
  PyObject* pySpot(NULL);
  PyObject* pyStrike(NULL);
  PyObject* pyTimeToExp(NULL);
  PyObject* pyDiscountCrv(NULL);
  PyObject* pyDivYield(NULL);
  PyObject* pyVolatility(NULL);
  PyObject* pyPdeParams(NULL);
  PyObject* pyAllResults(NULL);

  if (!PyArg_ParseTuple(pyArgs, "OOOOOOOOO", &pyPayoffType, &pyStrike, &pyTimeToExp, 
    &pySpot, &pyDiscountCrv, &pyDivYield, &pyVolatility, &pyPdeParams, &pyAllResults))
    return NULL;

  int payoffType = asInt(pyPayoffType);
  double spot = asDouble(pySpot);
  double strike = asDouble(pyStrike);
  double timeToExp = asDouble(pyTimeToExp);

  std::string name = asString(pyDiscountCrv);
  orf::SPtrYieldCurve spyc = orf::market().yieldCurves().get(name);
  ORF_ASSERT(spyc, "error: yield curve " + name + " not found");

  double divYield = asDouble(pyDivYield);

  orf::SPtrVolatilityTermStructure spvol;
  if (PyFloat_Check(pyVolatility) || PyLong_Check(pyVolatility)) {
    // Numerical value: create a constant volatility term structure on the fly
    double vol = asDouble(pyVolatility);
    std::vector<double> times = {timeToExp};
    std::vector<double> vols = {vol};
    spvol = std::make_shared<orf::VolatilityTermStructure>(
      times.begin(), times.end(), vols.begin(), vols.end(), 
      orf::VolatilityTermStructure::VolType::SPOTVOL);
  } else {
    // Object handle: get volatility term structure from market
    std::string volname = asString(pyVolatility);
    spvol = orf::market().volatilities().get(volname);
    ORF_ASSERT(spvol, "error: volatility term structure " + volname + " not found");
  }

  // read the PDE parameters
  orf::PdeParams pdeparams = asPdeParams(pyPdeParams);
  // read the allresults flag
  bool allresults = asBool(pyAllResults);

  // create the product
  orf::SPtrProduct spprod(new orf::DigitalCallPut(payoffType, strike, timeToExp));
  // create the PDE solver
  orf::Pde1DResults results;
  orf::Pde1DSolver solver(spprod, spyc, spot, divYield, spvol, results);
  solver.solve(pdeparams);

  // write results
  PyObject* ret = PyDict_New();
  int ok = PyDict_SetItem(ret, asPyScalar("Price"), asPyScalar(results.prices[0]));

  if (allresults) {
    orf::Vector spots;
    results.getSpotAxis(0, spots);
    orf::Matrix values(results.times.size(), results.values.front().size());
    for (size_t i = 0; i < results.times.size(); ++i)
      for (size_t j = 0; j < results.values.front().size(); ++j)
        values(i, j) = results.values[i](j, 0);

    PyDict_SetItem(ret, asPyScalar("Times"), asNumpy(results.times));
    PyDict_SetItem(ret, asPyScalar("Spots"), asNumpy(spots));
    PyDict_SetItem(ret, asPyScalar("Values"), asNumpy(values));
  }
  return ret;

PY_END;
}