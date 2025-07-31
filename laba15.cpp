#include <iostream>
#include <iomanip>
#include <limits>
#include <stdexcept>

using namespace std;

template <typename T>
class Polynomial {
private:
    T* coefficients;
    int degree;

    bool isMultiplicationSafe(T a, T b) const {
        if (a == 0 || b == 0) return true;

        if constexpr (is_floating_point_v<T>) {
            T max_val = numeric_limits<T>::max();
            return (abs(a) <= max_val / abs(b));
        }
        else {
            return (b > 0) ? (a <= numeric_limits<T>::max() / b)
                : (a >= numeric_limits<T>::min() / b);
        }
    }

    bool isAdditionSafe(T a, T b) const {
        if constexpr (is_floating_point_v<T>) {
            T max_val = numeric_limits<T>::max();
            return abs(a) <= max_val - abs(b);
        }
        else {
            if (b > 0) {
                return a <= numeric_limits<T>::max() - b;
            }
            else {
                return a >= numeric_limits<T>::min() - b;
            }
        }
    }

public:
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
                if (!isMultiplicationSafe(coefficients[i], other.coefficients[j])) {
                    cerr << "Error: Multiplication overflow in polynomial coefficients" << endl;
                    exit(1);
                }
                T product = coefficients[i] * other.coefficients[j];

                if (!isAdditionSafe(result.coefficients[i + j], product)) {
                    cerr << "Error: Addition overflow in polynomial coefficients" << endl;
                    exit(1);
                }

                result.coefficients[i + j] += product;
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

    T& operator[](int index) {
        return coefficients[index];
    }

    const T& operator[](int index) const {
        return coefficients[index];
    }
};

int main() {
    Polynomial<float> poly1(2);
    poly1[0] = 13456543546576345556535565356453346538.0f;
    poly1[1] = 3.2f;
    poly1[2] = 2.7f;

    Polynomial<float> poly2(1);
    poly2[0] = 4565435465763455565355656453346554531.0f;
    poly2[1] = 4.3f;

    cout << "Polynomial 1 (float): ";
    poly1.print();
    cout << "Polynomial 2 (float): ";
    poly2.print();

    Polynomial<float> resultFloat = poly1.multiply(poly2);
    cout << "Result (float): ";
    resultFloat.print();

    Polynomial<long> poly3(2);
    poly3[0] = 3;
    poly3[1] = 2;
    poly3[2] = 5;

    Polynomial<long> poly4(1);
    poly4[0] = 1;
    poly4[1] = 7;

    cout << "\nPolynomial 3 (long): ";
    poly3.print();
    cout << "Polynomial 4 (long): ";
    poly4.print();

    Polynomial<long> resultLong = poly3.multiply(poly4);
    cout << "Result (long): ";
    resultLong.print();

    return 0;
}