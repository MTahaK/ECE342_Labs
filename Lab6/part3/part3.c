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

float FLOAT_FACT(float n){
    if(n==0){
        return 1;
    }
    else{
        return(n * FLOAT_FACT(n-1));
    }
}
float SINE_FL(float x, int order){
    float result = 0.0;
    float x_pow = x;
    for(int i = 0; i < order; i++){
        // printf("%f\n", FLOAT_FACT(2*i+1));
        // if(i!=0) x_pow = x_pow * x * x;
        // printf("%f\n", x_po  w);
        result += (pow(-1.0, i)) * (  pow(x, 2*i+1) / FLOAT_FACT(2*i+1) );
    }
    return result;

}
struct Q_format{
	int m;
	float error;
};
typedef struct Q_format Q_format;

float dataset[10] = {0.0, M_PI, M_PI_2, M_PI/4, M_PI/6, 2*M_PI ,  -M_PI, -0.99999, -1, 1};

int main(){
    
    for(int i = 0; i < 10; i++){
        float x = dataset[i];
        SINE_FL(x, 10);
        printf("sin(%f): %f (math.h), %f (float imp.)\n", x, sin(x), SINE_FL(x, 20));
    }
	return 0;
} 
