#ifndef PAYOFF_H
#define PAYOFF_H

#include <stdexcept>

namespace opt {

    /**
     * @brief Classe abstraite représentant le payoff d'une option.
     */
    class Payoff {
    public:
        /**
         * @brief Calcule la valeur du payoff pour un prix S du sous-jacent.
         * @param S Prix du sous-jacent à maturité.
         * @param K Prix d'exercice de l'option.
         * @return Valeur du payoff.
         */
        virtual double operator()(double S, double K) const = 0;
    };

    /**
     * @brief Payoff d'une option d'achat (call).
     */
    class PayoffCall : public Payoff {
    public:
        double operator()(double S, double K) const override;
    };

    /**
     * @brief Payoff d'une option de vente (put).
     */
    class PayoffPut : public Payoff {
    public:
        double operator()(double S, double K) const override;
    };

    /**
     * @brief Payoff d'un call digital.
     */
    class PayoffDigitCall : public Payoff {
    public:
        double operator()(double S, double K) const override;
    };

    /**
     * @brief Payoff d'un put digital.
     */
    class PayoffDigitPut : public Payoff {
    public:
        double operator()(double S, double K) const override;
    };

} // namespace opt

#endif // PAYOFF_H


