#include "pch.h"
#include "Exports.h"

//=============================================================================
// Factorisation du code 
//=============================================================================

static bool g_errorDisplayed = false;

extern "C" __declspec(dllexport) void __stdcall ResetErrorFlag()
{
    g_errorDisplayed = false;
}

/**
 * @brief Affiche une boîte d'erreur une seule fois.
 * @param msg Le message d'erreur à afficher.
 * @param title Le titre de la boîte de dialogue.
 * @return NaN pour signaler l'erreur au code appelant.
 */
double reportError(const char* msg, const char* title) {
    if (!g_errorDisplayed) {
        wchar_t wmsg[256], wtitle[256];
        MultiByteToWideChar(CP_UTF8, 0, msg, -1, wmsg, 256);
        MultiByteToWideChar(CP_UTF8, 0, title, -1, wtitle, 256);
        MessageBoxW(nullptr, wmsg, wtitle, MB_OK | MB_ICONERROR);
        g_errorDisplayed = true;
    }
    return std::nan("");
}

/**
 * @brief Déclare une fonction extern "C" __stdcall retournant un double protégée par try/catch.
 * @param name Identifiant de la fonction exportée.
 * @param args Signature (entre parenthèses) de la fonction.
 * @param body Code à exécuter (doit inclure un return).
 */
