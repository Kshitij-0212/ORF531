/**
@file  ptpricers.hpp
@brief Declaration of portfolio pricing functions
*/

#include <orflib/defines.hpp>
#include <orflib/exception.hpp>
#include <orflib/math/matrix.hpp>
#include <tuple>

BEGIN_NAMESPACE(orf)

/** Mean return and standard deviation of returns of a portfolio */
std::tuple<double, double> ptRisk(Vector const& weights,
                                  Vector const& assetRets,
                                  Vector const& assetVols,
                                  Matrix const& correlMat);

/** Weights of the minimum variance portfolio */
Vector mvpWeights(Vector const& assetRets, Vector const& assetVols, Matrix const& correlMat);

/** Weights of the CAPM market portfolio */
Vector mktWeights(Vector const& assetRets, Vector const& assetVols, Matrix const& correlMat, double rfreeRate);

/** The mean return, standard deviation of returns, and lambda of the CAPM market portfolio */
std::tuple<double, double, double> 
mktRisk(Vector const& assetRets, Vector const& assetVols, Matrix const& correlMat, double rfreeRate);

/** Weights of the efficient market portfolio */
Vector meanVarWeights(Vector const& assetRets, Vector const& assetVols, Matrix const& correlMat, double lambda);

std::tuple<std::vector<double>, std::vector<double>, std::vector<double>> meanVarFront(Vector const& assetRets, Vector const& assetVols, Matrix const& correlMat, double lambdaMax, double nLambdaSteps);

END_NAMESPACE(orf)
