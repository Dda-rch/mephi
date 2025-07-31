#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <cstring>
#include <iomanip>
#include <type_traits>

using namespace std;

template <typename T>
T* multiplyPolynomials(const T* poly1, int degree1, const T* poly2, int degree2) {
    int resultDegree = degree1 + degree2;
    T* result = new T[resultDegree + 1]();

    for (int i = 0; i <= degree1; ++i) {
        for (int j = 0; j <= degree2; ++j) {
            result[i + j] += poly1[i] * poly2[j];
        }
    }

    return result;
}

char* multiplyPolynomials(char* const* poly1, int degree1, char* const* poly2, int degree2) {
    size_t totalLength = 0;
    for (int i = 0; i <= degree1; ++i) totalLength += strlen(poly1[i]);
    for (int i = 0; i <= degree2; ++i) totalLength += strlen(poly2[i]);

    char* result = new char[totalLength + 1];
    result[0] = '\0';

    for (int i = 0; i <= degree1; ++i) strcat(result, poly1[i]);
    for (int i = 0; i <= degree2; ++i) strcat(result, poly2[i]);

    return result;
}

template <typename T>
void printPolynomial(const T* poly, int degree) {
    for (int i = degree; i >= 0; --i) {
        if constexpr (is_floating_point_v<T>) {
            cout << fixed << setprecision(6) << poly[i];
        }
        else {
            cout << poly[i];
        }
        if (i != 0) cout << "x^" << i << " + ";
    }
    cout << endl;
}

int main() {
  
    float poly1_float[] = { 1.5f, 2.0f, 3.5f };
    float poly2_float[] = { 2.0f, 1.0f };
    float* result_float = multiplyPolynomials(poly1_float, 2, poly2_float, 1);
    cout << "Float result:\n";
    printPolynomial(result_float, 3);
    delete[] result_float;

    long poly1_long[] = { 4, 3, 2 };
    long poly2_long[] = { 1, 5 };
    long* result_long = multiplyPolynomials(poly1_long, 2, poly2_long, 1);
    cout << "\nLong result:\n";
    printPolynomial(result_long, 3);
    delete[] result_long;

    char str1[] = "Hello, ";
    char str2[] = "world! ";
    char str3[] = "This ";
    char str4[] = "is ";
    char str5[] = "a test.";

    char* poly1_str[] = { str1, str2 };
    char* poly2_str[] = { str3, str4, str5 };
    char* result_str = multiplyPolynomials(poly1_str, 1, poly2_str, 2);
    cout << "\nString result:\n" << result_str << endl;
    delete[] result_str;

    return 0;
}