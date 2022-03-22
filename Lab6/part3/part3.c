#include <stdio.h>
#include <math.h>
#include <float.h>
#include <string.h>



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

fixed FIXED_MULT(fixed op1, fixed op2){
		
	// calculate the result of this multiplication
	// and return it here.

	return (op1 * op2) >> Q_N;

}

fixed FIXED_DIV(fixed op1, fixed op2){
    return (fixed)((op1 / op2) << Q_N); 
}

// float FLOAT_FACT(float n){
//     if(n==0){
//         return 1;
//     }
//     else{
//         return(n * FLOAT_FACT(n-1));
//     }
// }
float SINE_FL(float x, int order){
    float result = 0.0;
    float x_pow = x; 
    float fact = 1.0;
    for(int i = 0; i <= order; i++){
        // printf("%f\n", FLOAT_FACT(2*i+1));
        // if(i!=0) x_pow = x_pow * x * x;
        // printf("%f\n", x_po  w);
        if(i!=0) fact *= (2*i)*(2*i+1);
        result += (pow(-1.0, i)) * (pow(x, 2*i+1)/fact);
    }
    return result;

}

float SINE_FIXED(fixed x, int order){
    fixed one = FLOAT_TO_FIXED(1.0); 
    fixed fact = one;
    fixed x_pow = x;
    fixed x_squared = FIXED_MULT(x, x); 
    fixed fact_term = one;
    
    float result = FIXED_TO_FLOAT(FIXED_DIV(x_pow, fact));
    printf("First: %f => %f, %f\n", FIXED_TO_FLOAT(result), FIXED_TO_FLOAT(x_pow), FIXED_TO_FLOAT(fact));

    for(int i = 1; i <= order; i++){
        // x_pow = FIXED_MULT( x_pow, FIXED_MULT(x, x));
        // // x_pow = FIXED_MULT(x_pow, x); 
        // fixed temp = FIXED_MULT(FLOAT_TO_FIXED((float) i + 1), FLOAT_TO_FIXED((float) i + 2));
        // fact = FIXED_MULT(fact, temp);
        // if(!fact) fact = one;

        x_pow = FIXED_MULT( x_pow, FIXED_MULT(x, x));
        fixed temp = FIXED_MULT(fact_term+one, fact_term+one+one);
        fact = FIXED_MULT(fact, temp);
        if(!fact) fact = one;
        fact_term = fact_term+one+one;
        // fact_term = fact_term+one+one;
        // printf("%f, %f\n", FIXED_TO_FLOAT(x_pow), FIXED_TO_FLOAT(fact));
        // printf("%f\n", FIXED_TO_FLOAT(fact));
        if( i % 2 == 0){
            printf("pos %d, ", i);
            result = result + FIXED_TO_FLOAT(FIXED_DIV(x_pow, fact));
            printf("Result: %f => x: %f, fact: %f\n", FIXED_TO_FLOAT(result), FIXED_TO_FLOAT(x_pow), FIXED_TO_FLOAT(fact));
            continue;
            
        }else{
            printf("neg %d, ", i);
            result = result - FIXED_TO_FLOAT(FIXED_DIV(x_pow, fact));
            printf("Result: %f => x: %f, fact: %f\n", FIXED_TO_FLOAT(result), FIXED_TO_FLOAT(x_pow), FIXED_TO_FLOAT(fact));
            continue;
        }
        // printf("Res: %f (div: %d, %f)\n", FIXED_TO_FLOAT(result), x_pow / fact, FIXED_TO_FLOAT(x_pow / fact));
        // printf("Result: %f => %f, %f\n", FIXED_TO_FLOAT(result), FIXED_TO_FLOAT(x_pow), FIXED_TO_FLOAT(fact));
    }
    return result;

}
struct Q_format{
	int m;
	float error;
};
typedef struct Q_format Q_format;

float dataset[10] = {0.0, M_PI, M_PI_2, M_PI/4, M_PI/6, 2*M_PI, -M_PI, -0.99999, -1, 1};

int main(){
    
    float error = 0;
    float temp_error = 0.0;
    float benchmark = 0.0;
    float computed = 0.0;
    SET_Q_FORMAT(24, 31-24);
    for(int i = 0; i <= 31; i++){
        SET_Q_FORMAT(i, 31-i);
        printf("%d, %d\n", i, 31-i);
        float x = 3.14159265359;
        benchmark = sinf(x);
        // computed = SINE_FL(x,10);
        computed = FIXED_TO_FLOAT(SINE_FIXED(FLOAT_TO_FIXED(x), 5));
        temp_error = benchmark - computed;
        error += abs(temp_error);
        printf("sin(%f): %f (math.h), %f (float imp.)\n", x, benchmark, computed);
    }
    // error/=10;
    printf("Error: %f\n", error);
	return 0;
} 
