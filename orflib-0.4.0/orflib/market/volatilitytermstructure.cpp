/**
@file  volatilitytermstructure.cpp
@brief Implementation of the VolatilityTermStructure class
*/

#include <orflib/market/volatilitytermstructure.hpp>
#include <cmath>

BEGIN_NAMESPACE(orf)

using namespace std;

void VolatilityTermStructure::initFromSpotVols()
{
  auto cit = fwdvars_.coeff_begin(0);

  double T1 = 0.0;
  double var1 = 0.0;  // cumulative variance at T1
  
  for (size_t i = 0; i < fwdvars_.size(); ++i, ++cit) {
    double T2 = fwdvars_.breakPoint(i);
    double vol2 = *cit;
    ORF_ASSERT(vol2 >= 0.0, "VolatilityTermStructure: volatilities must be non-negative");
    
    double var2 = vol2 * vol2 * T2;  // cumulative variance at T2
    double fwdvar = var2 - var1;     // forward variance from T1 to T2
    ORF_ASSERT(fwdvar >= 0.0,
      "VolatilityTermStructure: negative forward variance between T1 = " + to_string(T1) + " and T2 = " + to_string(T2));
    fwdvar /= (T2 - T1);               // convert to piecewise constant forward variance
    fwdvars_.setBreakPoint(i, T1);  // the ppoly object is right-continuous
    *cit = fwdvar;                  // overwrite the spot vol with the forward variance
  
    var1 = var2;  // remember the cumulative variance
    T1 = T2;      // remember the maturity
  }
}

void VolatilityTermStructure::initFromFwdVols()
{
  auto cit = fwdvars_.coeff_begin(0);
  
  double T1 = 0.0;
  for (size_t i = 0; i < fwdvars_.size(); ++i, ++cit) {
    double T2 = fwdvars_.breakPoint(i);
    double fwdvol = *cit;
    ORF_ASSERT(fwdvol >= 0.0, "VolatilityTermStructure: forward volatilities must be non-negative");
    
    double dt = T2 - T1;
    double fwdvar = fwdvol * fwdvol * dt;  // convert forward vol to forward variance
    
    fwdvars_.setBreakPoint(i, T1);  // the ppoly object is right-continuous
    *cit = fwdvar;                  // overwrite the forward vol with the forward variance
    
    T1 = T2;  // remember the maturity
  }
}

double VolatilityTermStructure::spotVol(double tMat) const
{
  ORF_ASSERT(tMat > 0.0, "VolatilityTermStructure: maturity must be positive");
  
  double totalVar = fwdvars_.integral(0.0, tMat);
  return sqrt(totalVar / tMat);  // return the annualized spot volatility
}

double VolatilityTermStructure::fwdVol(double tMat1, double tMat2) const
{
  ORF_ASSERT(tMat1 >= 0.0, "VolatilityTermStructure: forward volatilities for negative times not allowed");
  ORF_ASSERT(tMat1 < tMat2, "VolatilityTermStructure: maturities must be in ascending order");
  
  double fwdVar = fwdvars_.integral(tMat1, tMat2);
  double dt = tMat2 - tMat1;
  return sqrt(fwdVar / dt);  // return the annualized forward volatility
}

END_NAMESPACE(orf)