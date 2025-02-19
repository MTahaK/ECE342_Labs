#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <float.h>
#include "altera_avalon_performance_counter.h"
#include <sys/alt_irq.h>
#include "system.h"

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

	pre_measurement();

	for (int i=0; i<NUM_ITEMS; i++){
		PERF_BEGIN (PERFORMANCE_COUNTER_0_BASE, 1);
		result = a^a;
		PERF_END (PERFORMANCE_COUNTER_0_BASE, 1);
	}

	post_measurement();

	int overhead_cycles = (int)perf_get_section_time ((void*)PERFORMANCE_COUNTER_0_BASE, 1);

	printf("Overhead:%f\n", (float)overhead_cycles/NUM_ITEMS);

	while(1);
}
