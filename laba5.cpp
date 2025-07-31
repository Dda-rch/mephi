#include <iostream>
#include <cstring>
#include <cstdio>
#include <climits>
#include <cmath>
#include <cfenv>

class Polynom {
private:
    int degree;
    float* float_coeffs;
    long* long_coeffs;
    float* float_result;
    long* long_result;
    bool long_overflow; 
    bool float_overflow;

    bool checkLongCoeff(long coeff) {
        if (coeff == LONG_MIN || coeff == LONG_MAX) {
            printf("ERROR: Long coefficient overflow (%ld)\n", coeff);
            long_overflow = true;
            return false;
        }
        return true;
    }

    bool checkFloatCoeff(float coeff) {
        if (!std::isfinite(coeff)) {
            printf("ERROR: Float coefficient overflow/underflow (%f)\n", coeff);
            float_overflow = true;
            return false;
        }
        return true;
    }

    bool willLongMultiplyOverflow(long a, long b) {
        if (a > 0 && b > 0) {
            return a > LONG_MAX / b;
        }
        else if (a < 0 && b < 0) {
            return a < LONG_MAX / b;
        }
        else if (a > 0 && b < 0) {
            return b < LONG_MIN / a;
        }
        else if (a < 0 && b > 0) {
            return a < LONG_MIN / b;
        }
        return false;
    }

    bool willLongAddOverflow(long a, long b) {
        if (b > 0) {
            return a > LONG_MAX - b;
        }
        else {
            return a < LONG_MIN - b;
        }
    }

public:
    Polynom() {
        degree = 0;
        float_coeffs = new float[1];
        long_coeffs = new long[1];
        float_result = new float[1];
        long_result = new long[1];
        long_overflow = false;
        float_overflow = false;

        float_coeffs[0] = 0;
        long_coeffs[0] = 0;
        float_result[0] = 0;
        long_result[0] = 0;

        printf("DEFAULT POLY(degree %d)\n", degree);
    }

    Polynom(int deg) {
        degree = deg;
        float_coeffs = new float[degree + 1];
        long_coeffs = new long[degree + 1];
        float_result = new float[2 * degree + 1];
        long_result = new long[2 * degree + 1];
        long_overflow = false;
        float_overflow = false;

        for (int i = 0; i <= degree; i++) {
            float_coeffs[i] = 0;
            long_coeffs[i] = 0;
        }
        for (int i = 0; i <= 2 * degree; i++) {
            float_result[i] = 0;
            long_result[i] = 0;
        }

        printf("PARAMETR POLY(degree %d)\n", degree);
    }

    Polynom(const Polynom& other) {
        degree = other.degree;
        long_overflow = other.long_overflow;
        float_overflow = other.float_overflow;

        float_coeffs = new float[degree + 1];
        long_coeffs = new long[degree + 1];
        float_result = new float[2 * degree + 1];
        long_result = new long[2 * degree + 1];

        for (int i = 0; i <= degree; i++) {
            float_coeffs[i] = other.float_coeffs[i];
            long_coeffs[i] = other.long_coeffs[i];
        }
        for (int i = 0; i <= 2 * degree; i++) {
            float_result[i] = other.float_result[i];
            long_result[i] = other.long_result[i];
        }

        printf("COPY POLY(degree %d)\n", degree);
    }

    ~Polynom() {
        delete[] float_coeffs;
        delete[] long_coeffs;
        delete[] float_result;
        delete[] long_result;

        printf("Destruct polinom (degree %d)\n", degree);
    }

    void setFloatCoeffs(const float coeffs[]) {
        for (int i = 0; i <= degree && !float_overflow; i++) {
            float_coeffs[i] = coeffs[i];
            checkFloatCoeff(coeffs[i]);
        }
    }

    void setLongCoeffs(const long coeffs[]) {
        for (int i = 0; i <= degree && !long_overflow; i++) {
            long_coeffs[i] = coeffs[i];
            checkLongCoeff(coeffs[i]);
        }
    }

    void multiply() {
        for (int i = 0; i <= 2 * degree; i++) {
            float_result[i] = 0;
            long_result[i] = 0;
        }

        if (!float_overflow) {
            for (int i = 0; i <= degree; i++) {
                for (int j = 0; j <= degree; j++) {
                    float_result[i + j] += float_coeffs[i] * float_coeffs[j];
                    if (!checkFloatCoeff(float_result[i + j])) {
                        break; 
                    }
                }
                if (float_overflow) break;
            }
        }

        if (!long_overflow) {
            for (int i = 0; i <= degree; i++) {
                for (int j = 0; j <= degree; j++) {
                    if (willLongMultiplyOverflow(long_coeffs[i], long_coeffs[j])) {
                        long_overflow = true;
                        printf("ERROR: Long multiplication overflow at coefficients %d and %d (%ld * %ld)\n",
                            i, j, long_coeffs[i], long_coeffs[j]);
                        break;
                    }
                    long product = long_coeffs[i] * long_coeffs[j];

                    if (willLongAddOverflow(long_result[i + j], product)) {
                        long_overflow = true;
                        printf("ERROR: Long addition overflow at position %d (%ld + %ld)\n",
                            i + j, long_result[i + j], product);
                        break;
                    }

                    long_result[i + j] += product;
                }
                if (long_overflow) break;
            }
        }
    }

    void print(const char* title) const {
        printf("%s\n", title);

        printf("Float poly: ");
        if (float_overflow) {
            printf("Error max/min (input coefficients)");
        }
        else {
            for (int i = degree; i >= 0; i--) {
                if (float_coeffs[i] != 0) {
                    printf("%f", float_coeffs[i]);
                    if (i > 0) {
                        printf("x^%d + ", i);
                    }
                }
            }
        }
        printf("\n");

        printf("Long poly: ");
        if (long_overflow) {
            printf("Error max/min (input coefficients)");
        }
        else {
            for (int i = degree; i >= 0; i--) {
                if (long_coeffs[i] != 0) {
                    printf("%ld", long_coeffs[i]);
                    if (i > 0) {
                        printf("x^%d + ", i);
                    }
                }
            }
        }
        printf("\n");

        printf("Float result: ");
        if (float_overflow) {
            printf("Error max/min");
        }
        else {
            for (int i = 2 * degree; i >= 0; i--) {
                if (float_result[i] != 0) {
                    printf("%f", float_result[i]);
                    if (i > 0) {
                        printf("x^%d + ", i);
                    }
                }
            }
        }
        printf("\n");

        printf("Long result: ");
        if (long_overflow) {
            printf("Error max/min");
        }
        else {
            for (int i = 2 * degree; i >= 0; i--) {
                if (long_result[i] != 0) {
                    printf("%ld", long_result[i]);
                    if (i > 0) {
                        printf("x^%d + ", i);
                    }
                }
            }
        }
        printf("\n\n");
    }
};

int main() {
    Polynom p_default;
    p_default.print("Default polynomial:");

    Polynom p(3);

    float float_coeffs[4] = { 354545434554457887898989.0f, 2.0f, 3.0f, 4.0f}; 
    long long_coeffs[4] = {4, 3, 2, 1 };

    p.setFloatCoeffs(float_coeffs);
    p.setLongCoeffs(long_coeffs);
    p.multiply();
    p.print("Original polynomial:");

    Polynom p_copy(p);
    p_copy.print("Copied polynomial:");

    return 0;
}