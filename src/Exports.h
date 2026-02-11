#ifndef EXPORTS_H
#define EXPORTS_H

#include "Payoff.h"
#include "Aggregator.h"
#include "Option.h"
#include "Asian.h"

/**
 * @file Exports.h
 * @brief Interface C (DLL) pour Excel/VBA – options lookback (floating strike).
 *
 * Conventions :
 * - Interface "extern C" pour compatibilité VBA.
 * - Deux modes :
 *   (1) MC standard : suffixe _mc
 *   (2) MC avec réduction de variance (variables antithétiques) : suffixe _mc_vr
 * - Pour chaque grandeur (prix, grecques) on expose :
 *   estimateur, erreur standard, borne basse IC, borne haute IC.
 */

extern "C" {

    // ============================================================================
    //  LOOKBACK CALL — PRIX (MC standard)
    // ============================================================================

    __declspec(dllexport) double opt_lb_call_price_mc(double S0, double R, double sigma,
        double T0, double T, int paths, int steps, std::uint64_t seed);

    __declspec(dllexport) double opt_lb_call_price_mc_se(double S0, double R, double sigma,
        double T0, double T, int paths, int steps, std::uint64_t seed);

    __declspec(dllexport) double opt_lb_call_price_mc_ci_low(double S0, double R, double sigma,
        double T0, double T, int paths, int steps, std::uint64_t seed);

    __declspec(dllexport) double opt_lb_call_price_mc_ci_high(double S0, double R, double sigma,
        double T0, double T, int paths, int steps, std::uint64_t seed);

    // ============================================================================
    //  LOOKBACK CALL — PRIX (MC VR : antithétiques)
    // ============================================================================

    __declspec(dllexport) double opt_lb_call_price_mc_vr(double S0, double R, double sigma,
        double T0, double T, int paths, int steps, std::uint64_t seed);

    __declspec(dllexport) double opt_lb_call_price_mc_vr_se(double S0, double R, double sigma,
        double T0, double T, int paths, int steps, std::uint64_t seed);

    __declspec(dllexport) double opt_lb_call_price_mc_vr_ci_low(double S0, double R, double sigma,
        double T0, double T, int paths, int steps, std::uint64_t seed);

    __declspec(dllexport) double opt_lb_call_price_mc_vr_ci_high(double S0, double R, double sigma,
        double T0, double T, int paths, int steps, std::uint64_t seed);

    // ============================================================================
    //  LOOKBACK CALL — DELTA (MC standard/VR) 
    // ============================================================================

    __declspec(dllexport) double opt_lb_call_delta_mc(double S0, double R, double sigma,
        double T0, double T, int paths, int steps, std::uint64_t seed);

    __declspec(dllexport) double opt_lb_call_delta_mc_se(double S0, double R, double sigma,
        double T0, double T, int paths, int steps, std::uint64_t seed);

    __declspec(dllexport) double opt_lb_call_delta_mc_ci_low(double S0, double R, double sigma,
        double T0, double T, int paths, int steps, std::uint64_t seed);

    __declspec(dllexport) double opt_lb_call_delta_mc_ci_high(double S0, double R, double sigma,
        double T0, double T, int paths, int steps, std::uint64_t seed);

    __declspec(dllexport) double opt_lb_call_delta_mc_vr(double S0, double R, double sigma,
        double T0, double T, int paths, int steps, std::uint64_t seed);

    __declspec(dllexport) double opt_lb_call_delta_mc_vr_se(double S0, double R, double sigma,
        double T0, double T, int paths, int steps, std::uint64_t seed);

    __declspec(dllexport) double opt_lb_call_delta_mc_vr_ci_low(double S0, double R, double sigma,
        double T0, double T, int paths, int steps, std::uint64_t seed);

    __declspec(dllexport) double opt_lb_call_delta_mc_vr_ci_high(double S0, double R, double sigma,
        double T0, double T, int paths, int steps, std::uint64_t seed);

    // ============================================================================
    //  LOOKBACK CALL — GAMMA (MC standard/VR)
    // ============================================================================

    __declspec(dllexport) double opt_lb_call_gamma_mc(double S0, double R, double sigma,
        double T0, double T, int paths, int steps, std::uint64_t seed);

    __declspec(dllexport) double opt_lb_call_gamma_mc_se(double S0, double R, double sigma,
        double T0, double T, int paths, int steps, std::uint64_t seed);

    __declspec(dllexport) double opt_lb_call_gamma_mc_ci_low(double S0, double R, double sigma,
        double T0, double T, int paths, int steps, std::uint64_t seed);

    __declspec(dllexport) double opt_lb_call_gamma_mc_ci_high(double S0, double R, double sigma,
        double T0, double T, int paths, int steps, std::uint64_t seed);

    __declspec(dllexport) double opt_lb_call_gamma_mc_vr(double S0, double R, double sigma,
        double T0, double T, int paths, int steps, std::uint64_t seed);

    __declspec(dllexport) double opt_lb_call_gamma_mc_vr_se(double S0, double R, double sigma,
        double T0, double T, int paths, int steps, std::uint64_t seed);

    __declspec(dllexport) double opt_lb_call_gamma_mc_vr_ci_low(double S0, double R, double sigma,
        double T0, double T, int paths, int steps, std::uint64_t seed);

    __declspec(dllexport) double opt_lb_call_gamma_mc_vr_ci_high(double S0, double R, double sigma,
        double T0, double T, int paths, int steps, std::uint64_t seed);

    // ============================================================================
    //  LOOKBACK CALL — THETA (MC standard/VR)
    // ============================================================================

    __declspec(dllexport) double opt_lb_call_theta_mc(double S0, double R, double sigma,
        double T0, double T, int paths, int steps, std::uint64_t seed);

    __declspec(dllexport) double opt_lb_call_theta_mc_se(double S0, double R, double sigma,
        double T0, double T, int paths, int steps, std::uint64_t seed);

    __declspec(dllexport) double opt_lb_call_theta_mc_ci_low(double S0, double R, double sigma,
        double T0, double T, int paths, int steps, std::uint64_t seed);

    __declspec(dllexport) double opt_lb_call_theta_mc_ci_high(double S0, double R, double sigma,
        double T0, double T, int paths, int steps, std::uint64_t seed);

    __declspec(dllexport) double opt_lb_call_theta_mc_vr(double S0, double R, double sigma,
        double T0, double T, int paths, int steps, std::uint64_t seed);

    __declspec(dllexport) double opt_lb_call_theta_mc_vr_se(double S0, double R, double sigma,
        double T0, double T, int paths, int steps, std::uint64_t seed);

    __declspec(dllexport) double opt_lb_call_theta_mc_vr_ci_low(double S0, double R, double sigma,
        double T0, double T, int paths, int steps, std::uint64_t seed);

    __declspec(dllexport) double opt_lb_call_theta_mc_vr_ci_high(double S0, double R, double sigma,
        double T0, double T, int paths, int steps, std::uint64_t seed);

    // ============================================================================
    //  LOOKBACK CALL — RHO (MC standard/VR) 
    // ============================================================================

    __declspec(dllexport) double opt_lb_call_rho_mc(double S0, double R, double sigma,
        double T0, double T, int paths, int steps, std::uint64_t seed);

    __declspec(dllexport) double opt_lb_call_rho_mc_se(double S0, double R, double sigma,
        double T0, double T, int paths, int steps, std::uint64_t seed);

    __declspec(dllexport) double opt_lb_call_rho_mc_ci_low(double S0, double R, double sigma,
        double T0, double T, int paths, int steps, std::uint64_t seed);

    __declspec(dllexport) double opt_lb_call_rho_mc_ci_high(double S0, double R, double sigma,
        double T0, double T, int paths, int steps, std::uint64_t seed);

    __declspec(dllexport) double opt_lb_call_rho_mc_vr(double S0, double R, double sigma,
        double T0, double T, int paths, int steps, std::uint64_t seed);

    __declspec(dllexport) double opt_lb_call_rho_mc_vr_se(double S0, double R, double sigma,
        double T0, double T, int paths, int steps, std::uint64_t seed);

    __declspec(dllexport) double opt_lb_call_rho_mc_vr_ci_low(double S0, double R, double sigma,
        double T0, double T, int paths, int steps, std::uint64_t seed);

    __declspec(dllexport) double opt_lb_call_rho_mc_vr_ci_high(double S0, double R, double sigma,
        double T0, double T, int paths, int steps, std::uint64_t seed);

    // ============================================================================
    //  LOOKBACK CALL — VEGA (MC standard/VR) 
    // ============================================================================

    __declspec(dllexport) double opt_lb_call_vega_mc(double S0, double R, double sigma,
        double T0, double T, int paths, int steps, std::uint64_t seed);

    __declspec(dllexport) double opt_lb_call_vega_mc_se(double S0, double R, double sigma,
        double T0, double T, int paths, int steps, std::uint64_t seed);

    __declspec(dllexport) double opt_lb_call_vega_mc_ci_low(double S0, double R, double sigma,
        double T0, double T, int paths, int steps, std::uint64_t seed);

    __declspec(dllexport) double opt_lb_call_vega_mc_ci_high(double S0, double R, double sigma,
        double T0, double T, int paths, int steps, std::uint64_t seed);

    __declspec(dllexport) double opt_lb_call_vega_mc_vr(double S0, double R, double sigma,
        double T0, double T, int paths, int steps, std::uint64_t seed);

    __declspec(dllexport) double opt_lb_call_vega_mc_vr_se(double S0, double R, double sigma,
        double T0, double T, int paths, int steps, std::uint64_t seed);

    __declspec(dllexport) double opt_lb_call_vega_mc_vr_ci_low(double S0, double R, double sigma,
        double T0, double T, int paths, int steps, std::uint64_t seed);

    __declspec(dllexport) double opt_lb_call_vega_mc_vr_ci_high(double S0, double R, double sigma,
        double T0, double T, int paths, int steps, std::uint64_t seed);

    // ============================================================================
    //  LOOKBACK CALL — PRIX ASYMPTOTIQUE (Brownian Bridge, LOOKBACK ONLY)
    // ============================================================================

    __declspec(dllexport) double opt_lb_call_price_bb_asymptotic(double S0, double R, double sigma,
        double T0, double T);

    // ============================================================================
    //  LOOKBACK PUT — PRIX (MC standard)
    // ============================================================================

    __declspec(dllexport) double opt_lb_put_price_mc(double S0, double R, double sigma,
        double T0, double T, int paths, int steps, std::uint64_t seed);

    __declspec(dllexport) double opt_lb_put_price_mc_se(double S0, double R, double sigma,
        double T0, double T, int paths, int steps, std::uint64_t seed);

    __declspec(dllexport) double opt_lb_put_price_mc_ci_low(double S0, double R, double sigma,
        double T0, double T, int paths, int steps, std::uint64_t seed);

    __declspec(dllexport) double opt_lb_put_price_mc_ci_high(double S0, double R, double sigma,
        double T0, double T, int paths, int steps, std::uint64_t seed);

    // ============================================================================
    //  LOOKBACK PUT — PRIX (MC VR : antithétiques)
    // ============================================================================

    __declspec(dllexport) double opt_lb_put_price_mc_vr(double S0, double R, double sigma,
        double T0, double T, int paths, int steps, std::uint64_t seed);

    __declspec(dllexport) double opt_lb_put_price_mc_vr_se(double S0, double R, double sigma,
        double T0, double T, int paths, int steps, std::uint64_t seed);

    __declspec(dllexport) double opt_lb_put_price_mc_vr_ci_low(double S0, double R, double sigma,
        double T0, double T, int paths, int steps, std::uint64_t seed);

    __declspec(dllexport) double opt_lb_put_price_mc_vr_ci_high(double S0, double R, double sigma,
        double T0, double T, int paths, int steps, std::uint64_t seed);

    // ============================================================================
    //  LOOKBACK PUT — DELTA (MC standard/VR) 
    // ============================================================================

    __declspec(dllexport) double opt_lb_put_delta_mc(double S0, double R, double sigma,
        double T0, double T, int paths, int steps, std::uint64_t seed);

    __declspec(dllexport) double opt_lb_put_delta_mc_se(double S0, double R, double sigma,
        double T0, double T, int paths, int steps, std::uint64_t seed);

    __declspec(dllexport) double opt_lb_put_delta_mc_ci_low(double S0, double R, double sigma,
        double T0, double T, int paths, int steps, std::uint64_t seed);

    __declspec(dllexport) double opt_lb_put_delta_mc_ci_high(double S0, double R, double sigma,
        double T0, double T, int paths, int steps, std::uint64_t seed);

    __declspec(dllexport) double opt_lb_put_delta_mc_vr(double S0, double R, double sigma,
        double T0, double T, int paths, int steps, std::uint64_t seed);

    __declspec(dllexport) double opt_lb_put_delta_mc_vr_se(double S0, double R, double sigma,
        double T0, double T, int paths, int steps, std::uint64_t seed);

    __declspec(dllexport) double opt_lb_put_delta_mc_vr_ci_low(double S0, double R, double sigma,
        double T0, double T, int paths, int steps, std::uint64_t seed);

    __declspec(dllexport) double opt_lb_put_delta_mc_vr_ci_high(double S0, double R, double sigma,
        double T0, double T, int paths, int steps, std::uint64_t seed);

    // ============================================================================
    //  LOOKBACK PUT — GAMMA (MC standard/VR) 
    // ============================================================================

    __declspec(dllexport) double opt_lb_put_gamma_mc(double S0, double R, double sigma,
        double T0, double T, int paths, int steps, std::uint64_t seed);

    __declspec(dllexport) double opt_lb_put_gamma_mc_se(double S0, double R, double sigma,
        double T0, double T, int paths, int steps, std::uint64_t seed);

    __declspec(dllexport) double opt_lb_put_gamma_mc_ci_low(double S0, double R, double sigma,
        double T0, double T, int paths, int steps, std::uint64_t seed);

    __declspec(dllexport) double opt_lb_put_gamma_mc_ci_high(double S0, double R, double sigma,
        double T0, double T, int paths, int steps, std::uint64_t seed);

    __declspec(dllexport) double opt_lb_put_gamma_mc_vr(double S0, double R, double sigma,
        double T0, double T, int paths, int steps, std::uint64_t seed);

    __declspec(dllexport) double opt_lb_put_gamma_mc_vr_se(double S0, double R, double sigma,
        double T0, double T, int paths, int steps, std::uint64_t seed);

    __declspec(dllexport) double opt_lb_put_gamma_mc_vr_ci_low(double S0, double R, double sigma,
        double T0, double T, int paths, int steps, std::uint64_t seed);

    __declspec(dllexport) double opt_lb_put_gamma_mc_vr_ci_high(double S0, double R, double sigma,
        double T0, double T, int paths, int steps, std::uint64_t seed);

    // ============================================================================
    //  LOOKBACK PUT — THETA (MC standard/VR) 
    // ============================================================================

    __declspec(dllexport) double opt_lb_put_theta_mc(double S0, double R, double sigma,
        double T0, double T, int paths, int steps, std::uint64_t seed);

    __declspec(dllexport) double opt_lb_put_theta_mc_se(double S0, double R, double sigma,
        double T0, double T, int paths, int steps, std::uint64_t seed);

    __declspec(dllexport) double opt_lb_put_theta_mc_ci_low(double S0, double R, double sigma,
        double T0, double T, int paths, int steps, std::uint64_t seed);

    __declspec(dllexport) double opt_lb_put_theta_mc_ci_high(double S0, double R, double sigma,
        double T0, double T, int paths, int steps, std::uint64_t seed);

    __declspec(dllexport) double opt_lb_put_theta_mc_vr(double S0, double R, double sigma,
        double T0, double T, int paths, int steps, std::uint64_t seed);

    __declspec(dllexport) double opt_lb_put_theta_mc_vr_se(double S0, double R, double sigma,
        double T0, double T, int paths, int steps, std::uint64_t seed);

    __declspec(dllexport) double opt_lb_put_theta_mc_vr_ci_low(double S0, double R, double sigma,
        double T0, double T, int paths, int steps, std::uint64_t seed);

    __declspec(dllexport) double opt_lb_put_theta_mc_vr_ci_high(double S0, double R, double sigma,
        double T0, double T, int paths, int steps, std::uint64_t seed);

    // ============================================================================
    //  LOOKBACK PUT — RHO (MC standard/VR) 
    // ============================================================================

    __declspec(dllexport) double opt_lb_put_rho_mc(double S0, double R, double sigma,
        double T0, double T, int paths, int steps, std::uint64_t seed);

    __declspec(dllexport) double opt_lb_put_rho_mc_se(double S0, double R, double sigma,
        double T0, double T, int paths, int steps, std::uint64_t seed);

    __declspec(dllexport) double opt_lb_put_rho_mc_ci_low(double S0, double R, double sigma,
        double T0, double T, int paths, int steps, std::uint64_t seed);

    __declspec(dllexport) double opt_lb_put_rho_mc_ci_high(double S0, double R, double sigma,
        double T0, double T, int paths, int steps, std::uint64_t seed);

    __declspec(dllexport) double opt_lb_put_rho_mc_vr(double S0, double R, double sigma,
        double T0, double T, int paths, int steps, std::uint64_t seed);

    __declspec(dllexport) double opt_lb_put_rho_mc_vr_se(double S0, double R, double sigma,
        double T0, double T, int paths, int steps, std::uint64_t seed);

    __declspec(dllexport) double opt_lb_put_rho_mc_vr_ci_low(double S0, double R, double sigma,
        double T0, double T, int paths, int steps, std::uint64_t seed);

    __declspec(dllexport) double opt_lb_put_rho_mc_vr_ci_high(double S0, double R, double sigma,
        double T0, double T, int paths, int steps, std::uint64_t seed);

    // ============================================================================
    //  LOOKBACK PUT — VEGA (MC standard/VR) 
    // ============================================================================

    __declspec(dllexport) double opt_lb_put_vega_mc(double S0, double R, double sigma,
        double T0, double T, int paths, int steps, std::uint64_t seed);

    __declspec(dllexport) double opt_lb_put_vega_mc_se(double S0, double R, double sigma,
        double T0, double T, int paths, int steps, std::uint64_t seed);

    __declspec(dllexport) double opt_lb_put_vega_mc_ci_low(double S0, double R, double sigma,
        double T0, double T, int paths, int steps, std::uint64_t seed);

    __declspec(dllexport) double opt_lb_put_vega_mc_ci_high(double S0, double R, double sigma,
        double T0, double T, int paths, int steps, std::uint64_t seed);

    __declspec(dllexport) double opt_lb_put_vega_mc_vr(double S0, double R, double sigma,
        double T0, double T, int paths, int steps, std::uint64_t seed);

    __declspec(dllexport) double opt_lb_put_vega_mc_vr_se(double S0, double R, double sigma,
        double T0, double T, int paths, int steps, std::uint64_t seed);

    __declspec(dllexport) double opt_lb_put_vega_mc_vr_ci_low(double S0, double R, double sigma,
        double T0, double T, int paths, int steps, std::uint64_t seed);

    __declspec(dllexport) double opt_lb_put_vega_mc_vr_ci_high(double S0, double R, double sigma,
        double T0, double T, int paths, int steps, std::uint64_t seed);

    // ============================================================================
    //  LOOKBACK PUT — PRIX ASYMPTOTIQUE (Brownian Bridge, LOOKBACK ONLY)
    // ============================================================================

    __declspec(dllexport) double opt_lb_put_price_bb_asymptotic(double S0, double R, double sigma,
        double T0, double T);

} // extern "C"

#endif // EXPORTS_H
