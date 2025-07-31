#include <iostream>
#include <stdexcept>
#include <algorithm>
#include <cmath>
#include <fstream>

class FloatPolynomial {
private:
    float* coefficients;
    size_t size;

    void normalize() {
        while (size > 1 && std::fabs(coefficients[size - 1]) < 1e-9f) {
            size--;
        }
    }

    void resize(size_t newSize) {
        float* newCoeffs = new float[newSize]();
        size_t copySize = std::min(size, newSize);
        for (size_t i = 0; i < copySize; i++) {
            newCoeffs[i] = coefficients[i];
        }
        delete[] coefficients;
        coefficients = newCoeffs;
        size = newSize;
    }

public:
    FloatPolynomial() : coefficients(new float[1] {0.0f}), size(1) {}

    FloatPolynomial(const float* coeffs, size_t coeffs_size) : size(coeffs_size) {
        coefficients = new float[size];
        for (size_t i = 0; i < size; i++) {
            coefficients[i] = coeffs[i];
        }
        normalize();
    }

    FloatPolynomial(const FloatPolynomial& other) : size(other.size) {
        coefficients = new float[size];
        for (size_t i = 0; i < size; i++) {
            coefficients[i] = other.coefficients[i];
        }
    }

    ~FloatPolynomial() {
        delete[] coefficients;
    }

    FloatPolynomial& operator=(const FloatPolynomial& other) {
        if (this != &other) {
            delete[] coefficients;
            size = other.size;
            coefficients = new float[size];
            for (size_t i = 0; i < size; i++) {
                coefficients[i] = other.coefficients[i];
            }
        }
        return *this;
    }

    void divide(const FloatPolynomial& divisor, FloatPolynomial& quotient, FloatPolynomial& remainder) const {
        if (divisor.size == 0 || (divisor.size == 1 && std::fabs(divisor.coefficients[0]) < 1e-9f)) {
            throw std::invalid_argument("Division by zero polynomial");
        }

        remainder = *this;
        quotient = FloatPolynomial();

        if (degree() < divisor.degree()) {
            return;
        }

        size_t quotientSize = degree() - divisor.degree() + 1;
        float* quotientCoeffs = new float[quotientSize]();

        while (remainder.degree() >= divisor.degree()) {
            size_t currentPos = remainder.degree() - divisor.degree();
            float factor = remainder.coefficients[remainder.size - 1] / divisor.coefficients[divisor.size - 1];
            quotientCoeffs[currentPos] = factor;

            FloatPolynomial temp;
            temp.resize(remainder.size);

            for (size_t i = 0; i < divisor.size; i++) {
                temp.coefficients[i + currentPos] = divisor.coefficients[i] * factor;
            }

            remainder = remainder - temp;
        }

        delete[] quotient.coefficients;
        quotient.coefficients = quotientCoeffs;
        quotient.size = quotientSize;
        quotient.normalize();
        remainder.normalize();
    }


    void subtract(const FloatPolynomial& other, FloatPolynomial& result) const {
        size_t maxSize = std::max(size, other.size);
        result.resize(maxSize);

        for (size_t i = 0; i < maxSize; i++) {
            float a = (i < size) ? coefficients[i] : 0.0f;
            float b = (i < other.size) ? other.coefficients[i] : 0.0f;
            result.coefficients[i] = a - b;
        }

        result.normalize();
    }

    FloatPolynomial operator/(const FloatPolynomial& divisor) const {
        if (divisor.size == 0 || (divisor.size == 1 && std::fabs(divisor.coefficients[0]) < 1e-9f)) {
            throw std::invalid_argument("Division by zero polynomial");
        }

        FloatPolynomial remainder = *this;
        FloatPolynomial quotient;

        if (degree() < divisor.degree()) {
            return quotient;
        }

        size_t quotientSize = degree() - divisor.degree() + 1;
        float* quotientCoeffs = new float[quotientSize]();

        while (remainder.degree() >= divisor.degree()) {
            size_t currentPos = remainder.degree() - divisor.degree();
            float factor = remainder.coefficients[remainder.size - 1] / divisor.coefficients[divisor.size - 1];
            quotientCoeffs[currentPos] = factor;

            FloatPolynomial temp;
            temp.resize(remainder.size);

            for (size_t i = 0; i < divisor.size; i++) {
                temp.coefficients[i + currentPos] = divisor.coefficients[i] * factor;
            }

            remainder = remainder - temp;
        }

        delete[] quotient.coefficients;
        quotient.coefficients = quotientCoeffs;
        quotient.size = quotientSize;
        quotient.normalize();

        return quotient;
    }

    FloatPolynomial operator-(const FloatPolynomial& other) const {
        size_t maxSize = std::max(size, other.size);
        FloatPolynomial result;
        result.resize(maxSize);

        for (size_t i = 0; i < maxSize; i++) {
            float a = (i < size) ? coefficients[i] : 0.0f;
            float b = (i < other.size) ? other.coefficients[i] : 0.0f;
            result.coefficients[i] = a - b;
        }

        result.normalize();
        return result;
    }
    size_t degree() const {
        return size - 1;
    }

    void saveToFile(const std::string& filename) const {
        std::ofstream out(filename);
        if (!out) {
            throw std::runtime_error("Failed to open file for writing");
        }


        out << degree() << "\n";

        for (size_t i = 0; i < size; i++) {
            out << coefficients[i];
            if (i != size - 1) out << " ";
        }
    }

    void print() const {
        for (size_t i = 0; i < size; i++) {
            std::cout << coefficients[i];
            if (i != 0) std::cout << "x^" << i;
            if (i != size - 1) std::cout << " + ";
        }
        std::cout << std::endl;
    }
};

int main() {
    float coeffs1[] = { 1.0f, 3.0f, 2.0f }; 
    float coeffs2[] = { 5.0f, 1.0f };     

    FloatPolynomial p1(coeffs1, 3);
    FloatPolynomial p2(coeffs2, 2);

    std::cout << "Polynoms:\n";
    std::cout << "P1: ";
    p1.print();
    std::cout << "P2: ";
    p2.print();
    std::cout << "-----------------------\n";
  
    FloatPolynomial q, r;
    p1.divide(p2, q, r);
    std::cout << "Method divide():\n";
    std::cout << "Quotient : ";
    q.print();
    std::cout << "Remainder: ";
    r.print();
    std::cout << "-----------------------\n";

    FloatPolynomial divResult = p1 / p2;
    std::cout << "Operator (/):\n";
    divResult.print();
    std::cout << "-----------------------\n";

    FloatPolynomial subResultMethod;
    p1.subtract(p2, subResultMethod);
    std::cout << "Method subtract():\n";
    subResultMethod.print();
    std::cout << "-----------------------\n";

    FloatPolynomial subResultOp = p1 - p2;
    std::cout << "Operator (-):\n";
    subResultOp.print();
    std::cout << "-----------------------\n";

    p1.saveToFile("p1.txt");
    p2.saveToFile("p2.txt");
    q.saveToFile("quotient.txt");
    r.saveToFile("remainder.txt");
    divResult.saveToFile("division_result.txt");
    subResultMethod.saveToFile("subtract_method_result.txt");
    subResultOp.saveToFile("subtract_operator_result.txt");


    return 0;
}