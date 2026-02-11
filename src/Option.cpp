#include "pch.h"
#include "Option.h"

namespace opt {

    Option::Option(double S0, double R, double sigma, double T0, double T)
        : S0_(S0), R_(R), sigma_(sigma), T0_(T0), T_(T)
    {
        validate();
    }

    void Option::validate() const {
        if (!(S0_ > 0.0)) throw std::invalid_argument("S0 doit être strictement positif.");
        if (!(sigma_ >= 0.0)) throw std::invalid_argument("sigma doit être >= 0.");
        if (!(T_ > T0_)) throw std::invalid_argument("Il faut T > T0 (maturité strictement après T0).");
        if (!(std::isfinite(S0_) && std::isfinite(R_) && std::isfinite(sigma_) && std::isfinite(T0_) && std::isfinite(T_)))
            throw std::invalid_argument("Paramètres non finis (NaN/Inf) interdits.");
    }

    MCStats Option::makeCI95(double mean, double stdError) {
        double z = 1.96; // IC 95%
        MCStats out;
        out.estimate = mean;
        out.stdError = stdError;
        out.ciLow = mean - z * stdError;
        out.ciHigh = mean + z * stdError;
        return out;
    }

} // namespace opt
