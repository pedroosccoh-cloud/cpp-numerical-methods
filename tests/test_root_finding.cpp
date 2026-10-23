```cpp
#include <cassert>
#include <cmath>
#include <iostream>
#include <stdexcept>

#include "../include/bisection.hpp"
#include "../include/newton_raphson.hpp"

// Check whether two values are approximately equal.
bool approximately_equal(
    double actual,
    double expected,
    double tolerance = 1e-6
) {
    return std::abs(actual - expected) <= tolerance;
}

int main() {
    int passed = 0;

    // Test 1: Bisection finds the root of x^2 - 4 in [1, 3].
    {
        const auto f = [](double x) {
            return x * x - 4.0;
        };

        const BisectionResult result = bisection(f, 1.0, 3.0);

        assert(result.converged);
        assert(approximately_equal(result.root, 2.0));
        ++passed;
        std::cout << "Test 1 passed: Bisection finds a root.\n";
    }

    // Test 2: Newton-Raphson finds the root of x^2 - 4.
    {
        const auto f = [](double x) {
            return x * x - 4.0;
        };

        const NewtonResult result = newton_raphson(f, 1.5);

        assert(result.converged);
        assert(approximately_equal(result.root, 2.0));
        ++passed;
        std::cout << "Test 2 passed: Newton-Raphson finds a root.\n";
    }

    // Test 3: Bisection rejects an interval without a sign change.
    {
        const auto f = [](double x) {
            return x * x + 1.0;
        };

        bool exception_caught = false;

        try {
            (void)bisection(f, -1.0, 1.0);
        } catch (const std::invalid_argument&) {
            exception_caught = true;
        }

        assert(exception_caught);
        ++passed;
        std::cout << "Test 3 passed: Bisection rejects an invalid interval.\n";
    }

    // Test 4: Newton-Raphson recognizes an initial guess that is a root.
    {
        const auto f = [](double x) {
            return x * x - 4.0;
        };

        const NewtonResult result = newton_raphson(f, 2.0);

        assert(result.converged);
        assert(result.iterations == 0);
        assert(approximately_equal(result.root, 2.0));
        ++passed;
        std::cout << "Test 4 passed: Initial guess is already a root.\n";
    }

    // Test 5: Bisection rejects an invalid interval ordering.
    {
        const auto f = [](double x) {
            return x * x - 4.0;
        };

        bool exception_caught = false;

        try {
            (void)bisection(f, 3.0, 1.0);
        } catch (const std::invalid_argument&) {
            exception_caught = true;
        }

        assert(exception_caught);
        ++passed;
        std::cout << "Test 5 passed: Bisection rejects reversed endpoints.\n";
    }

    std::cout << "\nAll " << passed << " tests passed.\n";

    return 0;
}
```
