
#include <iostream>
#include <cmath>
#include <iomanip>

using namespace std;

// Funcion cuya raiz queremos encontrar
double f(double x) {
    return x * x - 2.0;
}

int main() {
    double a = 1.0;
    double b = 2.0;
    double tol = 1e-6;
    int max_iter = 100;

    if (f(a) * f(b) >= 0) {
        cout << "El intervalo no garantiza una raiz por biseccion.\n";
        return 1;
    }

    double c = a;

    for (int i = 1; i <= max_iter; i++) {
        c = (a + b) / 2.0;

        if (fabs(f(c)) < tol || (b - a) / 2.0 < tol) {
            cout << fixed << setprecision(8);
            cout << "Raiz aproximada: " << c <<endl;
            cout << "Iteraciones: " << i <<endl;
            cout << "f(raiz): " << f(c) <<endl;
            return 0;
        }

        if (f(a) * f(c) < 0) {
            b = c;
        } else {
            a = c;
        }
    }

    cout << "No se alcanzo la tolerancia en "
         << max_iter << " iteraciones."<<endl;

    return 0;
}
