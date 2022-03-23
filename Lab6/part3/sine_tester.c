#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <float.h>
// #include "system.h"
// #include "altera_avalon_performance_counter.h"
// #include <sys/alt_irq.h>
#include <time.h>


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
        // printf("%f\n", FLOAT_FACT(2*i+1));
        // if(i!=0) x_pow = x_pow * x * x;
        // printf("%f\n", x_po  w);
        if(i!=0) fact *= (2*i)*(2*i+1);
        result += (pow(-1.0, i)) * (pow(x, 2*i+1)/fact);
    }
    return result;

}

static inline fixed SINE_FIXED(fixed x, int order, int ver){
    fixed one = FLOAT_TO_FIXED(1.0);
    int fact = 1;
    fixed x_pow = x;
    fixed x_squared = FIXED_MULT(x, x);
    fixed fact_term = 1;
    fixed result = x_pow / fact;

    for(int i = 1; i <= order; i++){
        x_pow = FIXED_MULT( x_pow, x_squared);
        fact = fact * (fact_term+1) * (fact_term+2);
        fact_term+=2;
        if(fact == 0) fact = 1;
        if( i % 2 == 0){
            result += x_pow / fact;

        }else{
            result -= x_pow / fact;
        }
    }
    return result;

}
struct Q_format{
	int m;
	float error;
};
typedef struct Q_format Q_format;

float dataset[10] = {0.0, -M_PI, M_PI/6, M_PI/4, M_PI_2, 2*M_PI/3, M_PI, -0.99999, -1, 1};


// static alt_irq_context context; /* Use when disabling interrupts. */

// static void pre_measurement(void){
//   PERF_RESET (PERFORMANCE_COUNTER_0_BASE);
//   context = alt_irq_disable_all();
//   PERF_START_MEASURING (PERFORMANCE_COUNTER_0_BASE);
// }

// static void post_measurement(void){
// 	alt_irq_enable_all(context);
// 	PERF_STOP_MEASURING (PERFORMANCE_COUNTER_0_BASE);
// }

// Un-comment these options to disable the use of the floating-point hardware
// for the respective operation.
// #pragma GCC target("no-custom-fadds")
// #pragma GCC target("no-custom-fsubs")
// #pragma GCC target("no-custom-fmuls")
// #pragma GCC target("no-custom-fdivs")

#define NUM_ITEMS 100

int main(void){

//	volatile int a = 234;
//		volatile int result;

	SET_Q_FORMAT(23, 31-23);
    fixed conv_dataset[10];
    for(int i = 0;i < 10; i++){
    	conv_dataset[i] = FLOAT_TO_FIXED(dataset[i]);
		// printf("%f, %d\n", dataset[i], conv_dataset[i]);
    }
	fixed one = FLOAT_TO_FIXED(1.0);
	fixed x_pow = 0;
	fixed x_squared = 0;
	
	fixed results[10];
	

	float fl_result = 0.0;
    float fl_fact = 1.0;
	float fl_results[10];
	int order = 6;

	// pre_measurement();

	// Fixed Point
	for (int j=0; j<NUM_ITEMS; j++){
		// PERF_BEGIN (PERFORMANCE_COUNTER_0_BASE, 1);
		for(int i = 0; i < 10; i++){
    		int fact_term = 1;
    		int fact = 1;
			x = conv_dataset[i];
			x_pow = x;
			x_squared = FIXED_MULT(x, x);
			fixed result = x_pow / fact;
			for(int k = 1; k <= order; k++){
				x_pow = FIXED_MULT( x_pow, x_squared);
				fact = fact * (fact_term+1) * (fact_term+2);
				fact_term=fact_term+2;
				if(fact == 0) fact = 1;
				if( k % 2 == 0){
					result += x_pow / fact;

				}else{
					result -= x_pow / fact;
        		}
			results[k] = result;
			}
    	}

		// PERF_END (PERFORMANCE_COUNTER_0_BASE, 1);
	}
	// post_measurement();
//	// Floating Point


//	pre_measurement();
//

	for (int j=0; j<NUM_ITEMS; j++){
		// PERF_BEGIN (PERFORMANCE_COUNTER_0_BASE, 1);
		for(int j = 0; j < 10; j++){
			float x_fl = dataset[j];
			for(int i = 0; i <= order; i++){
				if(i!=0) fl_fact *= (2.0*i)*(2.0*i+1.0);
				fl_result += (pow(-1.0, i)) * (pow(x_fl, 2.0*i+1.0)/fl_fact);
			}
			fl_results[j] = fl_result;
		}
		// PERF_END (PERFORMANCE_COUNTER_0_BASE, 1);
	}
//
//
//	post_measurement();


	float conv_results[10];
	// double  time_taken = difftime(start, time(NULL));
	// for(int i = 0; i < 10; i++){
	// 	printf("%f: %f, %f\n", dataset[i], fl_results[i], FIXED_TO_FLOAT(results[i]));
	// }
	float error = 0;
	for(int j = 0; j < 10; j++){
		conv_results[j] = FIXED_TO_FLOAT(results[j]);
		printf("%f: %f\n", dataset[j], conv_results[j]);
		float temp_err = fl_results[j] - conv_results[j];
		if(temp_err<0) temp_err = temp_err * -1;
		error += temp_err;
	}
	error = error / 10;
	printf("Error in sine calculations in Q%d.%d: %f\n", Q_M, Q_N, error);


//	int fixed_point_time = (int)perf_get_section_time ((void*)PERFORMANCE_COUNTER_0_BASE, 1);
	//  int floating_point_time = (int)perf_get_section_time ((void*)PERFORMANCE_COUNTER_0_BASE, 1);

//	float fixed_cycles = (float)((fixed_point_time-606)/NUM_ITEMS);
	//  float floating_cycles = (float)((floating_point_time-606)/NUM_ITEMS);


//	printf("Fixed Point:%f\n", fixed_cycles);
	//  printf("Cycles: %f\n", floating_cycles);

	// while(1);
	return 0;
}


