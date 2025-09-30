/**
@file  simplepricers.cpp
@brief Implementation of simple pricing functions
*/

#include <orflib/pricers/simplepricers.hpp>
#include <orflib/math/stats/normaldistribution.hpp>

#include <cmath>
#include <vector>

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
std::vector<double> digitalOptionBS(int payoffType, double spot, double strike, double timeToExp,
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
  double d1 = d2 + sigT;
  NormalDistribution normal;


  double price = exp(-intRate * timeToExp) * normal.cdf(phi * d2);

  double delta = exp(-intRate * timeToExp) * phi * normal.pdf(d2) / (spot * sigT);

  double gamma = -exp(-intRate * timeToExp) * phi * normal.pdf(d2) * (d1) / (spot * spot * volatility * volatility * timeToExp);

  double vega = - phi * exp(-intRate * timeToExp) * normal.pdf(d2) * (d1) / volatility;

  double theta = intRate * price - phi * exp(-intRate * timeToExp) * normal.pdf(d2) * ((intRate - divYield - 0.5 * volatility * volatility) - (log(spot/strike)/(timeToExp))) / (2*sigT);
  std::vector<double> result = {price, delta, gamma, theta, vega};

  return result;
}

/** Price of a European option in the Black-Scholes model*/
std::vector<double> europeanOptionBS(int payoffType, double spot, double strike, double timeToExp,
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

  double eqtyDf = exp(-divYield * timeToExp);

  double delta = eqtyDf * (normal.cdf(d1) - (phi < 0 ? 1 : 0));
  double gamma = eqtyDf * normal.pdf(d1) / (spot * sigT);
  double vega = spot * eqtyDf * normal.pdf(d1) * sqrt(timeToExp);

  double theta = - (spot * volatility * eqtyDf * normal.pdf(d1)) / (2 * sqrt(timeToExp))
                 - phi * intRate * strike * df * normal.cdf(phi * d2)
                 + phi * divYield * spot * eqtyDf * normal.cdf(phi * d1);

  std::vector<double> result = {price, delta, gamma, theta, vega};

  return result;
}

END_NAMESPACE(orf)
