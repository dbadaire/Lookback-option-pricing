#ifndef OPTION_H
#define OPTION_H

#include <cstdint>
#include <cmath>
#include <limits>
#include <stdexcept>

namespace opt {

    /**
     * @brief Résultat statistique d'une estimation Monte Carlo (prix ou grecque).
     */
    struct MCStats {
        double estimate = std::numeric_limits<double>::quiet_NaN();  ///< Estimateur (moyenne).
        double stdError = std::numeric_limits<double>::quiet_NaN();  ///< Erreur standard.
        double ciLow = std::numeric_limits<double>::quiet_NaN();     ///< Borne inférieure IC 95%.
        double ciHigh = std::numeric_limits<double>::quiet_NaN();    ///< Borne supérieure IC 95%.
    };

    /**
     * @brief Interface abstraite commune pour les options en Black–Scholes (Monte Carlo).
     *
     * Stocke les paramètres de marché et fournit des utilitaires communs (validation, discount, IC 95%).
     */
    class Option {
    protected:
        double S0_, R_, sigma_, T0_, T_;

        /**
         * @brief Vérifie la cohérence des paramètres (domaines admissibles).
         * @throw std::invalid_argument si incohérent.
         */
        void validate() const;

        /**
         * @brief Calcule l'intervalle de confiance à 95% (z = 1.96).
         * @param mean Moyenne.
         * @param stdError Erreur standard.
         */
        static MCStats makeCI95(double mean, double stdError);

    public:
        /**
         * @brief Construit une option en modèle de Black–Scholes.
         * @param S0 Spot à T0.
         * @param R Taux sans risque continu.
         * @param sigma Volatilité constante.
         * @param T0 Temps de départ (valorisation).
         * @param T Temps de maturité.
         */
        Option(double S0, double R, double sigma, double T0, double T);

        virtual ~Option() = default;

        double S0() const { return S0_; }
        double R() const { return R_; }
        double sigma() const { return sigma_; }
        double T0() const { return T0_; }
        double T() const { return T_; }

        /**
         * @brief Prix Monte Carlo (à implémenter par les classes dérivées).
         *
         * @param paths Nombre de trajectoires simulées.
         * @param steps Nombre de pas de discrétisation entre T0 et T.
         * @param seed  Graine du générateur pseudo-aléatoire.
         * @param antithetic Active les variables antithétiques.
         * @return Statistiques (moyenne, erreur standard, IC 95%).
         */
        virtual MCStats priceMC(int paths, int steps, std::uint64_t seed, bool antithetic) const = 0;
    };

} // namespace opt

#endif // OPTION_H
