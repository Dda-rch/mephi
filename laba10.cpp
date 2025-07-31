#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include <iomanip>
#include <cctype>
#include <sstream>

using namespace std;

struct Delimiters {
    const string chars;
    Delimiters(const string& s) : chars(s) {}
};

istream& operator>>(istream& is, const Delimiters& delims) {
    char c;
    while (is.get(c)) {
        if (delims.chars.find(c) == string::npos && !isspace(c)) {
            is.putback(c);
            break;
        }
    }
    return is;
}

class NumberLine {
private:
    string line;
    long double sum;
    int maxDecimalPlaces;
    int numbersCount;

    int countDecimalPlaces(const string& numStr) const {
        size_t dotPos = numStr.find('.');
        if (dotPos != string::npos) {
            size_t lastPos = numStr.find_last_not_of('0');
            if (lastPos == string::npos || lastPos < dotPos) return 0;
            return lastPos - dotPos;
        }
        return 0;
    }

    void processLine() {
        istringstream iss(line);
        iss >> Delimiters(" ,;");

        string numberStr;
        char c;

        while (iss.get(c)) {
            if (c == '-' && (isdigit(iss.peek()) || iss.peek() == '.')) {
                numberStr = "-";
                while (iss.get(c) && (isdigit(c) || c == '.')) {
                    numberStr += c;
                }

                if (!numberStr.empty()) {
                    *this << numberStr;

                    char* end;
                    long double num = strtold(numberStr.c_str(), &end);
                    sum += fabsl(num);

                    int places = countDecimalPlaces(numberStr);
                    if (places > maxDecimalPlaces) {
                        maxDecimalPlaces = places;
                    }
                    numbersCount++;
                }

                if (!iss.eof()) iss.putback(c);
            }
        }
    }

public:
    NumberLine(const string& s) : line(s), sum(0.0L), maxDecimalPlaces(0), numbersCount(0) {
        processLine();
    }

    NumberLine& operator<<(const string& num) {
        cout << num << endl;
        return *this;
    }

    long double getSum() const { return sum; }
    int getMaxDecimalPlaces() const { return maxDecimalPlaces; }
    int getNumbersCount() const { return numbersCount; }
};

int main() {
    ifstream file("task4.txt");
    if (!file) {
        cerr << "ERROR: Cannot open file." << endl;
        return 1;
    }

    long double totalSum = 0.0L;
    int globalMaxPlaces = 0;

    cout << "Found negative numbers:" << endl;

    string line;
    while (getline(file, line)) {
        NumberLine nl(line);
        totalSum += nl.getSum();

        int places = nl.getMaxDecimalPlaces();
        if (places > globalMaxPlaces) {
            globalMaxPlaces = places;
        }
    }

    file.close();

    cout << fixed << setprecision(globalMaxPlaces);
    cout << "\nTotal sum: " << totalSum << endl;
    cout << "Expected:  146461.228981481493974" << endl;

    return 0;
}