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

float SINE_FL(float x, int order){
    float result = 0.0;
    float x_pow = x; 
    float fact = 1.0;
    for(int i = 0; i <= order; i++){
        if(i!=0) fact *= (2*i)*(2*i+1);
        result += (pow(-1.0, i)) * (pow(x, 2*i+1)/fact);
    }
    return result;

}

fixed SINE_FIXED(fixed x, int order, int ver){
    fixed one = FLOAT_TO_FIXED(1.0); 
    int fact = 1;
    fixed x_pow = x;
    fixed x_squared = FIXED_MULT(x, x); 
    fixed fact_term = 1;
    
    fixed result = x_pow / fact;
    // if(ver) printf("\nFirst Result: %d => x/fact = %d / %d = %d\n", result, x_pow, fact, x_pow/fact);
    if(ver) printf("\nFirst Result: %f => x/fact = %f / %f = %f\n", FIXED_TO_FLOAT(result), FIXED_TO_FLOAT(x_pow), ((float)fact), FIXED_TO_FLOAT(x_pow/fact));

    for(int i = 1; i <= order; i++){
        x_pow = FIXED_MULT( x_pow, x_squared);
        fact = fact * (fact_term=fact_term+1) * (fact_term=fact_term+1);
        if(fact == 0) fact = 1;
        if( i % 2 == 0){
            result += x_pow / fact;
            // if(ver) printf("pos %d, Result: %d => x/fact = %d / %d = %d\n", i, result, x_pow, fact, x_pow/fact);
            if(ver) printf("pos %d, Result: %f => x/fact = %f / %f = %f\n", i, FIXED_TO_FLOAT(result), FIXED_TO_FLOAT(x_pow), ((float)fact), FIXED_TO_FLOAT(x_pow/fact));
            continue;
            
        }else{
            result -= x_pow / fact;
            // if(ver) printf("neg %d, Result: %d => x/fact = %d / %d = %d\n", i, result, x_pow, fact, x_pow/fact);
            if(ver) printf("neg %d, Result: %f => x/fact = %f / %f = %f\n", i, FIXED_TO_FLOAT(result), FIXED_TO_FLOAT(x_pow), ((float)fact), FIXED_TO_FLOAT(x_pow/fact));
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

float dataset[10] = {0.0, -M_PI, M_PI/6, M_PI/4, M_PI_2, 2*M_PI/3, M_PI, -0.99999, -1, 1};

int main(){
    
    
    
    float min_err = FLT_MAX;
    int min_m = 0;
    for(int i = 1; i <= 31; i++){
        SET_Q_FORMAT(i, 31-i);
        // printf("%d, %d\n", i, 31-i);
        float error = 0;
        float benchmark = 0.0;
        float computed = 0.0;
        for(int j = 0; j < 10; j++){
            float x = dataset[j];
            benchmark = sinf(x);
            // computed = SINE_FL(x,10);
            computed = FIXED_TO_FLOAT(SINE_FIXED(FLOAT_TO_FIXED(x), 6, 0));
            float temp_error = benchmark - computed;
            if(temp_error < 0) temp_error = temp_error * -1;
            error +=temp_error;
            if(i==23) printf("%f: %f\n", dataset[j], computed);
            // printf("sin(%f): %f (math.h), %f (float imp.) - Error: %f\n", x, benchmark, computed, temp_error);
        }
        error = error /1;
        // printf("Total Error: %f\n==========\n", error);
        if(error < min_err){
            min_err = error;
            min_m = i;
        }
    // printf("2pi: %f, %d, %f\n", 2*M_PI, FLOAT_TO_FIXED(2*M_PI), FIXED_TO_FLOAT(FLOAT_TO_FIXED(2*M_PI)) );

    }
    SET_Q_FORMAT(16, 31-16);
    printf("Min Error: %f, Format: (%d, %d)\n", min_err, min_m, 31-min_m);
    // for(int i = 0; i < 10; i++){
    //     printf("%f, %d\n", dataset[i], FLOAT_TO_FIXED(dataset[i]));
    // }
	return 0;
} 
