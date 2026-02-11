#ifndef ASIAN_H
#define ASIAN_H

#include "Option.h"
#include <random>
#include <vector>

namespace opt {

    /**
     * @brief Option path-dépendante en Black–Scholes valorisée par Monte Carlo.
     *
     * @tparam TPayoff  Type de payoff :
     *                  double operator()(double agg, double ST) const
     *                  - agg : valeur agrégée (moyenne, min, max, ...)
     *                  - ST  : valeur finale du sous-jacent à l'échéance
     *
     * @tparam TAggregator Type d'agrégateur :
     *                     double operator()(double agg, double price, double step) const
     *
     * Variance reduction : variables antithétiques (optionnel).
     * Grecques : bump-and-reprice (différences finies centrées).
     */
    template <typename TPayoff, typename TAggregator>
    class Asian : public Option {
    private:
        TPayoff payoff_;
        TAggregator aggregator_;

        /**
         * @brief Simule le payoff actualisé (discounted) pour un vecteur gaussien donné.
         */
        double discountedPayoffFromZ(double S0, double R, double sigma, double T0, double T, int steps,
            const std::vector<double>& Zs, bool flip) const;

        /**
         * @brief Moteur Monte Carlo générique : calcule moyenne/SE/IC95% d'un estimateur défini "par trajectoire".
         *
         * @tparam SampleFn Callable : double(const std::vector<double>& Zs, bool flip)
         */
        template <typename SampleFn>
        MCStats runMC(int paths, int steps, std::uint64_t seed, bool antithetic, SampleFn&& sampleFn) const;

    public:
        /**
         * @brief Construit une option path-dépendante.
         */
        Asian(double S0, double R, double sigma, double T0, double T, const TPayoff& payoff, const TAggregator& aggregator)
            : Option(S0, R, sigma, T0, T), payoff_(payoff), aggregator_(aggregator)
        {
        }

        /**
         * @brief Prix par Monte Carlo (moyenne, erreur standard, IC 95%).
         */
        MCStats priceMC(int paths, int steps, std::uint64_t seed, bool antithetic) const override;

        /**
         * @brief Delta (dP/dS0) par différence centrée.
         */
        MCStats deltaMC(int paths, int steps, std::uint64_t seed, bool antithetic, double relEps = 1e-4) const;

        /**
         * @brief Gamma (d2P/dS0^2) par différence centrée.
         */
        MCStats gammaMC(int paths, int steps, std::uint64_t seed, bool antithetic, double relEps = 1e-3) const;

        /**
         * @brief Theta (dP/dT0) par différence centrée sur T0.
         */
        MCStats thetaMC(int paths, int steps, std::uint64_t seed, bool antithetic, double eps = 1.0 / 365.0) const;

        /**
         * @brief Rho (dP/dR) par différence centrée.
         */
        MCStats rhoMC(int paths, int steps, std::uint64_t seed, bool antithetic, double eps = 1e-4) const;

        /**
         * @brief Vega (dP/dsigma) par différence centrée.
         */
        MCStats vegaMC(int paths, int steps, std::uint64_t seed, bool antithetic, double eps = 1e-4) const;

        /**
        * @brief Prix asymptotique de référence avec correction Brownian Bridge (LOOKBACK ONLY).
        *
        * Cette méthode calcule un prix numérique de référence en utilisant :
        *  - une correction Brownian Bridge pour l’estimation continue du minimum ou du maximum,
        *  - un nombre élevé de trajectoires Monte Carlo,
        *  - une discrétisation temporelle fine,
        *  - des variables antithétiques systématiquement activées.
        *
        * @warning
        * Cette méthode est mathématiquement valable uniquement pour les options lookback,
        * c’est-à-dire lorsque l’agrégateur représente un minimum ou un maximum.
        * Elle n’est pas destinée à un usage opérationnel, mais à servir de
        * valeur asymptotique de référence pour l’étude de convergence.
        *
        * @return Prix Monte Carlo asymptotique (scalaire).
        */
        double priceMC_BrownianBridge_Asymptotic() const;
    };

