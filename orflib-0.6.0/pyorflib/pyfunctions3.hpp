/**
@file  pyfunctions3.hpp
@brief Implementation of Python callable functions
*/
#include <pyorflib/pyutils.hpp>

#include <orflib/defines.hpp>
#include <orflib/products/europeancallput.hpp>
#include <orflib/products/digitalcallput.hpp>
#include <orflib/pricers/bsmcpricer.hpp>
#include <orflib/math/stats/meanvarcalculator.hpp>
#include <orflib/math/random/rng.hpp>


static
PyObject*  pyOrfEuroBSMC(PyObject* pyDummy, PyObject* pyArgs)
{
PY_BEGIN;

  PyObject* pyPayoffType(NULL);
  PyObject* pySpot(NULL);
  PyObject* pyStrike(NULL);
  PyObject* pyTimeToExp(NULL);
  PyObject* pyDiscountCrv(NULL);
  PyObject* pyDivYield(NULL);
  PyObject* pyVolatility(NULL);
  PyObject* pyMcParams(NULL);
  PyObject* pyNPaths(NULL);

  if (!PyArg_ParseTuple(pyArgs, "OOOOOOOOO", &pyPayoffType, &pyStrike, &pyTimeToExp, 
    &pySpot, &pyDiscountCrv, &pyDivYield, &pyVolatility, &pyMcParams, &pyNPaths))
    return NULL;

  int payoffType = asInt(pyPayoffType);
  double spot = asDouble(pySpot);
  double strike = asDouble(pyStrike);
  double timeToExp = asDouble(pyTimeToExp);

  std::string name = asString(pyDiscountCrv);
  orf::SPtrYieldCurve spyc = orf::market().yieldCurves().get(name);
  ORF_ASSERT(spyc, "error: yield curve " + name + " not found");

  double divYield = asDouble(pyDivYield);
  
  // Handle volatility: either a numerical value or an object handle to a volatility term structure
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
 
  // read the MC parameters
  orf::McParams mcparams = asMcParams(pyMcParams);
  // read the number of paths
  unsigned long npaths = asInt(pyNPaths);

  // create the product
  orf::SPtrProduct spprod(new orf::EuropeanCallPut(payoffType, strike, timeToExp));
  // create the pricer
  orf::BsMcPricer bsmcpricer(spprod, spyc, divYield, spvol, spot, mcparams);
  // create the statistics calculator
  orf::MeanVarCalculator<double *> sc(bsmcpricer.nVariables());
  // run the simulation
  bsmcpricer.simulate(sc, npaths);
  // collect results
  orf::Matrix const& results = sc.results();
  // read out results
  size_t nsamples = sc.nSamples();
  double mean = results(0, 0);
  double stderror = results(1, 0);
  stderror = std::sqrt(stderror / nsamples);

  // write mean and standard error into a Python dictionary
  PyObject* ret = PyDict_New();
  int ok = PyDict_SetItem(ret, asPyScalar("Mean"), asPyScalar(mean));
  PyDict_SetItem(ret, asPyScalar("StdErr"), asPyScalar(stderror));
  return ret;

PY_END;
}

static
PyObject*  pyOrfDigiBSMC(PyObject* pyDummy, PyObject* pyArgs)
{
PY_BEGIN;

  PyObject* pyPayoffType(NULL);
  PyObject* pySpot(NULL);
  PyObject* pyStrike(NULL);
  PyObject* pyTimeToExp(NULL);
  PyObject* pyDiscountCrv(NULL);
  PyObject* pyDivYield(NULL);
  PyObject* pyVolatility(NULL);
  PyObject* pyMcParams(NULL);
  PyObject* pyNPaths(NULL);

  if (!PyArg_ParseTuple(pyArgs, "OOOOOOOOO", &pyPayoffType, &pyStrike, &pyTimeToExp, 
    &pySpot, &pyDiscountCrv, &pyDivYield, &pyVolatility, &pyMcParams, &pyNPaths))
    return NULL;

  int payoffType = asInt(pyPayoffType);
  double spot = asDouble(pySpot);
  double strike = asDouble(pyStrike);
  double timeToExp = asDouble(pyTimeToExp);

  std::string name = asString(pyDiscountCrv);
  orf::SPtrYieldCurve spyc = orf::market().yieldCurves().get(name);
  ORF_ASSERT(spyc, "error: yield curve " + name + " not found");

  double divYield = asDouble(pyDivYield);
  
  // Handle volatility: either a numerical value or an object handle to a volatility term structure
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
 
  // read the MC parameters
  orf::McParams mcparams = asMcParams(pyMcParams);
  // read the number of paths
  unsigned long npaths = asInt(pyNPaths);

  // create the digital product
  orf::SPtrProduct spprod(new orf::DigitalCallPut(payoffType, strike, timeToExp));
  // create the pricer
  orf::BsMcPricer bsmcpricer(spprod, spyc, divYield, spvol, spot, mcparams);
  // create the statistics calculator
  orf::MeanVarCalculator<double *> sc(bsmcpricer.nVariables());
  // run the simulation
  bsmcpricer.simulate(sc, npaths);
  // collect results
  orf::Matrix const& results = sc.results();
  // read out results
  size_t nsamples = sc.nSamples();
  double mean = results(0, 0);
  double stderror = results(1, 0);
  stderror = std::sqrt(stderror / nsamples);

  // write mean and standard error into a Python dictionary
  PyObject* ret = PyDict_New();
  int ok = PyDict_SetItem(ret, asPyScalar("Mean"), asPyScalar(mean));
  PyDict_SetItem(ret, asPyScalar("StdErr"), asPyScalar(stderror));
  return ret;

PY_END;
}
