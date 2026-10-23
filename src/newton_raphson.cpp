```cpp
#include <iomanip>
#include <iostream>
#include <exception>

#include "../include/newton_raphson.hpp"

/**
 * @brief Demonstrates Newton-Raphson on f(x) = x^3 - x - 2.
 *
 * @return 0 if execution completes successfully, 1 if an error occurs.
 */
int main() {
    // Define the function whose root will be approximated.
    const auto f = [](double x) {
        return x * x * x - x - 2.0;
    };

    try {
        // Apply Newton-Raphson with an initial guess of 1.5.
        const NewtonResult result = newton_raphson(f, 1.5);

        // Display the numerical results.
        std::cout << std::setprecision(12)
                  << "Root: " << result.root << '\n'
                  << "f(root): " << f(result.root) << '\n'
                  << "Iterations: " << result.iterations << '\n'
                  << "Converged: " << std::boolalpha
                  << result.converged << '\n';

        // Report if the iteration limit was reached.
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
```