    template <typename TPayoff, typename TAggregator>
    double Asian<TPayoff,TAggregator>::discountedPayoffFromZ(double S0, double R, double sigma, 
        double T0, double T, int steps, const std::vector<double>& Zs, bool flip) const
    {
        double Tau = T - T0;
        double dt = Tau / static_cast<double>(steps);
        double disc = std::exp(-R * Tau);
        double St = S0;
        double agg = S0;

        for (int j = 0; j < steps; ++j) {
            double Z = flip ? -Zs[j] : Zs[j];
            St *= std::exp((R - 0.5 * sigma * sigma) * dt + sigma * std::sqrt(dt) * Z);
            agg = aggregator_(agg, St, static_cast<double>(j + 1));
        }

        return disc * payoff_(St, agg);
    }

    template <typename TPayoff, typename TAggregator>
	template <typename SampleFn>
    MCStats Asian<TPayoff, TAggregator>::runMC(int paths, int steps, std::uint64_t seed, 
        bool antithetic, SampleFn&& sampleFn) const
    {
        if (paths <= 0) throw std::invalid_argument("paths doit être > 0.");
        if (steps <= 0) throw std::invalid_argument("steps doit être > 0.");

        std::mt19937_64 rng(seed);
        std::normal_distribution<double> nd(0.0, 1.0);

        int m = 0;
        double mean = 0.0;
        double M2 = 0.0;

        auto pushSample = [&](double x) {
            ++m;
            double d = x - mean;
            mean += d / m;
            double d2 = x - mean;
            M2 += d * d2;
            };

        if (antithetic) {
            int pairs = (paths + 1) / 2;
            for (int i = 0; i < pairs; ++i) {
                std::vector<double> Zs(steps);
                for (int j = 0; j < steps; ++j) Zs[j] = nd(rng);
                double s1 = sampleFn(Zs, false);
                double s2 = sampleFn(Zs, true);
                double sample = 0.5 * (s1 + s2);
                pushSample(sample);
            }
        }
        else {
            for (int i = 0; i < paths; ++i) {
                std::vector<double> Zs(steps);
                for (int j = 0; j < steps; ++j) Zs[j] = nd(rng);
                double sample = sampleFn(Zs, false);
                pushSample(sample);
            }
        }

        double var = (m > 1) ? (M2 / (m - 1)) : 0.0;
        double se = (m > 0) ? std::sqrt(var / m) : std::numeric_limits<double>::quiet_NaN();

        return Option::makeCI95(mean, se);
    }

    template <typename TPayoff, typename TAggregator>
    MCStats Asian<TPayoff, TAggregator>::priceMC(int paths, int steps, std::uint64_t seed, bool antithetic) const
    {
        auto samplePrice = [&](const std::vector<double>& Zs, bool flip) -> double {
            return discountedPayoffFromZ(S0_, R_, sigma_, T0_, T_, steps, Zs, flip);
            };

        return runMC(paths, steps, seed, antithetic, samplePrice);
    }

    template <typename TPayoff, typename TAggregator>
    MCStats Asian<TPayoff, TAggregator>::deltaMC(int paths, int steps, std::uint64_t seed, 
        bool antithetic, double relEps) const
    {
        double eps = relEps * S0_;

        auto sampleDelta = [&](const std::vector<double>& Zs, bool flip) -> double {
            double Pu = discountedPayoffFromZ(S0_ + eps, R_, sigma_, T0_, T_, steps, Zs, flip);
            double Pd = discountedPayoffFromZ(S0_ - eps, R_, sigma_, T0_, T_, steps, Zs, flip);
            return (Pu - Pd) / (2.0 * eps);
            };

        return runMC(paths, steps, seed, antithetic, sampleDelta);
    }

    template <typename TPayoff, typename TAggregator>
    MCStats Asian<TPayoff, TAggregator>::gammaMC(int paths, int steps, std::uint64_t seed, 
        bool antithetic, double relEps) const
    {
        double eps = relEps * S0_;

        auto sampleGamma = [&](const std::vector<double>& Zs, bool flip) -> double {
            double Pu = discountedPayoffFromZ(S0_ + eps, R_, sigma_, T0_, T_, steps, Zs, flip);
            double Pm = discountedPayoffFromZ(S0_, R_, sigma_, T0_, T_, steps, Zs, flip);
            double Pd = discountedPayoffFromZ(S0_ - eps, R_, sigma_, T0_, T_, steps, Zs, flip);
            return (Pu - 2.0 * Pm + Pd) / (eps * eps);
            };

        return runMC(paths, steps, seed, antithetic, sampleGamma);
    }

