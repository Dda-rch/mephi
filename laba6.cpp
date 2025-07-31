#include <iostream>
#include <fstream>
#include <iomanip>
#include <stdexcept>
#include <climits>
#include <cmath>

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

bool checkFloatCoeff(float coeff) {
    if (!std::isfinite(coeff)) {
        std::cerr << "ERROR: Float coefficient overflow/underflow (" << coeff << ")" << std::endl;
        return false;
    }
    return true;
}

class FloatPolynomial {
private:
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

    FloatPolynomial(const FloatPolynomial& other) : size(other.size) {
        coefficients = new float[size];
        for (size_t i = 0; i < size; ++i) {
            coefficients[i] = other.coefficients[i];
        }
    }

    FloatPolynomial& operator=(const FloatPolynomial& other) {
        if (this != &other) {
            delete[] coefficients;
            size = other.size;
            coefficients = new float[size];
            for (size_t i = 0; i < size; ++i) {
                coefficients[i] = other.coefficients[i];
            }
        }
        return *this;
    }

    void load(const std::string& filename) {
        std::ifstream file(filename);
        if (!file.is_open()) {
            std::cerr << "Failed to open file: " << filename << std::endl;
            return;
        }

        float* temp = new float[100];  
        size_t count = 0;
        float coeff;

        while (file >> coeff && count < 100) {
            temp[count++] = coeff;
        }

        delete[] coefficients;
        size = count;
        coefficients = new float[size];
        for (size_t i = 0; i < size; ++i) {
            coefficients[i] = temp[i];
            if (!checkFloatCoeff(coefficients[i])) {
                coefficients[i] = 0.0f;
            }
        }

        delete[] temp;
        normalize();
    }

    void extend(unsigned int newSize) {
        if (newSize > size) {
            float* newCoeffs = new float[newSize];
            for (size_t i = 0; i < size; ++i) {
                newCoeffs[i] = coefficients[i];
            }
            for (size_t i = size; i < newSize; ++i) {
                newCoeffs[i] = 0.0f;
            }
            delete[] coefficients;
            coefficients = newCoeffs;
            size = newSize;
        }
    }

    void normalize() {
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

    FloatPolynomial multiply(const FloatPolynomial& other) const {
        FloatPolynomial result;
        size_t newSize = size + other.size - 1;
        float* newCoeffs = new float[newSize]();

        for (size_t i = 0; i < size; ++i) {
            for (size_t j = 0; j < other.size; ++j) {
                float product = coefficients[i] * other.coefficients[j];
                if (!checkFloatCoeff(product)) {
                    std::cerr << "Float multiplication overflow detected!" << std::endl;
                    delete[] newCoeffs;
                    return FloatPolynomial(); 
                }

                if (!checkFloatCoeff(newCoeffs[i + j] + product)) {
                    std::cerr << "Float addition overflow detected!" << std::endl;
                    delete[] newCoeffs;
                    return FloatPolynomial();
                }
                newCoeffs[i + j] += product;
            }
        }

        delete[] result.coefficients;
        result.size = newSize;
        result.coefficients = newCoeffs;
        result.normalize();
        return result;
    }

    void print() const {
        for (size_t i = 0; i < size; ++i) {
            std::cout << std::fixed << std::setprecision(6) << coefficients[i];
            if (i != 0) std::cout << "x^" << i;
            if (i != size - 1) std::cout << " + ";
        }
        std::cout << std::endl;
    }

    void save(const std::string& filename) const {
        std::ofstream file(filename);
        if (!file.is_open()) {
            std::cerr << "Failed to open file for writing: " << filename << std::endl;
            return;
        }

        for (size_t i = 0; i < size; ++i) {
            file << coefficients[i] << " ";
        }
        file << std::endl;
    }

    size_t getDegree() const {
        return size - 1;
    }

    ~FloatPolynomial() {
        delete[] coefficients;
    }
};

class LongPolynomial {
private:
    long* coefficients;  
    size_t size;        

public:
    LongPolynomial() : coefficients(new long[1] {0}), size(1) {}

    LongPolynomial(const long* coeffs, size_t coeffs_size) : size(coeffs_size) {
        coefficients = new long[size];
        for (size_t i = 0; i < size; ++i) {
            coefficients[i] = coeffs[i];
        }
        normalize();
    }

