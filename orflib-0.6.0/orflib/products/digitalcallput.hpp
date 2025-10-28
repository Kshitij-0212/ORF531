/**
@file  digitalcallput.hpp
@brief European digital (cash or nothing) call/put option
*/

#ifndef ORF_DIGITALCALLPUT_HPP
#define ORF_DIGITALCALLPUT_HPP

#include <orflib/products/product.hpp>

BEGIN_NAMESPACE(orf)

/** European digital call/put option (cash or nothing payoff).
    Pays 1 if the option is in the money at expiration, 0 otherwise.
*/
class DigitalCallPut : public Product
{
public:
  /** Initializing ctor.
      @param payoffType 1 for call, -1 for put
      @param strike The strike price
      @param expiry The expiration time
  */
  DigitalCallPut(int payoffType, double strike, double expiry);

  /** Evaluates the product given the price path.
      For digital options, only the final price matters.
  */
  void eval(Matrix const& pricePath) override;

private:
  int payoffType_;      // 1 for call, -1 for put
  double strike_;       // strike price
  double expiry_;       // expiration time
};

///////////////////////////////////////////////////////////////////////////////
// Inline implementations

inline
DigitalCallPut::DigitalCallPut(int payoffType, double strike, double expiry)
: payoffType_(payoffType), strike_(strike), expiry_(expiry)
{
  ORF_ASSERT(payoffType == 1 || payoffType == -1, "payoffType must be 1 (call) or -1 (put)");
  ORF_ASSERT(strike > 0.0, "strike must be positive");
  ORF_ASSERT(expiry > 0.0, "expiry must be positive");

  // only one fixing time, the expiration
  fixTimes_.resize(1);
  fixTimes_[0] = expiry_;

  // assume that it will settle (pay) at expiration
  payTimes_.resize(1);
  payTimes_[0] = expiry_;

  // this product generates only one payment
  payAmounts_.resize(1);
}

inline
void DigitalCallPut::eval(Matrix const& pricePath)
{
  // For digital options, we only care about the final price
  double finalPrice = pricePath(pricePath.n_rows - 1, 0);
  
  // Calculate the digital payoff
  double payoff = 0.0;
  if (payoffType_ == 1) {
    // Digital call: pays 1 if S > K, 0 otherwise
    if (finalPrice > strike_) {
      payoff = 1.0;
    }
  } else {
    // Digital put: pays 1 if S < K, 0 otherwise
    if (finalPrice < strike_) {
      payoff = 1.0;
    }
  }
  
  // Set the payment amount
  payAmounts_[0] = payoff;
}

END_NAMESPACE(orf)

#endif // ORF_DIGITALCALLPUT_HPP