    template <typename TPayoff, typename TAggregator>
    MCStats Asian<TPayoff, TAggregator>::thetaMC(int paths, int steps, std::uint64_t seed, 
        bool antithetic, double eps) const
    {
        if (!(T0_ + eps < T_ && T0_ - eps < T_))
            throw std::invalid_argument("Theta: eps trop grand par rapport à T0/T.");

        auto sampleTheta = [&](const std::vector<double>& Zs, bool flip) -> double {
            double Pu = discountedPayoffFromZ(S0_, R_, sigma_, T0_ + eps, T_, steps, Zs, flip);
            double Pd = discountedPayoffFromZ(S0_, R_, sigma_, T0_ - eps, T_, steps, Zs, flip);
            return (Pu - Pd) / (2.0 * eps);
            };

        return runMC(paths, steps, seed, antithetic, sampleTheta);
    }

    template <typename TPayoff, typename TAggregator>
    MCStats Asian<TPayoff, TAggregator>::rhoMC(int paths, int steps, std::uint64_t seed, 
        bool antithetic, double eps) const
    {
        auto sampleRho = [&](const std::vector<double>& Zs, bool flip) -> double {
            double Pu = discountedPayoffFromZ(S0_, R_ + eps, sigma_, T0_, T_, steps, Zs, flip);
            double Pd = discountedPayoffFromZ(S0_, R_ - eps, sigma_, T0_, T_, steps, Zs, flip);
            return (Pu - Pd) / (2.0 * eps);
            };

        return runMC(paths, steps, seed, antithetic, sampleRho);
    }

    template <typename TPayoff, typename TAggregator>
    MCStats Asian<TPayoff, TAggregator>::vegaMC(int paths, int steps, std::uint64_t seed, 
        bool antithetic, double eps) const
    {
        auto sampleVega = [&](const std::vector<double>& Zs, bool flip) -> double {
            double Pu = discountedPayoffFromZ(S0_, R_, sigma_ + eps, T0_, T_, steps, Zs, flip);
            double Pd = discountedPayoffFromZ(S0_, R_, sigma_ - eps, T0_, T_, steps, Zs, flip);
            return (Pu - Pd) / (2.0 * eps);
            };

        return runMC(paths, steps, seed, antithetic, sampleVega);
    }