    LongPolynomial(const LongPolynomial& other) : size(other.size) {
        coefficients = new long[size];
        for (size_t i = 0; i < size; ++i) {
            coefficients[i] = other.coefficients[i];
        }
    }

    LongPolynomial& operator=(const LongPolynomial& other) {
        if (this != &other) {
            delete[] coefficients;
            size = other.size;
            coefficients = new long[size];
            for (size_t i = 0; i < size; ++i) {
                coefficients[i] = other.coefficients[i];
            }
        }
        return *this;
    }

    void load(const std::string& filename) {
        std::ifstream file(filename);
        if (!file.is_open()) {
            std::cerr << "Failed to open file: " << filename << std::endl;
            return;
        }

        long* temp = new long[100];  
        size_t count = 0;
        long coeff;

        while (file >> coeff && count < 100) {
            temp[count++] = coeff;
        }

        delete[] coefficients;
        size = count;
        coefficients = new long[size];
        for (size_t i = 0; i < size; ++i) {
            coefficients[i] = temp[i];
        }

        delete[] temp;
        normalize();
    }

    void extend(unsigned int newSize) {
        if (newSize > size) {
            long* newCoeffs = new long[newSize];
            for (size_t i = 0; i < size; ++i) {
                newCoeffs[i] = coefficients[i];
            }
            for (size_t i = size; i < newSize; ++i) {
                newCoeffs[i] = 0;
            }
            delete[] coefficients;
            coefficients = newCoeffs;
            size = newSize;
        }
    }

    void normalize() {
        size_t newSize = size;
        while (newSize > 1 && coefficients[newSize - 1] == 0) {
            newSize--;
        }
        if (newSize != size) {
            long* newCoeffs = new long[newSize];
            for (size_t i = 0; i < newSize; ++i) {
                newCoeffs[i] = coefficients[i];
            }
            delete[] coefficients;
            coefficients = newCoeffs;
            size = newSize;
        }
    }

    LongPolynomial multiply(const LongPolynomial& other) const {
        LongPolynomial result;
        size_t newSize = size + other.size - 1;
        long* newCoeffs = new long[newSize]();

        for (size_t i = 0; i < size; ++i) {
            for (size_t j = 0; j < other.size; ++j) {
                if (willLongMultiplyOverflow(coefficients[i], other.coefficients[j])) {
                    std::cerr << "Long multiplication overflow detected!" << std::endl;
                    delete[] newCoeffs;
                    return LongPolynomial(); 
                }
                long product = coefficients[i] * other.coefficients[j];

                if (willLongAddOverflow(newCoeffs[i + j], product)) {
                    std::cerr << "Long addition overflow detected!" << std::endl;
                    delete[] newCoeffs;
                    return LongPolynomial(); 
                }
                newCoeffs[i + j] += product;
            }
        }

        delete[] result.coefficients;
        result.size = newSize;
        result.coefficients = newCoeffs;
        result.normalize();
        return result;
    }

    void print() const {
        for (size_t i = 0; i < size; ++i) {
            std::cout << coefficients[i];
            if (i != 0) std::cout << "x^" << i;
            if (i != size - 1) std::cout << " + ";
        }
        std::cout << std::endl;
    }

    void save(const std::string& filename) const {
        std::ofstream file(filename);
        if (!file.is_open()) {
            std::cerr << "Failed to open file for writing: " << filename << std::endl;
            return;
        }

        for (size_t i = 0; i < size; ++i) {
            file << coefficients[i] << " ";
        }
        file << std::endl;
    }

    size_t getDegree() const {
        return size - 1;
    }

