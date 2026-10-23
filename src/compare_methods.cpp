#include <cmath>
#include <exception>
#include <iomanip>
#include <iostream>

#include "../include/bisection.hpp"
#include "../include/newton_raphson.hpp"

/**
 * @brief Compares the bisection and Newton-Raphson methods.
 *
 * Both methods approximate a root of f(x) = x^3 - x - 2.
 *
 * @return 0 if both methods complete successfully, 1 otherwise.
 */
int main() {
    // Define the function to solve.
    const auto f = [](double x) {
        return x * x * x - x - 2.0;
    };

    try {
        // Run both methods using suitable initial conditions.
        const BisectionResult bisection_result =
            bisection(f, 1.0, 2.0);

        const NewtonResult newton_result =
            newton_raphson(f, 1.5);

        // Display the results in a comparison table.
        std::cout << std::scientific << std::setprecision(10);

        std::cout << "\nROOT-FINDING METHODS COMPARISON\n";
        std::cout << "Function: f(x) = x^3 - x - 2\n\n";

        std::cout << std::left
                  << std::setw(20) << "Method"
                  << std::setw(22) << "Root"
                  << std::setw(22) << "Absolute residual"
                  << std::setw(15) << "Iterations"
                  << "Converged\n";

        std::cout << std::string(90, '-') << '\n';

        std::cout << std::left
                  << std::setw(20) << "Bisection"
                  << std::setw(22) << bisection_result.root
                  << std::setw(22)
                  << std::abs(f(bisection_result.root))
                  << std::setw(15) << bisection_result.iterations
                  << std::boolalpha << bisection_result.converged
                  << '\n';

        std::cout << std::left
                  << std::setw(20) << "Newton-Raphson"
                  << std::setw(22) << newton_result.root
                  << std::setw(22)
                  << std::abs(f(newton_result.root))
                  << std::setw(15) << newton_result.iterations
                  << std::boolalpha << newton_result.converged
                  << '\n';

        // Report whether both methods converged.
        if (!bisection_result.converged ||
            !newton_result.converged) {
            std::cerr << "\nWarning: At least one method did not converge.\n";
            return 1;
        }

    } catch (const std::exception& error) {
        std::cerr << "Error: " << error.what() << '\n';
        return 1;
    }

    return 0;
}
