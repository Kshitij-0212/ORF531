/**
@file  simplepricers.cpp
@brief Implementation of simple pricing functions
*/

#include <orflib/pricers/simplepricers.hpp>
#include <orflib/math/stats/normaldistribution.hpp>

#include <cmath>

BEGIN_NAMESPACE(orf)

/** The forward price of an asset */
double fwdPrice(double spot, double timeToExp, double intRate, double divYield)
{
  ORF_ASSERT(spot >= 0.0, "spot must be non-negative");
  ORF_ASSERT(timeToExp >= 0.0, "time to expiration must be non-negative");
  ORF_ASSERT(intRate >= 0.0, "interest rate must be non-negative");
  ORF_ASSERT(divYield >= 0.0, "dividend yield must be non-negative");

  return spot * exp((intRate - divYield) * timeToExp);
}

/** Price of a European digital option in the Black-Scholes model*/
double digitalOptionBS(int payoffType, double spot, double strike, double timeToExp,
                       double intRate, double divYield, double volatility)
{
  ORF_ASSERT(payoffType == 1 || payoffType == -1, "payoffType must be 1 or -1");
  ORF_ASSERT(strike >= 0.0, "strike must be non-negative");
  ORF_ASSERT(divYield >= 0.0, "dividend yield must be non-negative");
  ORF_ASSERT(volatility >= 0.0, "volatility must be non-negative");

  double phi = payoffType;
  double fwd = fwdPrice(spot, timeToExp, intRate, divYield);
  double sigT = volatility * sqrt(timeToExp);
  double d2 = log(fwd / strike) / sigT - 0.5 * sigT;
  NormalDistribution normal;

  double price = exp(-intRate * timeToExp) * normal.cdf(phi * d2);

  return price;
}

/** Price of a European option in the Black-Scholes model*/
double europeanOptionBS(int payoffType, double spot, double strike, double timeToExp, 
                        double intRate, double divYield, double volatility)
{
  ORF_ASSERT(payoffType == 1 || payoffType == -1, "payoffType must be 1 or -1");
  ORF_ASSERT(strike >= 0.0, "strike must be non-negative");
  ORF_ASSERT(volatility >= 0.0, "volatility must be non-negative");

  double phi = payoffType;
  double fwd = fwdPrice(spot, timeToExp, intRate, divYield);
  double sigT = volatility * sqrt(timeToExp);
  double d1 = log(fwd / strike) / sigT + 0.5 * sigT;
  double d2 = d1 - sigT;

  NormalDistribution normal;
  double df = exp(-intRate * timeToExp);
  double price = fwd * normal.cdf(phi * d1) - strike * normal.cdf(phi * d2);
  price *= phi * df;

  return price;
}

/** Price of a Knockout Forward*/
double knockoutFwd(double spot, double strike, double KOLevel, double timeToExp, double timeToKO, 
                    double intRate, double divYield, double volatility)
{
  ORF_ASSERT(strike >= 0.0, "strike must be non-negative");
  ORF_ASSERT(volatility >= 0.0, "volatility must be non-negative");
  ORF_ASSERT(timeToKO < timeToExp, "timeToKO must be less than timeToExp");
  ORF_ASSERT(timeToKO >= 0.0, "timeToKO must be non-negative");
  ORF_ASSERT(timeToExp >= 0.0, "timeToExp must be non-negative");
  double price = 0.0;

  double alpha =  exp((intRate - divYield)*(timeToExp-timeToKO));
  double beta = exp(-intRate * (timeToExp));
  double gamma = exp(intRate * (timeToKO));

  double euroCall = europeanOptionBS(1, spot, KOLevel, timeToKO, intRate, divYield, volatility);
  double digiCall = digitalOptionBS(1, spot, KOLevel, timeToKO, intRate, divYield, volatility);

  price = gamma*beta*(alpha * euroCall + (alpha*KOLevel-strike) * digiCall); 

  return price;
}

double quantoFwdPrice(double spot, double timeToExp, double intRate, double divYield, double volFX, double volAsset, double corr)
{
  ORF_ASSERT(spot >= 0.0, "spot must be non-negative");
  ORF_ASSERT(timeToExp >= 0.0, "time to expiration must be non-negative");
  ORF_ASSERT(intRate >= 0.0, "interest rate must be non-negative");
  ORF_ASSERT(divYield >= 0.0, "dividend yield must be non-negative");
  ORF_ASSERT(volFX >= 0.0, "FX volatility must be non-negative");
  ORF_ASSERT(volAsset >= 0.0, "asset volatility must be non-negative");
  ORF_ASSERT(corr >= -1.0 && corr <= 1.0, "correlation must be in [-1,1]");

  double adjDivYield = corr * volFX * volAsset;
  double fwd = fwdPrice(spot, timeToExp, intRate, divYield);

  return fwd * exp(adjDivYield * timeToExp);
}

END_NAMESPACE(orf)
