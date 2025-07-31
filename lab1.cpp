#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <float.h>

float divide(float dividend, float divisor) {
    if (divisor == 0) {
        printf("Error /0\n");
        return 100000;
    }
    return dividend / divisor;
}

void clearInputBuffer() {
    int c;
    while (( c = getchar()) != '\n');
}

int main() {
    float a1, b1, a2, b2, promB, resB, resF;
    int count = 1, i = 0;

block: {
    printf("Enter a1 and b1: ");
    int A = scanf("%f", &a1);
    int B = scanf("%f", &b1);

    if (A == 1 && B == 1 && a1 <= FLT_MAX && b1 <= FLT_MAX && a1 >= -FLT_MAX && b1 >= -FLT_MAX) {
        if (b1 != 0) {
            promB = a1 / b1;
            resB = promB;
            printf("%f\n", resB);
        }
        else {
            printf("Error /0\n");
        }
        i++;
    }
    else {
        printf("Error scanf %d\n", i + 1);
        clearInputBuffer();  
    }
    }

if (i <= count) {
    goto block;
}

printf("Enter a2 and b2: ");
int C = scanf("%f", &a2);
int D = scanf("%f", &b2);

if (C == 1 && D == 1 && a2 <= FLT_MAX && b2 <= FLT_MAX && a2 >= -FLT_MAX && b2 >= -FLT_MAX) {
    resF = divide(a2, b2);
    printf("%f\n", resF);
}
else {
    printf("Error scanf 3\n");
    clearInputBuffer();
}

return 0;
}