    template <typename TPayoff, typename TAggregator>
    double Asian<TPayoff, TAggregator>::priceMC_BrownianBridge_Asymptotic() const
    {
        // Paramètres fixes de référence
        int paths = 1'000;
        int steps = 1'000;
        bool antithetic = true;
        std::uint64_t seed = 42;

        double Tau = T_ - T0_;
        double dt = Tau / static_cast<double>(steps);
        double disc = std::exp(-R_ * Tau);

        std::mt19937_64 rng(seed);
        std::normal_distribution<double> nd(0.0, 1.0);
        std::uniform_real_distribution<double> ud(0.0, 1.0);

        // --------------------------------------------------------------------
        // Helper: échantillonner le maximum d’un Brownian Bridge (en log) sur un pas dt
        // Bridge sur X(t) avec X0=a, X1=b, variance incrémentale = sigma^2 * dt.
        //
        // Pour m >= max(a,b):
        //   P(M <= m | a,b) = 1 - exp( -2 (m-a)(m-b) / (sigma^2 dt) )
        //
        // Si U ~ Unif(0,1), on peut poser V = 1-U ~ Unif(0,1),
        //   (m-a)(m-b) = -0.5 * (sigma^2 dt) * log(V)
        //
        // Solution (racine supérieure) :
        //   m = 0.5 * (a+b + sqrt((a-b)^2 + 4K)), K = -0.5*s2dt*log(V)
        // --------------------------------------------------------------------
        auto bridgeMaxLog = [&](double a, double b, double s2dt, double V) -> double {
            // V doit être dans (0,1) pour log(V)
            V = std::min<double>(std::max<double>(V, 1e-16), 1.0 - 1e-16);
            double K = -0.5 * s2dt * std::log(V);
            double D = (a - b) * (a - b) + 4.0 * K;
            return 0.5 * (a + b + std::sqrt(D));
            };

        // Minimum en log via symétrie : min(X) = - max(-X)
        auto bridgeMinLog = [&](double a, double b, double s2dt, double V) -> double {
            return -bridgeMaxLog(-a, -b, s2dt, V);
            };

        // Payoff avec correction BB : on met à jour l’agrégateur avec les extrêmes continus + endpoint.
        // (Si l’agrégateur est Max, l’info "min" n’affecte pas. Si Min, l’info "max" n’affecte pas.)
        auto payoffBB = [&](const std::vector<double>& Zs, const std::vector<double>& Umax,
            const std::vector<double>& Umin, bool flip) -> double
            {
                double St = S0_;
                double agg = S0_;
                double logS_prev = std::log(S0_);
                double s2dt = sigma_ * sigma_ * dt;

                for (int j = 0; j < steps; ++j) {
                    double Z = flip ? -Zs[j] : Zs[j];
                    double logS_next = logS_prev + (R_ - 0.5 * sigma_ * sigma_) * dt + sigma_ * std::sqrt(dt) * Z;
                    double S_next = std::exp(logS_next);

                    // Extrêmes continus conditionnels (en log), puis retour en niveau
                    double logM = bridgeMaxLog(logS_prev, logS_next, s2dt, Umax[j]);
                    double logm = bridgeMinLog(logS_prev, logS_next, s2dt, Umin[j]);
                    double S_max_cont = std::exp(logM);
                    double S_min_cont = std::exp(logm);

                    // Mise à jour agrégateur (lookback min/max)
                    agg = aggregator_(agg, S_max_cont, j + 0.5);
                    agg = aggregator_(agg, S_min_cont, j + 0.5);
                    agg = aggregator_(agg, S_next, j + 1.0);
                    St = S_next;
                    logS_prev = logS_next;
                }

                return payoff_(St, agg);
            };

        double sumPayoff = 0.0;

        if (antithetic) {
            const int pairs = paths / 2;
            for (int i = 0; i < pairs; ++i) {

                // 1) Génère Zs
                std::vector<double> Zs(steps);
                for (int j = 0; j < steps; ++j)
                    Zs[j] = nd(rng);

                // 2) Génère aussi les U (uniformes) une fois par paire, puis réutilisés pour flip=true
                //    => partie "bridge" corrélée entre (Z) et (-Z)
                std::vector<double> Umax(steps), Umin(steps);
                for (int j = 0; j < steps; ++j) {
                    double u1 = ud(rng);
                    double u2 = ud(rng);
                    // On borne pour éviter log(0)
                    Umax[j] = std::min<double>(std::max<double>(u1, 1e-16), 1.0 - 1e-16);
                    Umin[j] = std::min<double>(std::max<double>(u2, 1e-16), 1.0 - 1e-16);
                }

                double p1 = payoffBB(Zs, Umax, Umin, false);
                double p2 = payoffBB(Zs, Umax, Umin, true);
                sumPayoff += 0.5 * (p1 + p2);
            }

            double meanPayoff = sumPayoff / static_cast<double>(pairs);
            return disc * meanPayoff;
        }
        else {
            for (int i = 0; i < paths; ++i) {

                std::vector<double> Zs(steps);
                for (int j = 0; j < steps; ++j)
                    Zs[j] = nd(rng);

                std::vector<double> Umax(steps), Umin(steps);
                for (int j = 0; j < steps; ++j) {
                    double u1 = ud(rng);
                    double u2 = ud(rng);
                    Umax[j] = std::min<double>(std::max<double>(u1, 1e-16), 1.0 - 1e-16);
                    Umin[j] = std::min<double>(std::max<double>(u2, 1e-16), 1.0 - 1e-16);
                }

                sumPayoff += payoffBB(Zs, Umax, Umin, false);
            }

            double meanPayoff = sumPayoff / static_cast<double>(paths);
            return disc * meanPayoff;
        }
    }

} // namespace opt

#endif // ASIAN_H

