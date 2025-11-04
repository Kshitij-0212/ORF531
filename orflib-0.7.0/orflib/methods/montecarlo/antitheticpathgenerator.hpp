/**
@file  antitheticpathgenerator.hpp
@brief Antithetic path generator for Monte Carlo simulation that uses antithetic sampling
       to reduce variance by generating negatively correlated paths
*/

#ifndef ORF_ANTITHETICPATHGENERATOR_HPP
#define ORF_ANTITHETICPATHGENERATOR_HPP

#include <orflib/defines.hpp>
#include <orflib/exception.hpp>
#include <orflib/methods/montecarlo/pathgenerator.hpp>

BEGIN_NAMESPACE(orf)

class AntitheticPathGenerator : public PathGenerator
{
public:
  /** Constructor
      @param generator The underlying path generator
  */
  explicit AntitheticPathGenerator(SPtrPathGenerator generator)
    : PathGenerator()
    , generator_(generator)
    , needNewPath_(true)
  {
    if (!generator_)
      ORF_ASSERT(0,
        "AntitheticPathGenerator: null path generator passed");

    // Set dimensions from the underlying generator
    ntimesteps_ = generator_->nTimeSteps();
    nfactors_ = generator_->nFactors();
    lastPath_.resize(ntimesteps_, nfactors_);
  }

  /** Returns the next path.
      First call returns a new path, second call returns its antithetic pair.
      @param pricePath The matrix to store the path in
  */
  void next(Matrix& pricePath) override
  {
    if (needNewPath_)
    {
      // Get a new path from the generator and store it
      generator_->next(lastPath_);
      pricePath = lastPath_;
      needNewPath_ = false;
    }
    else
    {
      // Return antithetic path (negated version of last path)
      pricePath = -lastPath_;
      needNewPath_ = true;
    }
  }

private:
  SPtrPathGenerator generator_;  // The underlying path generator
  Matrix lastPath_;             // Stores the last generated path
  bool needNewPath_;           // Whether we need to generate a new path
};

END_NAMESPACE(orf)

#endif // ORF_ANTITHETICPATHGENERATOR_HPP
