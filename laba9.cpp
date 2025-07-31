#include <iostream>
#include <fstream>
#include <iomanip>
#include <stdexcept>
#include <climits>
#include <cmath>

bool checkFloatCoeff(float coeff) {
    if (!std::isfinite(coeff)) {
        std::cerr << "ERROR: Float coefficient overflow/underflow (" << coeff << ")" << std::endl;
        return false;
    }
    return true;
}

class FloatPolynomial {
protected:
    float* coefficients;
    size_t size;

public:
    FloatPolynomial() : coefficients(new float[1] {0.0f}), size(1) {}

    FloatPolynomial(const float* coeffs, size_t coeffs_size) : size(coeffs_size) {
        coefficients = new float[size];
        for (size_t i = 0; i < size; ++i) {
            coefficients[i] = coeffs[i];
            if (!checkFloatCoeff(coefficients[i])) {
                coefficients[i] = 0.0f;
            }
        }
        normalize();
    }

    virtual ~FloatPolynomial() {
        delete[] coefficients;
    }

    virtual void normalize() {
        size_t newSize = size;
        while (newSize > 1 && coefficients[newSize - 1] == 0.0f) {
            newSize--;
        }
        if (newSize != size) {
            float* newCoeffs = new float[newSize];
            for (size_t i = 0; i < newSize; ++i) {
                newCoeffs[i] = coefficients[i];
            }
            delete[] coefficients;
            coefficients = newCoeffs;
            size = newSize;
        }
    }

    size_t getDegree() const {
        return size - 1;
    }

    virtual void print() const {
        for (size_t i = 0; i < size; ++i) {
            std::cout << std::fixed << std::setprecision(2) << coefficients[i];
            if (i != 0) std::cout << "x^" << i;
            if (i != size - 1) std::cout << " + ";
        }
    }

    virtual void save(const std::string& filename) const {
        std::ofstream file(filename);
        if (!file.is_open()) {
            std::cerr << "Failed to open file for writing: " << filename << std::endl;
            return;
        }
        for (size_t i = 0; i < size; ++i) {
            file << coefficients[i] << " ";
        }
    }
};

class ComplexPolynomial : public FloatPolynomial {
private:
    float* imaginary_coeffs;

public:
    ComplexPolynomial() : FloatPolynomial(), imaginary_coeffs(new float[1] {0.0f}) {}

    ComplexPolynomial(const float* real_coeffs, const float* imag_coeffs, size_t coeffs_size)
        : FloatPolynomial(real_coeffs, coeffs_size) {
        imaginary_coeffs = new float[size];
        for (size_t i = 0; i < size; ++i) {
            imaginary_coeffs[i] = imag_coeffs[i];
            if (!checkFloatCoeff(imaginary_coeffs[i])) {
                imaginary_coeffs[i] = 0.0f;
            }
        }
        normalize();
    }

    ComplexPolynomial(const ComplexPolynomial& other) : FloatPolynomial(other) {
        imaginary_coeffs = new float[size];
        for (size_t i = 0; i < size; ++i) {
            imaginary_coeffs[i] = other.imaginary_coeffs[i];
        }
    }

    ComplexPolynomial& operator=(const ComplexPolynomial& other) {
        if (this != &other) {
            FloatPolynomial::operator=(other);
            delete[] imaginary_coeffs;
            imaginary_coeffs = new float[size];
            for (size_t i = 0; i < size; ++i) {
                imaginary_coeffs[i] = other.imaginary_coeffs[i];
            }
        }
        return *this;
    }

    ~ComplexPolynomial() override {
        delete[] imaginary_coeffs;
    }

    void normalize() override {
        size_t newSize = size;
        while (newSize > 1 &&
            coefficients[newSize - 1] == 0.0f &&
            imaginary_coeffs[newSize - 1] == 0.0f) {
            newSize--;
        }
        if (newSize != size) {
            float* newRealCoeffs = new float[newSize];
            float* newImagCoeffs = new float[newSize];
            for (size_t i = 0; i < newSize; ++i) {
                newRealCoeffs[i] = coefficients[i];
                newImagCoeffs[i] = imaginary_coeffs[i];
            }
            delete[] coefficients;
            delete[] imaginary_coeffs;
            coefficients = newRealCoeffs;
            imaginary_coeffs = newImagCoeffs;
            size = newSize;
        }
    }

    void print() const override {
        for (size_t i = 0; i < size; ++i) {
            std::cout << "(" << coefficients[i] << std::showpos << imaginary_coeffs[i]
                << "i" << std::noshowpos << ")";
                if (i != 0) std::cout << "x^" << i;
                if (i != size - 1) std::cout << " + ";
        }
        std::cout << std::endl;
    }

    void save(const std::string& filename) const override {
        std::ofstream file(filename);
        if (!file.is_open()) {
            std::cerr << "Failed to open file for writing: " << filename << std::endl;
            return;
        }
        for (size_t i = 0; i < size; ++i) {
            file << coefficients[i] << " " << imaginary_coeffs[i] << " ";
        }
    }

    void load(const std::string& filename) {
        std::ifstream file(filename);
        if (!file.is_open()) {
            std::cerr << "Failed to open file: " << filename << std::endl;
            return;
        }

        float* temp_real = nullptr;
        float* temp_imag = nullptr;
        size_t count = 0;
        float real, imag;

        while (file >> real >> imag) {
            if (temp_real == nullptr) {
                temp_real = new float[100];
                temp_imag = new float[100];
            }
            temp_real[count] = real;
            temp_imag[count] = imag;
            count++;
        }

        delete[] coefficients;
        delete[] imaginary_coeffs;

        size = count;
        coefficients = new float[size];
        imaginary_coeffs = new float[size];

        for (size_t i = 0; i < size; ++i) {
            coefficients[i] = temp_real[i];
            imaginary_coeffs[i] = temp_imag[i];
        }

        delete[] temp_real;
        delete[] temp_imag;

        normalize();
    }
};

int main() {
    std::cout << "=== Complex Polynomial===" << std::endl;

    float real1[] = { 14.0f, 3.0f };
    float imag1[] = { 2.0f, 4.0f };
    ComplexPolynomial cp1(real1, imag1, 2);

    float real2[] = { 5.0f, 7.0f, 9.0f };
    float imag2[] = { 6.0f, 8.0f, 10.0f };
    ComplexPolynomial cp2(real2, imag2, 3);

    std::cout << "Polynomial 1: ";
    cp1.print();
    std::cout << "Polynomial 2: ";
    cp2.print();

    cp1.save("cp1.txt");
    cp2.save("cp2.txt");

    ComplexPolynomial cp3;
    cp3.load("cp1.txt");
    std::cout << "Loaded polynomial: ";
    cp3.print();

    return 0;
}