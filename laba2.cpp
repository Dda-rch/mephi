#include <iostream>

void multiplyPolynom(float poly1[], float poly2[], float result[]) {
    for (int i = 0; i < 7; i++) {
        result[i] = 0;
    }

    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            result[i + j] += poly1[i] * poly2[j];
        }
    }
}

void multiplyPolynom(long poly1[], long poly2[], long result[]) {
    for (int i = 0; i < 7; i++) {
        result[i] = 0;
    }

    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            result[i + j] += poly1[i] * poly2[j];
        }
    }
}

void printPolynom(float poly[], int degree) {
    for (int i = degree; i >= 0; i--) {
        if (poly[i] != 0) {
            printf("%f", poly[i]);
            if (i > 0) {
                printf("x^%d + ", i);
            }
        }
    }
    printf("\n");
}

void printPolynom(long poly[], int degree) {
    for (int i = degree; i >= 0; i--) {
        if (poly[i] != 0) {
            printf("%ld", poly[i]);
            if (i > 0) {
                printf("x^%d + ", i);
            }
        }
    }
    printf("\n");
}

int main() {
    float poly1_float[4] = { 4, 3, 2, 1 };

    float poly2_float[4] = { 8, 7, 6, 5 };

    float result_float[7];

    multiplyPolynom(poly1_float, poly2_float, result_float);

    printf("Resulting polynomial (float): ");
    printPolynom(result_float, 6);

    long poly1_long[4] = { 4, 3, 2, 1 };

    long poly2_long[4] = { 8, 7, 6, 5 };

    long result_long[7];
    multiplyPolynom(poly1_long, poly2_long, result_long);

    printf("Resulting polynomial (long): ");
    printPolynom(result_long, 6);

    return 0;
}
