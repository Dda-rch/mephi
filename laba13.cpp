#include <iostream>
#include <iomanip>

using namespace std;

template <typename T>
struct Polynomial {
    T* coefficients;
    int degree;

    Polynomial(int deg) : degree(deg) {
        coefficients = new T[degree + 1]();
    }

    ~Polynomial() {
        delete[] coefficients;
    }

    Polynomial<T> multiply(const Polynomial<T>& other) const {
        int resultDegree = degree + other.degree;
        Polynomial<T> result(resultDegree);

        for (int i = 0; i <= degree; ++i) {
            for (int j = 0; j <= other.degree; ++j) {
                result.coefficients[i + j] += coefficients[i] * other.coefficients[j];
            }
        }

        return result;
    }

    void print() const {
        for (int i = degree; i >= 0; --i) {
            if (coefficients[i] != 0) {
                if constexpr (is_floating_point_v<T>) {
                    cout << fixed << setprecision(6) << coefficients[i];
                }
                else {
                    cout << coefficients[i];
                }
                if (i > 0) cout << "x^" << i;
                if (i != 0) cout << " + ";
            }
        }
        cout << endl;
    }
};

int main() {
    Polynomial<float> poly1(2);
    poly1.coefficients[0] = 1.5f;
    poly1.coefficients[1] = 3.2f;
    poly1.coefficients[2] = 2.7f;

    Polynomial<float> poly2(1);
    poly2.coefficients[0] = 5.1f;
    poly2.coefficients[1] = 4.3f;

    cout << "Polynomial 1 (float): ";
    poly1.print();
    cout << "Polynomial 2 (float): ";
    poly2.print();

    Polynomial<float> resultFloat = poly1.multiply(poly2);
    cout << "Result (float): ";
    resultFloat.print();

    Polynomial<long> poly3(2);
    poly3.coefficients[0] = 3;
    poly3.coefficients[1] = 2;
    poly3.coefficients[2] = 5;

    Polynomial<long> poly4(1);
    poly4.coefficients[0] = 1;
    poly4.coefficients[1] = 7;

    cout << "\nPolynomial 3 (long): ";
    poly3.print();
    cout << "Polynomial 4 (long): ";
    poly4.print();

    Polynomial<long> resultLong = poly3.multiply(poly4);
    cout << "Result (long): ";
    resultLong.print();

    return 0;
}