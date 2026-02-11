#include "pch.h"
#include "Payoff.h"

namespace opt {

    double PayoffCall::operator()(double S, double K) const {
        if (K < 0.0)
            throw std::invalid_argument("Strike doit être non-négatif");
        return std::max<double>(S - K, 0.0);
    }

    double PayoffPut::operator()(double S, double K) const {
        if (K < 0.0)
            throw std::invalid_argument("Strike doit être non-négatif");
        return std::max<double>(K - S, 0.0);
    }

    double PayoffDigitCall::operator()(double S, double K) const {
        if (K < 0.0)
            throw std::invalid_argument("Strike doit être non-négatif");
        return (S > K) ? 1.0 : 0.0;
    }

    double PayoffDigitPut::operator()(double S, double K) const {
        if (K < 0.0)
            throw std::invalid_argument("Strike doit être non-négatif");
        return (S < K) ? 1.0 : 0.0;
    }

} // namespace opt
