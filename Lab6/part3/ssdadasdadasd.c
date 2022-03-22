#include <stdio.h>
#include <math.h>
#include <float.h>
#include <limits.h>

typedef int fixed;

int Q_M = 0;
int Q_N = 0;
int F_ONE = 0;

void SET_Q_FORMAT(int M, int N){
    Q_M = M;
    Q_N = N;
    F_ONE = 1 << N;
}

int FLOAT_TO_FIXED(float f){
    return (fixed)(f * F_ONE);
}

float FIXED_TO_FLOAT(fixed f){
    return (float)(f)/F_ONE;
}

fixed FIXED_MULT(fixed op1, fixed op2) {	
    return (fixed)((op1 * op2) >> Q_N); 
}

fixed FIXED_DIV(fixed op1, fixed op2) {
    return (fixed)((op1 / op2) << Q_N); 
}

float values[20] = {
    0.00001, 0.258792, 0.6258, 1.0700010090, 1.4, 1.57079632679, 1.67, 1.98756, 2.13256789, 2.58887, 
    2.356, 3, 3.14159265359, 3.25, 3.7589, 4.000025, 4.71238898038, 5.25898, 6.0001458796, 6.28318530718
};

float error_per_format[32];

float compute_sine_float(int iterations, float input) {
    float one = -1.0; 
    float factorial = 1.0;  
    float x = 1.0; 
    float computed_sine = 0.0; 

    for(int i = 0; i <= iterations * 2; i++) {
        x *= input;  
        factorial *= i + 1; 
        if(i % 2 == 0) {
            one *= -1.0; 
        }

        if((i + 1) % 2 == 1) {
            computed_sine += (one * (x / factorial));
        }
    }

    float error = fabs(sinf(input) - computed_sine); 
    return computed_sine; 
}

float compute_sine_fixed(int iterations, fixed input, float float_input) {     
    fixed one_fixed_positive = (fixed)(1.0 * F_ONE); 
    fixed one_fixed = one_fixed_positive;
    fixed factorial = one_fixed_positive;
    fixed x = one_fixed_positive;
    fixed computed_sine = 0; 
    
    
    for(int i = 0; i < iterations ; i++) {
        x = FIXED_MULT(x, input);  

        if(i != 0) {
            fixed intermediate_factorial = FIXED_MULT(FLOAT_TO_FIXED((float) i + 1), FLOAT_TO_FIXED((float) i + 2));
            factorial = FIXED_MULT(factorial, intermediate_factorial);
        }

        if(factorial == 0) {
            factorial = one_fixed_positive; 
        }
        
        if(i % 2 == 0) {
            computed_sine += FIXED_MULT(x, FIXED_DIV(one_fixed_positive, factorial));
        } else {
            computed_sine -= FIXED_MULT(x, FIXED_DIV(one_fixed_positive, factorial));
        }
        printf("%f, %f, %f\n", FIXED_TO_FLOAT(x), FIXED_TO_FLOAT(factorial), FIXED_TO_FLOAT(computed_sine));
        
    }

    float error = fabs(sinf(float_input) - FIXED_TO_FLOAT(computed_sine));
    return error; 
} 

void test_formats(int iterations) {
    //Test each value with each possible format
    float min_error = FLT_MAX; 
    int min_m = 0;; 

    // for(int m = 1; m <= 31; m++) {
        SET_Q_FORMAT(24, 31-24);
        
        // float error = 0.0;  
        // for(int i = 0; i < 20; i++) {
            compute_sine_fixed(iterations, FLOAT_TO_FIXED(values[12]), values[12]);
        // }
        // error /= 20;
        // error_per_format[m] = error; 

        // if(error < min_error) {
        //     min_error = error; 
        //     min_m = m;
        // }
    // }
    
    //Print value that minimizes error
    // printf("\nITERATIONS: %d\n" ,iterations);
    // printf("FINAL ERROR: %.15f\n" ,min_error);
    // printf("CHOSEN FORMAT: %d, %d (m, n)\n" ,min_m, 31-min_m); 
}

int main() {
    test_formats(10);
    // test_formats(5);
    // test_formats(7);
    // test_formats(9);
    // test_formats(11);
    return 0; 
}