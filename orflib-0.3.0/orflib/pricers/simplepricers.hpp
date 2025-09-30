/**
@file  simplepricers.hpp
@brief Declaration of simple pricing functions
*/

#ifndef ORF_SIMPLEPRICERS_HPP
#define ORF_SIMPLEPRICERS_HPP

#include <orflib/defines.hpp>
#include <orflib/exception.hpp>

#include <cmath>
#include <vector>

BEGIN_NAMESPACE(orf)

/** The forward price of an asset */
double fwdPrice(double spot, double timeToExp, double intRate, double divYield);

/** Price of a European digital option in the Black-Scholes model*/
std::vector<double> digitalOptionBS(int payoffType, double spot, double strike, double timeToExp,
                       double intRate, double divYield, double volatility);

/** Price of a European option in the Black-Scholes model*/
std::vector<double> europeanOptionBS(int payoffType, double spot, double strike, double timeToExp,
                        double intRate, double divYield, double volatility);

END_NAMESPACE(orf)

#endif // ORF_SIMPLEPRICERS_HPP
