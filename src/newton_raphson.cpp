
#include <algorithm>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <limits>
#include <stdexcept>

struct NewtonResult {
    double root;
    int iterations;
    bool converged;
};

// Estimates the first derivative using central finite differences.
template <typename Function>
double numerical_derivative(
    Function f,
    double x,
    double relative_step = 1e-5
) {
    if (!std::isfinite(x) ||
        !std::isfinite(relative_step) ||
        relative_step <= 0.0) {
        throw std::invalid_argument("Invalid derivative input.");
    }

    // Scale the step to the magnitude of x.
    const double h = relative_step * std::max(1.0, std::abs(x));

    const double f_plus = f(x + h);
    const double f_minus = f(x - h);

    if (!std::isfinite(f_plus) || !std::isfinite(f_minus)) {
        throw std::runtime_error(
            "Non-finite function value during differentiation."
        );
    }

    const double derivative = (f_plus - f_minus) / (2.0 * h);

    if (!std::isfinite(derivative)) {
        throw std::runtime_error("Non-finite derivative.");
    }

    return derivative;
}

/**
 * @brief Finds a root using Newton-Raphson with a numerical derivative.
 *
 * @param f Function whose root is being searched for.
 * @param initial_guess Initial approximation of the root.
 * @param tolerance Maximum accepted absolute function value.
 * @param max_iterations Maximum number of iterations.
 * @return Root approximation, iteration count, and convergence status.
 * @throws std::invalid_argument If the inputs are invalid.
 * @throws std::runtime_error If a numerical evaluation is non-finite
 *         or the derivative is too small.
 *
 * @note The derivative is estimated using central finite differences.
 */
template <typename Function>
NewtonResult newton_raphson(
    Function f,
    double initial_guess,
    double tolerance = 1e-8,
    int max_iterations = 100
) {
    if (!std::isfinite(initial_guess)) {
        throw std::invalid_argument("Invalid initial guess.");
    }

    if (!std::isfinite(tolerance) || tolerance <= 0.0 ||
        max_iterations <= 0) {
        throw std::invalid_argument("Invalid stopping criteria.");
    }

    double x = initial_guess;
    double fx = f(x);

    if (!std::isfinite(fx)) {
        throw std::runtime_error("Non-finite initial function value.");
    }

    if (std::abs(fx) <= tolerance) {
        return {x, 0, true};
    }

    for (int i = 1; i <= max_iterations; ++i) {
        const double dfx = numerical_derivative(f, x);

        // Avoid an unstable division by a nearly zero derivative.
        const double derivative_threshold =
            std::numeric_limits<double>::epsilon() *
            std::max(1.0, std::abs(fx)) /
            (1e-5 * std::max(1.0, std::abs(x)));

        if (std::abs(dfx) <= derivative_threshold) {
            throw std::runtime_error(
                "Derivative is zero or too small."
            );
        }

        // Apply the Newton-Raphson update.
        const double next_x = x - fx / dfx;

        if (!std::isfinite(next_x)) {
            throw std::runtime_error("Non-finite iteration value.");
        }

        const double next_fx = f(next_x);

        if (!std::isfinite(next_fx)) {
            throw std::runtime_error("Non-finite function value.");
        }

        if (std::abs(next_fx) <= tolerance) {
            return {next_x, i, true};
        }

        x = next_x;
        fx = next_fx;
    }

    return {x, max_iterations, false};
}

int main() {
    // Solve f(x) = x^3 - x - 2.
    const auto f = [](double x) {
        return x * x * x - x - 2.0;
    };

    try {
        const NewtonResult result = newton_raphson(f, 1.5);

        std::cout << std::setprecision(12)
                  << "Root: " << result.root << '\n'
                  << "f(root): " << f(result.root) << '\n'
                  << "Iterations: " << result.iterations << '\n'
                  << "Converged: " << std::boolalpha
                  << result.converged << '\n';
    } catch (const std::exception& error) {
        std::cerr << "Error: " << error.what() << '\n';
        return 1;
    }

    return 0;
}
