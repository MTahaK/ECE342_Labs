#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <float.h>
#include "system.h"
#include "altera_avalon_performance_counter.h"
#include <sys/alt_irq.h>
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

static inline int FLOAT_TO_FIXED(float f){
	return (fixed)(f * F_ONE);
}

static inline float FIXED_TO_FLOAT(fixed f){
	return (float)(f)/F_ONE;
}

static inline fixed FIXED_MULT(fixed op1, fixed op2){
	return (op1 * op2) >> Q_N;
}
void EMA_FLOAT(float data[10], float out[10]){
	out[0] = data[0];
	for(int i = 1; i < 10; i++){
		out[i] = (0.1*data[i]) + (1-0.1)*data[i-1];
	}
}

// For each of these datasets, find the Qm.n representation that minimizes the error.
// You only need to consider the following values of m : 4, 8, 16 and 24.
// float datasets[4][10] = {{429.53605647241415, 54.051172931707704, 163.58682870876768, 580.5297854212683, 699.4746398396891, 816.6555570791686, 552.2376581729087, 754.3120681461689, 85.19740340786541, 190.84266131541648},
// {120069.86565516416, 21469.916452351572, 193920.6012388812, 201536.93026739376, 29439.382625284743, 155926.95756308752, 244118.3902060167, 21616.645699486355, 113960.0450136428, 125626.1350580619},
// {0.07402272300410051, 4.0733941539004475, 2.7780734593791974, 4.647096565449923, 4.760313581913216, 1.0700010090755887, 5.999940306877167, 0.09692246551677397, 6.232799120276484, 4.409500084502266},
// {18.922375022588515, 9.976071114431022, 1.1578493397893121, 16.946195430873466, 13.258356573949856, 22.789397104263998, 4.467453001844939, 28.000955923708002, 9.65995387643512, 27.66319226833275}};

float datasets[4][10] = {
    {953.719492564774, 1386.4900999229606, 413.77211455491334, 969.3928032514359, 1407.935265320118, 1151.4737485111898, 951.4091060969595, 90.01095579004799, 1544.4545295602322, 1547.6125331076184},

    {37.11573877282737, 110.2244276395097, 73.84148832799397, 0.7303245588438511, 59.51482116160429, 22.624547275316957, 61.86862462726829, 73.21019546083052, 79.52048412956717, 107.61291538127792},
    
    {5.868417167987105, 2.269600113986436, 4.393357085048192, 0.750565860328428, 6.081457592188938, 1.1258856665435557, 4.146100992982636, 1.1676705540118277, 2.6981174753851844, 6.818996255170779},
    
    {41585.23095566271, 66712.28745045475, 79416.60577202369, 39600.15691339665, 82660.66156177265, 18218.498895458128, 77669.67202852937, 129212.96467414031, 9868.191611084774, 40789.970328807554}
};


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
//  #pragma GCC target("no-custom-fadds")
//  #pragma GCC target("no-custom-fsubs")
//  #pragma GCC target("no-custom-fmuls")
//  #pragma GCC target("no-custom-fdivs")

#define NUM_ITEMS 100

int main(void){

	SET_Q_FORMAT(24, 31-24);
	float d1[10];
	for(int i = 0; i < 10; i++){
		d1[i] = datasets[2][i];
	}
	fixed d2[10];
	for(int i = 0; i < 10; i++){
		d2[i] = FLOAT_TO_FIXED(datasets[2][i]);
	}
	float ema1[10];
	fixed emafi[10];
	fixed alpha = FLOAT_TO_FIXED(0.1);
	fixed diff = FLOAT_TO_FIXED(0.9);


	// FIXED POINT

	// pre_measurement();

	for (int j=0; j<NUM_ITEMS; j++){
		// PERF_BEGIN (PERFORMANCE_COUNTER_0_BASE, 1);

		emafi[0] = d2[0];
		for(int i = 1; i < 10; i++){
			emafi[i] = FIXED_MULT(alpha, d2[i]) + FIXED_MULT(diff, emafi[i-1]);
		}

		// PERF_END (PERFORMANCE_COUNTER_0_BASE, 1);
	}


	// post_measurement();

	// FLOATING POINT
// 
	pre_measurement();

	for (int j=0; j<NUM_ITEMS; j++){
		PERF_BEGIN (PERFORMANCE_COUNTER_0_BASE, 1);

		ema1[0] = d1[0];
		for(int i = 1; i < 10; i++){
			ema1[i] = (0.1f*d1[i]) + (0.9f)*ema1[i-1];
		}

		PERF_END (PERFORMANCE_COUNTER_0_BASE, 1);
	}


	post_measurement();


	// double  time_taken = difftime(start, time(NULL));

	float error = 0;
	for(int j = 0; j < 10; j++){
		float temp_err = fabs(ema1[j] - FIXED_TO_FLOAT(emafi[j]));
		// printf("Float Calc: %f, Fixed Calc: %f, err: %f\n", ema1[j], FIXED_TO_FLOAT(emafi[j]), temp_err);
		error += temp_err;
	}
	error = error / 10;
	printf("Error in EMA calculations in Q%d.%d: %f\n", Q_M, Q_N, error);


	int section_time = (int)perf_get_section_time ((void*)PERFORMANCE_COUNTER_0_BASE, 1);

	float cycles = (float)((section_time-606)/NUM_ITEMS);


	printf("Cycles (Hardware Floating Point): %f\n", cycles);

	while(1);
}
