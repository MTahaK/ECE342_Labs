#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <float.h>
#include "altera_avalon_performance_counter.h"
#include <sys/alt_irq.h>
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

void EMA_FILTER(float data[10], float out[10]){
	fixed alpha = FLOAT_TO_FIXED(0.1);
	fixed prev = FLOAT_TO_FIXED(data[0]);
	out[0] = FIXED_TO_FLOAT(prev);

	for(int i = 1; i < 10; i++){
		fixed conv = FLOAT_TO_FIXED(data[i]);
		fixed s_i = FIXED_MULT(alpha, conv) + FIXED_MULT((1 - alpha), prev);
		prev = conv;
		out[i] = FIXED_TO_FLOAT(s_i);
	}
}
void EMA_FLOAT(float data[10], float out[10]){
	out[0] = data[0];
	for(int i = 1; i < 10; i++){
		out[i] = (0.1*data[i]) + (1-0.1)*data[i-1];
	}
}

// For each of these datasets, find the Qm.n representation that minimizes the error.
// You only need to consider the following values of m : 4, 8, 16 and 24. 
float datasets[4][10] = {{429.53605647241415, 54.051172931707704, 163.58682870876768, 580.5297854212683, 699.4746398396891, 816.6555570791686, 552.2376581729087, 754.3120681461689, 85.19740340786541, 190.84266131541648},
{120069.86565516416, 21469.916452351572, 193920.6012388812, 201536.93026739376, 29439.382625284743, 155926.95756308752, 244118.3902060167, 21616.645699486355, 113960.0450136428, 125626.1350580619},
{0.07402272300410051, 4.0733941539004475, 2.7780734593791974, 4.647096565449923, 4.760313581913216, 1.0700010090755887, 5.999940306877167, 0.09692246551677397, 6.232799120276484, 4.409500084502266},
{18.922375022588515, 9.976071114431022, 1.1578493397893121, 16.946195430873466, 13.258356573949856, 22.789397104263998, 4.467453001844939, 28.000955923708002, 9.65995387643512, 27.66319226833275}};


static alt_irq_context context; /* Use when disabling interrupts. */

static void pre_measurement(void){
  PERF_RESET (PERFORMANCE_COUNTER_0_BASE);
  context = alt_irq_disable_all();
  PERF_START_MEASURING (PERFORMANCE_COUNTER_0_BASE);  
}

static void post_measurement(void){
	alt_irq_enable_all(context);
	PERF_STOP_MEASURING (PERFORMANCE_COUNTER_0_BASE);	
}

// Un-comment these options to disable the use of the floating-point hardware
// for the respective operation.
// #pragma GCC target("no-custom-fadds")
// #pragma GCC target("no-custom-fsubs")
// #pragma GCC target("no-custom-fmuls")
// #pragma GCC target("no-custom-fdivs")

#define NUM_ITEMS 100

int main(void){

	volatile int a = 234;
	volatile int result;


	SET_Q_FORMAT(24, 31-24);
	fixed d1[10];
	fixed d2[10];
	for(int i = 0; i < 10; i++){
		d1[i] = FLOAT_TO_FIXED(datasets[2][i]);
	}
	for(int i = 0; i < 10; i++){
		d2[i] = FLOAT_TO_FIXED(datasets[3][i]);
	}
	float emafl1[10];
	float emafl[10];
	float emafi[10];
	EMA_FLOAT(datasets[2], emafl1);
	EMA_FLOAT(datasets[3], emafl);
	fixed alpha = FLOAT_TO_FIXED(0.1);
	fixed prev1 = FLOAT_TO_FIXED(datasets[2][0]);
	fixed prev = d2[0];
	pre_measurement();
	
	
	for (int i=0; i<NUM_ITEMS; i++){
		PERF_BEGIN (PERFORMANCE_COUNTER_0_BASE, 1);
		emafi[0] = d2[0];
		for(int i = 1; i < 10; i++){
			fixed conv = d2[i];
			fixed first = (alpha*conv) >> Q_N;
			fixed second = (1-alpha) * prev) >> Q_N;
			fixed s_i = first + second;
			prev = conv;
			emafi[i] = (float)(s_i)/F_ONE;
		}
		result = a^a;		
		PERF_END (PERFORMANCE_COUNTER_0_BASE, 1);
	}
	
	post_measurement();
	float error = 0;
	for(int j = 0; j < 10; j++){
		float temp_err = emafl[j] - emafi[j];
		printf("%f, %f, err: %f\n", emafl[j], emafi[j], temp_err);
		if(temp_err<0) temp_err = temp_err * -1;
		error += temp_err;
	}
	error = error / 10;
	printf("Error in EMA calculations in Q%d.%d: %f\n", Q_M, Q_N, error);

	int overhead_cycles = (int)perf_get_section_time ((void*)PERFORMANCE_COUNTER_0_BASE, 1);

	printf("Overhead:%f\n", (float)overhead_cycles/NUM_ITEMS);

	while(1);
}
