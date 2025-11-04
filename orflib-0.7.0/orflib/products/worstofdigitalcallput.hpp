/**
@file  worstofdigitalcallput.hpp
@brief The payoff of a digital option on the worst performing asset in a basket
*/

#ifndef ORF_WORSTOFDIGITALCALLPUT_HPP
#define ORF_WORSTOFDIGITALCALLPUT_HPP

#include <orflib/products/product.hpp>
#include <algorithm>
#include <limits>

BEGIN_NAMESPACE(orf)

/** The worst-of digital call/put class
*/
class WorstOfDigitalCallPut : public Product
{
public:
  /** Initializing ctor */
  WorstOfDigitalCallPut(int payoffType, double strike, double fixTime, double expTime, size_t nassets);

  /** The number of assets this product depends on */
  virtual size_t nAssets() const override { return nassets_; }

  /** Evaluates the product given the passed-in path
      The "pricePath" matrix must have 2 rows for T1 and T2,
      and as many columns as the number of assets
  */
  virtual void eval(Matrix const& pricePath) override;

private:
  int payoffType_;     // 1: call; -1 put
  double strike_;      // Strike price K
  double fixTime_;     // Fixing time T1
  double expTime_;     // Expiry time T2
  size_t nassets_;
};

///////////////////////////////////////////////////////////////////////////////
// Inline definitions

inline
WorstOfDigitalCallPut::WorstOfDigitalCallPut(int payoffType, double strike, 
                                            double fixTime, double expTime, size_t nassets)
  : payoffType_(payoffType), strike_(strike)
  , fixTime_(fixTime), expTime_(expTime), nassets_(nassets)
{
  ORF_ASSERT(payoffType == 1 || payoffType == -1, 
             "WorstOfDigitalCallPut: the payoff type must be 1 (call) or -1 (put)!");
  ORF_ASSERT(strike > 0.0, "WorstOfDigitalCallPut: the strike must be positive!");
  ORF_ASSERT(fixTime > 0.0, "WorstOfDigitalCallPut: the fixing time must be positive!");
  ORF_ASSERT(expTime > fixTime, "WorstOfDigitalCallPut: expiry time must be after fixing time!");
  ORF_ASSERT(nassets_ > 0, "WorstOfDigitalCallPut: must have at least one asset!");

  // two fixing times: T1 and T2
  fixTimes_.resize(2);
  fixTimes_[0] = fixTime_;  // T1
  fixTimes_[1] = expTime_;  // T2

  // pay at expiration
  payTimes_.resize(1);
  payTimes_[0] = expTime_;

  // this product generates only one payment
  payAmounts_.resize(1);
}

inline void 
WorstOfDigitalCallPut::eval(Matrix const& pricePath)
{
  size_t nassets = pricePath.n_cols;
  // Find the minimum return ratio across all assets
  double minRatio = std::numeric_limits<double>::max();
    
  // For each asset j, compute Sj(T2)/Sj(T1)
  for (size_t j = 0; j < nassets; ++j)
  {
    double priceT1 = pricePath(0, j);  // Price at T1
    double priceT2 = pricePath(1, j);  // Price at T2
    double ratio = priceT2 / priceT1;
    minRatio = std::min(minRatio, ratio);
  }

  // Compute payoff based on call/put
  if (payoffType_ == 1)
    payAmounts_[0] = minRatio > strike_ ? 1.0 : 0.0;  // Call
  else
    payAmounts_[0] = minRatio < strike_ ? 1.0 : 0.0;  // Put
}

END_NAMESPACE(orf)

#endif // ORF_WORSTOFDIGITALCALLPUT_HPP