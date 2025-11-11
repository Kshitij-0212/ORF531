/**
@file  bermudancallput.hpp
@brief The payoff of a Bermudan Call/Put option
*/

#ifndef ORF_BERMUDANCALLPUT_HPP
#define ORF_BERMUDANCALLPUT_HPP

#include <orflib/products/europeancallput.hpp>
#include <orflib/products/americancallput.hpp>

BEGIN_NAMESPACE(orf)

/** The Bermudan call/put class
*/
class BermudanCallPut : public EuropeanCallPut
{
public:
  /** Initializing ctor */
  BermudanCallPut(int payoffType, double strike, Vector const& timesToExer);

  /** Evaluates the product at fixing time index idx
  */
  virtual void eval(size_t idx, Vector const& pricePath, double contValue);
};

///////////////////////////////////////////////////////////////////////////////
// Inline definitions

inline
BermudanCallPut::BermudanCallPut(int payoffType, double strike, Vector const& timesToExer)
: EuropeanCallPut(payoffType, strike, timesToExer[timesToExer.size() - 1])
{
  // count the number of days between 0 and timeToExp
  size_t nfixings = static_cast<size_t>(timesToExer.size());
  ORF_ASSERT(nfixings > 0, "BermudanCallPut: the option has expired!");
  fixTimes_.resize(nfixings);
  for (size_t i = 0; i < nfixings - 1; ++i)
    fixTimes_[i] = timesToExer[i];
  fixTimes_[nfixings - 1] = timesToExer[nfixings - 1];

  payTimes_ = fixTimes_;

  // this product could generate a payment on each given time between now and expiration.
  payAmounts_.resize(payTimes_.size());
}

// This product has as many fixings as given.
inline void BermudanCallPut::eval(size_t idx, Vector const& spots, double contValue)
{
  double spot = spots[0];

  if (idx == payAmounts_.size() - 1) { // this is the last index
    double payoff = (spot - strike_) * payoffType_;
    payAmounts_[idx] = payoff > 0.0 ? payoff : 0.0;
  }
  else {  // this is not the last index, check the exercise condition
    double intrinsicValue = (spot - strike_) * payoffType_;
    intrinsicValue = intrinsicValue >= 0.0 ? intrinsicValue : 0.0;
    payAmounts_[idx] = contValue >= intrinsicValue ? contValue : intrinsicValue;
    // zero out the amounts after this index
    for (size_t j = idx + 1; j < payAmounts_.size(); ++j)
      payAmounts_[j] = 0.0;
  }
}

END_NAMESPACE(orf)

#endif // ORF_BERMUDANCALLPUT_HPP
