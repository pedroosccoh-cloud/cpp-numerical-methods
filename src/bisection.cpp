
#include <exception>
#include <iomanip>
#include <iostream>

#include "../include/bisection.hpp"

/**
 * @brief Demonstrates the bisection algorithm on f(x) = x^3 - x - 2.
 *
 * The interval [1, 2] brackets a root because the function has
 * opposite signs at its endpoints.
 *
 * The program displays the root approximation, residual,
 * iteration count, and convergence status.
 *
 * @return 0 if execution completes successfully, 1 if an error occurs.
 */
int main() {
    // Define the function whose root will be approximated.
    const auto f = [](double x) {
        return x * x * x - x - 2.0;
    };

    try {
        // Apply the bisection method to the interval [1, 2].
        const BisectionResult result = bisection(f, 1.0, 2.0);

        // Display the numerical results with 12 significant digits.
        std::cout << std::setprecision(12)
                  << "Root: " << result.root << '\n'
                  << "f(root): " << f(result.root) << '\n'
                  << "Iterations: " << result.iterations << '\n'
                  << "Converged: " << std::boolalpha
                  << result.converged << '\n';

        // A result may be returned even if the iteration limit was reached.
        if (!result.converged) {
            std::cerr << "Warning: Maximum iterations reached.\n";
            return 1;
        }
    } catch (const std::exception& error) {
        // Report invalid inputs or numerical evaluation errors.
        std::cerr << "Error: " << error.what() << '\n';
        return 1;
    }

    return 0;
}
