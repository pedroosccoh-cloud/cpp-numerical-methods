
#ifndef BISECTION_HPP
#define BISECTION_HPP

#include <cmath>
#include <stdexcept>

/**
 * @brief Stores the result of the bisection algorithm.
 *
 * @param root Approximation of the root.
 * @param iterations Number of iterations performed.
 * @param converged Whether a stopping criterion was satisfied.
 */
struct BisectionResult {
    double root;
    int iterations;
    bool converged;
};

/**
 * @brief Finds a root using the bisection method.
 *
 * The function must be continuous on [a, b], and the endpoint
 * values must have opposite signs unless an endpoint is a root.
 *
 * @param f Function whose root is being searched for.
 * @param a Lower endpoint of the initial interval.
 * @param b Upper endpoint of the initial interval.
 * @param tolerance Positive tolerance for the stopping criteria.
 * @param max_iterations Maximum number of iterations.
 *
 * @return BisectionResult containing the root approximation,
 *         iteration count, and convergence status.
 *
 * @throws std::invalid_argument If the interval, tolerance,
 *         iteration limit, endpoint values, or sign condition is invalid.
 * @throws std::runtime_error If a function evaluation during iteration
 *         produces a non-finite value.
 *
 * @note Convergence is declared when either the absolute residual
 *       |f(c)| or the half-width of the current interval is within
 *       the specified tolerance.
 */
template <typename Function>
BisectionResult bisection(
    Function f,
    double a,
    double b,
    double tolerance = 1e-8,
    int max_iterations = 100
) {
    // Validate the interval and ensure its endpoints are finite.
    if (!std::isfinite(a) || !std::isfinite(b) || a >= b) {
        throw std::invalid_argument("Invalid interval.");
    }

    // Validate the stopping criteria.
    if (!std::isfinite(tolerance) || tolerance <= 0.0 ||
        max_iterations <= 0) {
        throw std::invalid_argument("Invalid stopping criteria.");
    }

    // Evaluate the function at both endpoints.
    double fa = f(a);
    double fb = f(b);

    if (!std::isfinite(fa) || !std::isfinite(fb)) {
        throw std::invalid_argument("Non-finite endpoint value.");
    }

    // Handle roots located exactly at an endpoint.
    if (fa == 0.0) return {a, 0, true};
    if (fb == 0.0) return {b, 0, true};

    // A sign change brackets a root for a continuous function.
    if (std::signbit(fa) == std::signbit(fb)) {
        throw std::invalid_argument(
            "Interval must bracket a root."
        );
    }

    // Repeatedly halve the interval containing the root.
    for (int i = 1; i <= max_iterations; ++i) {
        // Compute the midpoint while reducing overflow risk.
        const double c = a + (b - a) / 2.0;
        const double fc = f(c);

        if (!std::isfinite(fc)) {
            throw std::runtime_error("Non-finite function value.");
        }

        // Stop if the residual or half-interval width is sufficiently small.
        if (std::abs(fc) <= tolerance ||
            (b - a) / 2.0 <= tolerance) {
            return {c, i, true};
        }

        // Preserve the half-interval where the sign changes.
        if (std::signbit(fa) != std::signbit(fc)) {
            b = c;
            fb = fc;
        } else {
            a = c;
            fa = fc;
        }
    }

    // Return the final midpoint, indicating that convergence
    // was not achieved within the iteration limit.
    return {a + (b - a) / 2.0, max_iterations, false};
}

#endif