#define SAFE_DOUBLE(name, args, body)               \
extern "C" double __stdcall name args {             \
    try { body; }                                   \
    catch(const std::exception& e) {                \
        return reportError(e.what(), #name);        \
    }                                               \
    catch(...) {                                    \
        return reportError("Erreur inconnue dans " #name, #name); \
    }                                               \
}

// Helper de construction
inline opt::Asian<opt::PayoffCall, opt::LookMin>
    makeLookbackCall(double S0, double R, double sigma, double T0, double T)
{
    return opt::Asian<opt::PayoffCall, opt::LookMin>(S0, R, sigma, T0, T, opt::PayoffCall(), opt::LookMin());
}

inline opt::Asian<opt::PayoffPut, opt::LookMax>
    makeLookbackPut(double S0, double R, double sigma, double T0, double T)
{
    return opt::Asian<opt::PayoffPut, opt::LookMax>(S0, R, sigma, T0, T, opt::PayoffPut(), opt::LookMax());
}

// ============================================================================
//  LOOKBACK CALL — PRIX (MC standard)
// ============================================================================

SAFE_DOUBLE(opt_lb_call_price_mc, 
    (double S0, double R, double sigma, double T0, double T, int paths, int steps, std::uint64_t seed),
    {
        auto stats = makeLookbackCall(S0, R, sigma, T0, T).priceMC(paths, steps, seed, false);
        return stats.estimate;
    }
)

SAFE_DOUBLE(opt_lb_call_price_mc_se,
    (double S0, double R, double sigma, double T0, double T, int paths, int steps, std::uint64_t seed),
    {
        auto stats = makeLookbackCall(S0, R, sigma, T0, T).priceMC(paths, steps, seed, false);
        return stats.stdError;
    }
)

SAFE_DOUBLE(opt_lb_call_price_mc_ci_low,
    (double S0, double R, double sigma, double T0, double T, int paths, int steps, std::uint64_t seed),
    {
        auto stats = makeLookbackCall(S0, R, sigma, T0, T).priceMC(paths, steps, seed, false);
        return stats.ciLow;
    }
)

SAFE_DOUBLE(opt_lb_call_price_mc_ci_high,
    (double S0, double R, double sigma, double T0, double T, int paths, int steps, std::uint64_t seed),
    {
        auto stats = makeLookbackCall(S0, R, sigma, T0, T).priceMC(paths, steps, seed, false);
        return stats.ciHigh;
    }
)

// ============================================================================
//  LOOKBACK CALL — PRIX (MC VR : antithétiques)
// ============================================================================

SAFE_DOUBLE(opt_lb_call_price_mc_vr,
    (double S0, double R, double sigma, double T0, double T, int paths, int steps, std::uint64_t seed),
    {
        auto stats = makeLookbackCall(S0, R, sigma, T0, T).priceMC(paths, steps, seed, true);
        return stats.estimate;
    }
)

SAFE_DOUBLE(opt_lb_call_price_mc_vr_se,
    (double S0, double R, double sigma, double T0, double T, int paths, int steps, std::uint64_t seed),
    {
        auto stats = makeLookbackCall(S0, R, sigma, T0, T).priceMC(paths, steps, seed, true);
        return stats.stdError;
    }
)

SAFE_DOUBLE(opt_lb_call_price_mc_vr_ci_low,
    (double S0, double R, double sigma, double T0, double T, int paths, int steps, std::uint64_t seed),
    {
        auto stats = makeLookbackCall(S0, R, sigma, T0, T).priceMC(paths, steps, seed, true);
        return stats.ciLow;
    }
)

SAFE_DOUBLE(opt_lb_call_price_mc_vr_ci_high,
    (double S0, double R, double sigma, double T0, double T, int paths, int steps, std::uint64_t seed),
    {
        auto stats = makeLookbackCall(S0, R, sigma, T0, T).priceMC(paths, steps, seed, true);
        return stats.ciHigh;
    }
)

// ============================================================================
//  LOOKBACK CALL — DELTA (MC standard/VR)
// ============================================================================

SAFE_DOUBLE(opt_lb_call_delta_mc,
    (double S0, double R, double sigma, double T0, double T, int paths, int steps, std::uint64_t seed),
    {
        auto stats = makeLookbackCall(S0, R, sigma, T0, T).deltaMC(paths, steps, seed, false);
        return stats.estimate;
    }
)

SAFE_DOUBLE(opt_lb_call_delta_mc_se,
    (double S0, double R, double sigma, double T0, double T, int paths, int steps, std::uint64_t seed),
    {
        auto stats = makeLookbackCall(S0, R, sigma, T0, T).deltaMC(paths, steps, seed, false);
        return stats.stdError;
    }
)

SAFE_DOUBLE(opt_lb_call_delta_mc_ci_low,
    (double S0, double R, double sigma, double T0, double T, int paths, int steps, std::uint64_t seed),
    {
        auto stats = makeLookbackCall(S0, R, sigma, T0, T).deltaMC(paths, steps, seed, false);
        return stats.ciLow;
    }
)

SAFE_DOUBLE(opt_lb_call_delta_mc_ci_high,
    (double S0, double R, double sigma, double T0, double T, int paths, int steps, std::uint64_t seed),
    {
        auto stats = makeLookbackCall(S0, R, sigma, T0, T).deltaMC(paths, steps, seed, false);
        return stats.ciHigh;
    }
)

SAFE_DOUBLE(opt_lb_call_delta_mc_vr,
    (double S0, double R, double sigma, double T0, double T, int paths, int steps, std::uint64_t seed),
    {
        auto stats = makeLookbackCall(S0, R, sigma, T0, T).deltaMC(paths, steps, seed, true);
        return stats.estimate;
    }
)

SAFE_DOUBLE(opt_lb_call_delta_mc_vr_se,
    (double S0, double R, double sigma, double T0, double T, int paths, int steps, std::uint64_t seed),
    {
        auto stats = makeLookbackCall(S0, R, sigma, T0, T).deltaMC(paths, steps, seed, true);
        return stats.stdError;
    }
)

SAFE_DOUBLE(opt_lb_call_delta_mc_vr_ci_low,
    (double S0, double R, double sigma, double T0, double T, int paths, int steps, std::uint64_t seed),
    {
        auto stats = makeLookbackCall(S0, R, sigma, T0, T).deltaMC(paths, steps, seed, true);
        return stats.ciLow;
    }
)

SAFE_DOUBLE(opt_lb_call_delta_mc_vr_ci_high,
    (double S0, double R, double sigma, double T0, double T, int paths, int steps, std::uint64_t seed),
    {
        auto stats = makeLookbackCall(S0, R, sigma, T0, T).deltaMC(paths, steps, seed, true);
        return stats.ciHigh;
    }
)

// ============================================================================
//  LOOKBACK CALL — GAMMA (MC standard/VR)
// ============================================================================

SAFE_DOUBLE(opt_lb_call_gamma_mc,
    (double S0, double R, double sigma, double T0, double T, int paths, int steps, std::uint64_t seed),
    {
        auto stats = makeLookbackCall(S0, R, sigma, T0, T).gammaMC(paths, steps, seed, false);
        return stats.estimate;
    }
)

SAFE_DOUBLE(opt_lb_call_gamma_mc_se,
    (double S0, double R, double sigma, double T0, double T, int paths, int steps, std::uint64_t seed),
    {
        auto stats = makeLookbackCall(S0, R, sigma, T0, T).gammaMC(paths, steps, seed, false);
        return stats.stdError;
    }
)

SAFE_DOUBLE(opt_lb_call_gamma_mc_ci_low,
    (double S0, double R, double sigma, double T0, double T, int paths, int steps, std::uint64_t seed),
    {
        auto stats = makeLookbackCall(S0, R, sigma, T0, T).gammaMC(paths, steps, seed, false);
        return stats.ciLow;
    }
)

SAFE_DOUBLE(opt_lb_call_gamma_mc_ci_high,
    (double S0, double R, double sigma, double T0, double T, int paths, int steps, std::uint64_t seed),
    {
        auto stats = makeLookbackCall(S0, R, sigma, T0, T).gammaMC(paths, steps, seed, false);
        return stats.ciHigh;
    }
)

SAFE_DOUBLE(opt_lb_call_gamma_mc_vr,
    (double S0, double R, double sigma, double T0, double T, int paths, int steps, std::uint64_t seed),
    {
        auto stats = makeLookbackCall(S0, R, sigma, T0, T).gammaMC(paths, steps, seed, true);
        return stats.estimate;
    }
)

SAFE_DOUBLE(opt_lb_call_gamma_mc_vr_se,
    (double S0, double R, double sigma, double T0, double T, int paths, int steps, std::uint64_t seed),
    {
        auto stats = makeLookbackCall(S0, R, sigma, T0, T).gammaMC(paths, steps, seed, true);
        return stats.stdError;
    }
)

SAFE_DOUBLE(opt_lb_call_gamma_mc_vr_ci_low,
    (double S0, double R, double sigma, double T0, double T, int paths, int steps, std::uint64_t seed),
    {
        auto stats = makeLookbackCall(S0, R, sigma, T0, T).gammaMC(paths, steps, seed, true);
        return stats.ciLow;
    }
)

SAFE_DOUBLE(opt_lb_call_gamma_mc_vr_ci_high,
    (double S0, double R, double sigma, double T0, double T, int paths, int steps, std::uint64_t seed),
    {
        auto stats = makeLookbackCall(S0, R, sigma, T0, T).gammaMC(paths, steps, seed, true);
        return stats.ciHigh;
    }
)

// ============================================================================
//  LOOKBACK CALL — THETA (MC standard/VR)
// ============================================================================

SAFE_DOUBLE(opt_lb_call_theta_mc,
    (double S0, double R, double sigma, double T0, double T, int paths, int steps, std::uint64_t seed),
    {
        auto stats = makeLookbackCall(S0, R, sigma, T0, T).thetaMC(paths, steps, seed, false);
        return stats.estimate;
    }
)

SAFE_DOUBLE(opt_lb_call_theta_mc_se,
    (double S0, double R, double sigma, double T0, double T, int paths, int steps, std::uint64_t seed),
    {
        auto stats = makeLookbackCall(S0, R, sigma, T0, T).thetaMC(paths, steps, seed, false);
        return stats.stdError;
    }
)

SAFE_DOUBLE(opt_lb_call_theta_mc_ci_low,
    (double S0, double R, double sigma, double T0, double T, int paths, int steps, std::uint64_t seed),
    {
        auto stats = makeLookbackCall(S0, R, sigma, T0, T).thetaMC(paths, steps, seed, false);
        return stats.ciLow;
    }
)

SAFE_DOUBLE(opt_lb_call_theta_mc_ci_high,
    (double S0, double R, double sigma, double T0, double T, int paths, int steps, std::uint64_t seed),
    {
        auto stats = makeLookbackCall(S0, R, sigma, T0, T).thetaMC(paths, steps, seed, false);
        return stats.ciHigh;
    }
)

SAFE_DOUBLE(opt_lb_call_theta_mc_vr,
    (double S0, double R, double sigma, double T0, double T, int paths, int steps, std::uint64_t seed),
    {
        auto stats = makeLookbackCall(S0, R, sigma, T0, T).thetaMC(paths, steps, seed, true);
        return stats.estimate;
    }
)

SAFE_DOUBLE(opt_lb_call_theta_mc_vr_se,
    (double S0, double R, double sigma, double T0, double T, int paths, int steps, std::uint64_t seed),
    {
        auto stats = makeLookbackCall(S0, R, sigma, T0, T).thetaMC(paths, steps, seed, true);
        return stats.stdError;
    }
)

SAFE_DOUBLE(opt_lb_call_theta_mc_vr_ci_low,
    (double S0, double R, double sigma, double T0, double T, int paths, int steps, std::uint64_t seed),
    {
        auto stats = makeLookbackCall(S0, R, sigma, T0, T).thetaMC(paths, steps, seed, true);
        return stats.ciLow;
    }
)

SAFE_DOUBLE(opt_lb_call_theta_mc_vr_ci_high,
    (double S0, double R, double sigma, double T0, double T, int paths, int steps, std::uint64_t seed),
    {
        auto stats = makeLookbackCall(S0, R, sigma, T0, T).thetaMC(paths, steps, seed, true);
        return stats.ciHigh;
    }
)

// ============================================================================
//  LOOKBACK CALL — RHO (MC standard/VR)
// ============================================================================

SAFE_DOUBLE(opt_lb_call_rho_mc,
    (double S0, double R, double sigma, double T0, double T, int paths, int steps, std::uint64_t seed),
    {
        auto stats = makeLookbackCall(S0, R, sigma, T0, T).rhoMC(paths, steps, seed, false);
        return stats.estimate;
    }
)

SAFE_DOUBLE(opt_lb_call_rho_mc_se,
    (double S0, double R, double sigma, double T0, double T, int paths, int steps, std::uint64_t seed),
    {
        auto stats = makeLookbackCall(S0, R, sigma, T0, T).rhoMC(paths, steps, seed, false);
        return stats.stdError;
    }
)

SAFE_DOUBLE(opt_lb_call_rho_mc_ci_low,
    (double S0, double R, double sigma, double T0, double T, int paths, int steps, std::uint64_t seed),
    {
        auto stats = makeLookbackCall(S0, R, sigma, T0, T).rhoMC(paths, steps, seed, false);
        return stats.ciLow;
    }
)

SAFE_DOUBLE(opt_lb_call_rho_mc_ci_high,
    (double S0, double R, double sigma, double T0, double T, int paths, int steps, std::uint64_t seed),
    {
        auto stats = makeLookbackCall(S0, R, sigma, T0, T).rhoMC(paths, steps, seed, false);
        return stats.ciHigh;
    }
)

SAFE_DOUBLE(opt_lb_call_rho_mc_vr,
    (double S0, double R, double sigma, double T0, double T, int paths, int steps, std::uint64_t seed),
    {
        auto stats = makeLookbackCall(S0, R, sigma, T0, T).rhoMC(paths, steps, seed, true);
        return stats.estimate;
    }
)

SAFE_DOUBLE(opt_lb_call_rho_mc_vr_se,
    (double S0, double R, double sigma, double T0, double T, int paths, int steps, std::uint64_t seed),
    {
        auto stats = makeLookbackCall(S0, R, sigma, T0, T).rhoMC(paths, steps, seed, true);
        return stats.stdError;
    }
)

SAFE_DOUBLE(opt_lb_call_rho_mc_vr_ci_low,
    (double S0, double R, double sigma, double T0, double T, int paths, int steps, std::uint64_t seed),
    {
        auto stats = makeLookbackCall(S0, R, sigma, T0, T).rhoMC(paths, steps, seed, true);
        return stats.ciLow;
    }
)

SAFE_DOUBLE(opt_lb_call_rho_mc_vr_ci_high,
    (double S0, double R, double sigma, double T0, double T, int paths, int steps, std::uint64_t seed),
    {
        auto stats = makeLookbackCall(S0, R, sigma, T0, T).rhoMC(paths, steps, seed, true);
        return stats.ciHigh;
    }
)

// ============================================================================
//  LOOKBACK CALL — VEGA (MC standard/VR)
// ============================================================================

SAFE_DOUBLE(opt_lb_call_vega_mc,
    (double S0, double R, double sigma, double T0, double T, int paths, int steps, std::uint64_t seed),
    {
        auto stats = makeLookbackCall(S0, R, sigma, T0, T).vegaMC(paths, steps, seed, false);
        return stats.estimate;
    }
)

SAFE_DOUBLE(opt_lb_call_vega_mc_se,
    (double S0, double R, double sigma, double T0, double T, int paths, int steps, std::uint64_t seed),
    {
        auto stats = makeLookbackCall(S0, R, sigma, T0, T).vegaMC(paths, steps, seed, false);
        return stats.stdError;
    }
)

SAFE_DOUBLE(opt_lb_call_vega_mc_ci_low,
    (double S0, double R, double sigma, double T0, double T, int paths, int steps, std::uint64_t seed),
    {
        auto stats = makeLookbackCall(S0, R, sigma, T0, T).vegaMC(paths, steps, seed, false);
        return stats.ciLow;
    }
)

SAFE_DOUBLE(opt_lb_call_vega_mc_ci_high,
    (double S0, double R, double sigma, double T0, double T, int paths, int steps, std::uint64_t seed),
    {
        auto stats = makeLookbackCall(S0, R, sigma, T0, T).vegaMC(paths, steps, seed, false);
        return stats.ciHigh;
    }
)

SAFE_DOUBLE(opt_lb_call_vega_mc_vr,
    (double S0, double R, double sigma, double T0, double T, int paths, int steps, std::uint64_t seed),
    {
        auto stats = makeLookbackCall(S0, R, sigma, T0, T).vegaMC(paths, steps, seed, true);
        return stats.estimate;
    }
)

SAFE_DOUBLE(opt_lb_call_vega_mc_vr_se,
    (double S0, double R, double sigma, double T0, double T, int paths, int steps, std::uint64_t seed),
    {
        auto stats = makeLookbackCall(S0, R, sigma, T0, T).vegaMC(paths, steps, seed, true);
        return stats.stdError;
    }
)

SAFE_DOUBLE(opt_lb_call_vega_mc_vr_ci_low,
    (double S0, double R, double sigma, double T0, double T, int paths, int steps, std::uint64_t seed),
    {
        auto stats = makeLookbackCall(S0, R, sigma, T0, T).vegaMC(paths, steps, seed, true);
        return stats.ciLow;
    }
)

SAFE_DOUBLE(opt_lb_call_vega_mc_vr_ci_high,
    (double S0, double R, double sigma, double T0, double T, int paths, int steps, std::uint64_t seed),
    {
        auto stats = makeLookbackCall(S0, R, sigma, T0, T).vegaMC(paths, steps, seed, true);
        return stats.ciHigh;
    }
)

// ============================================================================
//  LOOKBACK CALL — PRIX ASYMPTOTIQUE (Brownian Bridge, LOOKBACK ONLY)
// ============================================================================

SAFE_DOUBLE(opt_lb_call_price_bb_asymptotic,
    (double S0, double R, double sigma, double T0, double T),
    {
        return makeLookbackCall(S0, R, sigma, T0, T).priceMC_BrownianBridge_Asymptotic();
    }
)

// ============================================================================
//  LOOKBACK PUT — PRIX (MC standard)
// ============================================================================

SAFE_DOUBLE(opt_lb_put_price_mc,
    (double S0, double R, double sigma, double T0, double T, int paths, int steps, std::uint64_t seed),
    {
        auto stats = makeLookbackPut(S0, R, sigma, T0, T).priceMC(paths, steps, seed, false);
        return stats.estimate;
    }
)

SAFE_DOUBLE(opt_lb_put_price_mc_se,
    (double S0, double R, double sigma, double T0, double T, int paths, int steps, std::uint64_t seed),
    {
        auto stats = makeLookbackPut(S0, R, sigma, T0, T).priceMC(paths, steps, seed, false);
        return stats.stdError;
    }
)

SAFE_DOUBLE(opt_lb_put_price_mc_ci_low,
    (double S0, double R, double sigma, double T0, double T, int paths, int steps, std::uint64_t seed),
    {
        auto stats = makeLookbackPut(S0, R, sigma, T0, T).priceMC(paths, steps, seed, false);
        return stats.ciLow;
    }
)

SAFE_DOUBLE(opt_lb_put_price_mc_ci_high,
    (double S0, double R, double sigma, double T0, double T, int paths, int steps, std::uint64_t seed),
    {
        auto stats = makeLookbackPut(S0, R, sigma, T0, T).priceMC(paths, steps, seed, false);
        return stats.ciHigh;
    }
)

// ============================================================================
//  LOOKBACK PUT — PRIX (MC VR : antithétiques)
// ============================================================================

SAFE_DOUBLE(opt_lb_put_price_mc_vr,
    (double S0, double R, double sigma, double T0, double T, int paths, int steps, std::uint64_t seed),
    {
        auto stats = makeLookbackPut(S0, R, sigma, T0, T).priceMC(paths, steps, seed, true);
        return stats.estimate;
    }
)

SAFE_DOUBLE(opt_lb_put_price_mc_vr_se,
    (double S0, double R, double sigma, double T0, double T, int paths, int steps, std::uint64_t seed),
    {
        auto stats = makeLookbackPut(S0, R, sigma, T0, T).priceMC(paths, steps, seed, true);
        return stats.stdError;
    }
)

SAFE_DOUBLE(opt_lb_put_price_mc_vr_ci_low,
    (double S0, double R, double sigma, double T0, double T, int paths, int steps, std::uint64_t seed),
    {
        auto stats = makeLookbackPut(S0, R, sigma, T0, T).priceMC(paths, steps, seed, true);
        return stats.ciLow;
    }
)

SAFE_DOUBLE(opt_lb_put_price_mc_vr_ci_high,
    (double S0, double R, double sigma, double T0, double T, int paths, int steps, std::uint64_t seed),
    {
        auto stats = makeLookbackPut(S0, R, sigma, T0, T).priceMC(paths, steps, seed, true);
        return stats.ciHigh;
    }
)

// ============================================================================
//  LOOKBACK PUT — DELTA (MC standard/VR)
// ============================================================================

SAFE_DOUBLE(opt_lb_put_delta_mc,
    (double S0, double R, double sigma, double T0, double T, int paths, int steps, std::uint64_t seed),
    {
        auto stats = makeLookbackPut(S0, R, sigma, T0, T).deltaMC(paths, steps, seed, false);
        return stats.estimate;
    }
)

SAFE_DOUBLE(opt_lb_put_delta_mc_se,
    (double S0, double R, double sigma, double T0, double T, int paths, int steps, std::uint64_t seed),
    {
        auto stats = makeLookbackPut(S0, R, sigma, T0, T).deltaMC(paths, steps, seed, false);
        return stats.stdError;
    }
)

SAFE_DOUBLE(opt_lb_put_delta_mc_ci_low,
    (double S0, double R, double sigma, double T0, double T, int paths, int steps, std::uint64_t seed),
    {
        auto stats = makeLookbackPut(S0, R, sigma, T0, T).deltaMC(paths, steps, seed, false);
        return stats.ciLow;
    }
)

SAFE_DOUBLE(opt_lb_put_delta_mc_ci_high,
    (double S0, double R, double sigma, double T0, double T, int paths, int steps, std::uint64_t seed),
    {
        auto stats = makeLookbackPut(S0, R, sigma, T0, T).deltaMC(paths, steps, seed, false);
        return stats.ciHigh;
    }
)

SAFE_DOUBLE(opt_lb_put_delta_mc_vr,
    (double S0, double R, double sigma, double T0, double T, int paths, int steps, std::uint64_t seed),
    {
        auto stats = makeLookbackPut(S0, R, sigma, T0, T).deltaMC(paths, steps, seed, true);
        return stats.estimate;
    }
)

SAFE_DOUBLE(opt_lb_put_delta_mc_vr_se,
    (double S0, double R, double sigma, double T0, double T, int paths, int steps, std::uint64_t seed),
    {
        auto stats = makeLookbackPut(S0, R, sigma, T0, T).deltaMC(paths, steps, seed, true);
        return stats.stdError;
    }
)

SAFE_DOUBLE(opt_lb_put_delta_mc_vr_ci_low,
    (double S0, double R, double sigma, double T0, double T, int paths, int steps, std::uint64_t seed),
    {
        auto stats = makeLookbackPut(S0, R, sigma, T0, T).deltaMC(paths, steps, seed, true);
        return stats.ciLow;
    }
)

SAFE_DOUBLE(opt_lb_put_delta_mc_vr_ci_high,
    (double S0, double R, double sigma, double T0, double T, int paths, int steps, std::uint64_t seed),
    {
        auto stats = makeLookbackPut(S0, R, sigma, T0, T).deltaMC(paths, steps, seed, true);
        return stats.ciHigh;
    }
)

// ============================================================================
//  LOOKBACK PUT — GAMMA (MC standard/VR)
// ============================================================================

SAFE_DOUBLE(opt_lb_put_gamma_mc,
    (double S0, double R, double sigma, double T0, double T, int paths, int steps, std::uint64_t seed),
    {
        auto stats = makeLookbackPut(S0, R, sigma, T0, T).gammaMC(paths, steps, seed, false);
        return stats.estimate;
    }
)

SAFE_DOUBLE(opt_lb_put_gamma_mc_se,
    (double S0, double R, double sigma, double T0, double T, int paths, int steps, std::uint64_t seed),
    {
        auto stats = makeLookbackPut(S0, R, sigma, T0, T).gammaMC(paths, steps, seed, false);
        return stats.stdError;
    }
)

SAFE_DOUBLE(opt_lb_put_gamma_mc_ci_low,
    (double S0, double R, double sigma, double T0, double T, int paths, int steps, std::uint64_t seed),
    {
        auto stats = makeLookbackPut(S0, R, sigma, T0, T).gammaMC(paths, steps, seed, false);
        return stats.ciLow;
    }
)

SAFE_DOUBLE(opt_lb_put_gamma_mc_ci_high,
    (double S0, double R, double sigma, double T0, double T, int paths, int steps, std::uint64_t seed),
    {
        auto stats = makeLookbackPut(S0, R, sigma, T0, T).gammaMC(paths, steps, seed, false);
        return stats.ciHigh;
    }
)

SAFE_DOUBLE(opt_lb_put_gamma_mc_vr,
    (double S0, double R, double sigma, double T0, double T, int paths, int steps, std::uint64_t seed),
    {
        auto stats = makeLookbackPut(S0, R, sigma, T0, T).gammaMC(paths, steps, seed, true);
        return stats.estimate;
    }
)

SAFE_DOUBLE(opt_lb_put_gamma_mc_vr_se,
    (double S0, double R, double sigma, double T0, double T, int paths, int steps, std::uint64_t seed),
    {
        auto stats = makeLookbackPut(S0, R, sigma, T0, T).gammaMC(paths, steps, seed, true);
        return stats.stdError;
    }
)

SAFE_DOUBLE(opt_lb_put_gamma_mc_vr_ci_low,
    (double S0, double R, double sigma, double T0, double T, int paths, int steps, std::uint64_t seed),
    {
        auto stats = makeLookbackPut(S0, R, sigma, T0, T).gammaMC(paths, steps, seed, true);
        return stats.ciLow;
    }
)

SAFE_DOUBLE(opt_lb_put_gamma_mc_vr_ci_high,
    (double S0, double R, double sigma, double T0, double T, int paths, int steps, std::uint64_t seed),
    {
        auto stats = makeLookbackPut(S0, R, sigma, T0, T).gammaMC(paths, steps, seed, true);
        return stats.ciHigh;
    }
)

// ============================================================================
//  LOOKBACK PUT — THETA (MC standard/VR)
// ============================================================================

SAFE_DOUBLE(opt_lb_put_theta_mc,
    (double S0, double R, double sigma, double T0, double T, int paths, int steps, std::uint64_t seed),
    {
        auto stats = makeLookbackPut(S0, R, sigma, T0, T).thetaMC(paths, steps, seed, false);
        return stats.estimate;
    }
)

SAFE_DOUBLE(opt_lb_put_theta_mc_se,
    (double S0, double R, double sigma, double T0, double T, int paths, int steps, std::uint64_t seed),
    {
        auto stats = makeLookbackPut(S0, R, sigma, T0, T).thetaMC(paths, steps, seed, false);
        return stats.stdError;
    }
)

SAFE_DOUBLE(opt_lb_put_theta_mc_ci_low,
    (double S0, double R, double sigma, double T0, double T, int paths, int steps, std::uint64_t seed),
    {
        auto stats = makeLookbackPut(S0, R, sigma, T0, T).thetaMC(paths, steps, seed, false);
        return stats.ciLow;
    }
)

SAFE_DOUBLE(opt_lb_put_theta_mc_ci_high,
    (double S0, double R, double sigma, double T0, double T, int paths, int steps, std::uint64_t seed),
    {
        auto stats = makeLookbackPut(S0, R, sigma, T0, T).thetaMC(paths, steps, seed, false);
        return stats.ciHigh;
    }
)

SAFE_DOUBLE(opt_lb_put_theta_mc_vr,
    (double S0, double R, double sigma, double T0, double T, int paths, int steps, std::uint64_t seed),
    {
        auto stats = makeLookbackPut(S0, R, sigma, T0, T).thetaMC(paths, steps, seed, true);
        return stats.estimate;
    }
)

SAFE_DOUBLE(opt_lb_put_theta_mc_vr_se,
    (double S0, double R, double sigma, double T0, double T, int paths, int steps, std::uint64_t seed),
    {
        auto stats = makeLookbackPut(S0, R, sigma, T0, T).thetaMC(paths, steps, seed, true);
        return stats.stdError;
    }
)

SAFE_DOUBLE(opt_lb_put_theta_mc_vr_ci_low,
    (double S0, double R, double sigma, double T0, double T, int paths, int steps, std::uint64_t seed),
    {
        auto stats = makeLookbackPut(S0, R, sigma, T0, T).thetaMC(paths, steps, seed, true);
        return stats.ciLow;
    }
)

SAFE_DOUBLE(opt_lb_put_theta_mc_vr_ci_high,
    (double S0, double R, double sigma, double T0, double T, int paths, int steps, std::uint64_t seed),
    {
        auto stats = makeLookbackPut(S0, R, sigma, T0, T).thetaMC(paths, steps, seed, true);
        return stats.ciHigh;
    }
)

// ============================================================================
//  LOOKBACK PUT — RHO (MC standard/VR)
// ============================================================================

SAFE_DOUBLE(opt_lb_put_rho_mc,
    (double S0, double R, double sigma, double T0, double T, int paths, int steps, std::uint64_t seed),
    {
        auto stats = makeLookbackPut(S0, R, sigma, T0, T).rhoMC(paths, steps, seed, false);
        return stats.estimate;
    }
)

SAFE_DOUBLE(opt_lb_put_rho_mc_se,
    (double S0, double R, double sigma, double T0, double T, int paths, int steps, std::uint64_t seed),
    {
        auto stats = makeLookbackPut(S0, R, sigma, T0, T).rhoMC(paths, steps, seed, false);
        return stats.stdError;
    }
)

SAFE_DOUBLE(opt_lb_put_rho_mc_ci_low,
    (double S0, double R, double sigma, double T0, double T, int paths, int steps, std::uint64_t seed),
    {
        auto stats = makeLookbackPut(S0, R, sigma, T0, T).rhoMC(paths, steps, seed, false);
        return stats.ciLow;
    }
)

SAFE_DOUBLE(opt_lb_put_rho_mc_ci_high,
    (double S0, double R, double sigma, double T0, double T, int paths, int steps, std::uint64_t seed),
    {
        auto stats = makeLookbackPut(S0, R, sigma, T0, T).rhoMC(paths, steps, seed, false);
        return stats.ciHigh;
    }
)

SAFE_DOUBLE(opt_lb_put_rho_mc_vr,
    (double S0, double R, double sigma, double T0, double T, int paths, int steps, std::uint64_t seed),
    {
        auto stats = makeLookbackPut(S0, R, sigma, T0, T).rhoMC(paths, steps, seed, true);
        return stats.estimate;
    }
)

SAFE_DOUBLE(opt_lb_put_rho_mc_vr_se,
    (double S0, double R, double sigma, double T0, double T, int paths, int steps, std::uint64_t seed),
    {
        auto stats = makeLookbackPut(S0, R, sigma, T0, T).rhoMC(paths, steps, seed, true);
        return stats.stdError;
    }
)

SAFE_DOUBLE(opt_lb_put_rho_mc_vr_ci_low,
    (double S0, double R, double sigma, double T0, double T, int paths, int steps, std::uint64_t seed),
    {
        auto stats = makeLookbackPut(S0, R, sigma, T0, T).rhoMC(paths, steps, seed, true);
        return stats.ciLow;
    }
)

SAFE_DOUBLE(opt_lb_put_rho_mc_vr_ci_high,
    (double S0, double R, double sigma, double T0, double T, int paths, int steps, std::uint64_t seed),
    {
        auto stats = makeLookbackPut(S0, R, sigma, T0, T).rhoMC(paths, steps, seed, true);
        return stats.ciHigh;
    }
)

// ============================================================================
//  LOOKBACK CALL — VEGA (MC standard/VR)
// ============================================================================

SAFE_DOUBLE(opt_lb_put_vega_mc,
    (double S0, double R, double sigma, double T0, double T, int paths, int steps, std::uint64_t seed),
    {
        auto stats = makeLookbackPut(S0, R, sigma, T0, T).vegaMC(paths, steps, seed, false);
        return stats.estimate;
    }
)

SAFE_DOUBLE(opt_lb_put_vega_mc_se,
    (double S0, double R, double sigma, double T0, double T, int paths, int steps, std::uint64_t seed),
    {
        auto stats = makeLookbackPut(S0, R, sigma, T0, T).vegaMC(paths, steps, seed, false);
        return stats.stdError;
    }
)

SAFE_DOUBLE(opt_lb_put_vega_mc_ci_low,
    (double S0, double R, double sigma, double T0, double T, int paths, int steps, std::uint64_t seed),
    {
        auto stats = makeLookbackPut(S0, R, sigma, T0, T).vegaMC(paths, steps, seed, false);
        return stats.ciLow;
    }
)

SAFE_DOUBLE(opt_lb_put_vega_mc_ci_high,
    (double S0, double R, double sigma, double T0, double T, int paths, int steps, std::uint64_t seed),
    {
        auto stats = makeLookbackPut(S0, R, sigma, T0, T).vegaMC(paths, steps, seed, false);
        return stats.ciHigh;
    }
)

SAFE_DOUBLE(opt_lb_put_vega_mc_vr,
    (double S0, double R, double sigma, double T0, double T, int paths, int steps, std::uint64_t seed),
    {
        auto stats = makeLookbackPut(S0, R, sigma, T0, T).vegaMC(paths, steps, seed, true);
        return stats.estimate;
    }
)

SAFE_DOUBLE(opt_lb_put_vega_mc_vr_se,
    (double S0, double R, double sigma, double T0, double T, int paths, int steps, std::uint64_t seed),
    {
        auto stats = makeLookbackPut(S0, R, sigma, T0, T).vegaMC(paths, steps, seed, true);
        return stats.stdError;
    }
)

SAFE_DOUBLE(opt_lb_put_vega_mc_vr_ci_low,
    (double S0, double R, double sigma, double T0, double T, int paths, int steps, std::uint64_t seed),
    {
        auto stats = makeLookbackPut(S0, R, sigma, T0, T).vegaMC(paths, steps, seed, true);
        return stats.ciLow;
    }
)

SAFE_DOUBLE(opt_lb_put_vega_mc_vr_ci_high,
    (double S0, double R, double sigma, double T0, double T, int paths, int steps, std::uint64_t seed),
    {
        auto stats = makeLookbackPut(S0, R, sigma, T0, T).vegaMC(paths, steps, seed, true);
        return stats.ciHigh;
    }
)

// ============================================================================
//  LOOKBACK PUT — PRIX ASYMPTOTIQUE (Brownian Bridge, LOOKBACK ONLY)
// ============================================================================

SAFE_DOUBLE(opt_lb_put_price_bb_asymptotic,
    (double S0, double R, double sigma, double T0, double T),
    {
        return makeLookbackPut(S0, R, sigma, T0, T).priceMC_BrownianBridge_Asymptotic();
    }
)