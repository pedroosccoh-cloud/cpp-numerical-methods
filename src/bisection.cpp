#include <cmath>
#include <iomanip>
#include <iostream>
#include <stdexcept>

// Stores the result returned by the bisection algorithm.
struct BisectionResult {
    double root;       // Approximation of the root.
    int iterations;    // Number of iterations performed.
    bool converged;    // True if a stopping criterion was satisfied.
};

/**
 * @brief Finds a root of a continuous function using the bisection method.
 *
 * The initial interval [a, b] must contain a root, verified by opposite
 * signs at its endpoints (unless an endpoint is already a root).
 *
 * @param f Function whose root is being searched for.
 * @param a Lower endpoint of the interval.
 * @param b Upper endpoint of the interval.
 * @param tolerance Maximum accepted function residual or half-interval width.
 * @param max_iterations Maximum number of iterations allowed.
 * @return BisectionResult containing the root approximation, iteration count,
 *         and convergence status.
 * @throws std::invalid_argument If the interval or stopping criteria are invalid,
 *         or if the initial interval does not bracket a root.
 * @throws std::runtime_error If the function returns a non-finite value
 *         during iteration.
 *
 * @note The bisection method assumes that f is continuous on [a, b].
 */
template <typename Function>
BisectionResult bisection(
    Function f,
    double a,
    double b,
    double tolerance = 1e-8,
    int max_iterations = 100
) {
    // Validate the interval.
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

    // Return immediately if either endpoint is already a root.
    if (fa == 0.0) return {a, 0, true};
    if (fb == 0.0) return {b, 0, true};

    // A sign change is required to guarantee a root for a continuous function.
    if (std::signbit(fa) == std::signbit(fb)) {
        throw std::invalid_argument(
            "Interval must bracket a root."
        );
    }

    // Repeatedly halve the interval to narrow down the root.
    for (int i = 1; i <= max_iterations; ++i) {
        // Compute the midpoint while reducing the risk of overflow.
        const double c = a + (b - a) / 2.0;
        const double fc = f(c);

        if (!std::isfinite(fc)) {
            throw std::runtime_error("Non-finite function value.");
        }

        // Stop if the residual or half the interval width is sufficiently small.
        if (std::abs(fc) <= tolerance ||
            (b - a) / 2.0 <= tolerance) {
            return {c, i, true};
        }

        // Keep the half-interval where the function changes sign.
        if (std::signbit(fa) != std::signbit(fc)) {
            b = c;
            fb = fc;
        } else {
            a = c;
            fa = fc;
        }
    }

    // Return the best midpoint approximation if the iteration limit is reached.
    return {a + (b - a) / 2.0, max_iterations, false};
}

int main() {
    // Define the equation f(x) = x^3 - x - 2.
    const auto f = [](double x) {
        return x * x * x - x - 2.0;
    };

    try {
        // Search for a root in the interval [1, 2].
        const BisectionResult result = bisection(f, 1.0, 2.0);

        // Display the numerical results with 12 significant digits.
        std::cout << std::setprecision(12)
                  << "Root: " << result.root << '\n'
                  << "f(root): " << f(result.root) << '\n'
                  << "Iterations: " << result.iterations << '\n'
                  << "Converged: " << std::boolalpha
                  << result.converged << '\n';
    } catch (const std::exception& error) {
        // Report invalid input or numerical evaluation errors.
        std::cerr << "Error: " << error.what() << '\n';
        return 1;
    }

    return 0;
}