    ~LongPolynomial() {
        delete[] coefficients;
    }
};

int main() {
    const int MAX_POLYNOMIALS = 10;
    size_t all_degrees[MAX_POLYNOMIALS];
    int degree_count = 0;

    std::cout << "=== FloatPolynomial Demonstration ===" << std::endl;

    float coeffs1[] = {45678976543453453453434534533434456451.0f, 2.0f};
    FloatPolynomial fp1(coeffs1, 2);
    std::cout << "First polynomial: ";
    fp1.print();
    fp1.save("fp1_coeffs.txt");  
    if (degree_count < MAX_POLYNOMIALS) {
        all_degrees[degree_count++] = fp1.getDegree();
    }
    
    for (size_t i = 0; i < 2; ++i) {
        if (!checkFloatCoeff(coeffs1[i])) {
            std::cerr << "Float overflow detected in coefficient " << i << std::endl;
        }
    }

    float coeffs2[] = {45656789765434534534534345345334344515.0f, 4.0f, 3.0f};
    FloatPolynomial fp2(coeffs2, 3);
    std::cout << "Second polynomial: ";
    fp2.print();
    fp2.save("fp2_coeffs.txt");  
    if (degree_count < MAX_POLYNOMIALS) {
        all_degrees[degree_count++] = fp2.getDegree();
    }

    for (size_t i = 0; i < 3; ++i) {
        if (!checkFloatCoeff(coeffs2[i])) {
            std::cerr << "Float overflow detected in coefficient " << i << std::endl;
        }
    }

    std::cout << "\nMultiplying polynomials..." << std::endl;
    FloatPolynomial fpResult = fp1.multiply(fp2);
    std::cout << "Result: ";
    fpResult.print();
    fpResult.save("fp_result_coeffs.txt"); 
    if (degree_count < MAX_POLYNOMIALS) {
        all_degrees[degree_count++] = fpResult.getDegree();
    }

    std::cout << "\nDemonstrating other methods:" << std::endl;

    std::cout << "Extending first polynomial to degree 4..." << std::endl;
    fp1.extend(5);
    std::cout << "After extend: ";
    fp1.print();
    fp1.normalize();
    std::cout << "After normalization: ";
    fp1.print();

    FloatPolynomial fp4;
    std::cout << "Loading polynomial from file..." << std::endl;
    fp4.load("fp1_coeffs.txt");
    std::cout << "Loaded polynomial: ";
    fp4.print();

    std::cout << "\n\n=== LongPolynomial Demonstration ===" << std::endl;

    long lcoeffs1[] = {1, 2};
    LongPolynomial lp1(lcoeffs1, 2);
    std::cout << "First polynomial: ";
    lp1.print();
    lp1.save("lp1_coeffs.txt");  
    if (degree_count < MAX_POLYNOMIALS) {
        all_degrees[degree_count++] = lp1.getDegree();
    }

    for (size_t i = 0; i < 2; ++i) {
        if (lcoeffs1[i] == LONG_MAX || lcoeffs1[i] == LONG_MIN) {
            std::cerr << "Potential long overflow in coefficient " << i << std::endl;
        }
    }

    long lcoeffs2[] = {5, 4, 3};
    LongPolynomial lp2(lcoeffs2, 3);
    std::cout << "Second polynomial: ";
    lp2.print();
    lp2.save("lp2_coeffs.txt"); 
    if (degree_count < MAX_POLYNOMIALS) {
        all_degrees[degree_count++] = lp2.getDegree();
    }

    for (size_t i = 0; i < 3; ++i) {
        if (lcoeffs2[i] == LONG_MAX || lcoeffs2[i] == LONG_MIN) {
            std::cerr << "Potential long overflow in coefficient " << i << std::endl;
        }
    }

    std::cout << "\nMultiplying polynomials..." << std::endl;
    LongPolynomial lpResult = lp1.multiply(lp2);
    std::cout << "Result: ";
    lpResult.print();
    lpResult.save("lp_result_coeffs.txt");  
    if (degree_count < MAX_POLYNOMIALS) {
        all_degrees[degree_count++] = lpResult.getDegree();
    }

    std::cout << "\nDemonstrating other methods:" << std::endl;

    std::cout << "Extending first polynomial to degree 4..." << std::endl;
    lp1.extend(5);
    std::cout << "After extend: ";
    lp1.print();
    lp1.normalize();
    std::cout << "After normalization: ";
    lp1.print();

    std::cout << "\nSaving polynomial to file..." << std::endl;
    lp2.save("lp_test.txt");

    LongPolynomial lp4;
    std::cout << "Loading polynomial from file..." << std::endl;
    lp4.load("lp_test.txt");
    std::cout << "Loaded polynomial: ";
    lp4.print();

    std::ofstream degrees_file("all_degrees.txt");
    if (degrees_file.is_open()) {
        for (int i = 0; i < degree_count; ++i) {
            degrees_file << all_degrees[i] << " ";
        }
        degrees_file.close();
        std::cout << "\nAll polynomial degrees saved to all_degrees.txt" << std::endl;
    } else {
        std::cerr << "Failed to save degrees to file" << std::endl;
    }

    return 0;
}