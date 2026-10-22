#include <cmath>
#include <iomanip>
#include <iostream>
#include <stdexcept>

struct BisectionResult {
    double root;
    int iterations;
    bool converged;
};

template <typename Function>
BisectionResult bisection(
    Function f,
    double a,
    double b,
    double tolerance = 1e-8,
    int max_iterations = 100
) {
    if (!std::isfinite(a) || !std::isfinite(b) || a >= b) {
        throw std::invalid_argument("Invalid interval.");
    }

    if (!std::isfinite(tolerance) || tolerance <= 0.0 ||
        max_iterations <= 0) {
        throw std::invalid_argument("Invalid stopping criteria.");
    }

    double fa = f(a);
    double fb = f(b);

    if (!std::isfinite(fa) || !std::isfinite(fb)) {
        throw std::invalid_argument("Non-finite endpoint value.");
    }

    if (fa == 0.0) return {a, 0, true};
    if (fb == 0.0) return {b, 0, true};

    if (std::signbit(fa) == std::signbit(fb)) {
        throw std::invalid_argument(
            "Interval must bracket a root."
        );
    }

    for (int i = 1; i <= max_iterations; ++i) {
        const double c = a + (b - a) / 2.0;
        const double fc = f(c);

        if (!std::isfinite(fc)) {
            throw std::runtime_error("Non-finite function value.");
        }

        if (std::abs(fc) <= tolerance ||
            (b - a) / 2.0 <= tolerance) {
            return {c, i, true};
        }

        if (std::signbit(fa) != std::signbit(fc)) {
            b = c;
            fb = fc;
        } else {
            a = c;
            fa = fc;
        }
    }

    return {a + (b - a) / 2.0, max_iterations, false};
}

int main() {
    const auto f = [](double x) {
        return x * x * x - x - 2.0;
    };

    try {
        const BisectionResult result = bisection(f, 1.0, 2.0);

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

