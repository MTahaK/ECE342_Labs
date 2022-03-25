#include <stdio.h>
#include <math.h>
#include <float.h>
#include <string.h>
#include "datasets.h"

typedef int fixed;

int Q_M = 0;
int Q_N = 0;
int F_ONE = 0;

void SET_Q_FORMAT(int M, int N)
{
	Q_M = M;
	Q_N = N;
	F_ONE = 1 << N;
}

int FLOAT_TO_FIXED(float f)
{
	return (fixed)(f * F_ONE);
}

float FIXED_TO_FLOAT(fixed f)
{
	return (float)(f) / F_ONE;
}

fixed FIXED_MULT(fixed op1, fixed op2)
{

	// calculate the result of this multiplication
	// and return it here.

	return (op1 * op2) >> Q_N;
}

void EMA_FILTER(float data[10], float out[10])
{
	fixed alpha = FLOAT_TO_FIXED(0.1);
	fixed diff = FLOAT_TO_FIXED(0.9);
	out[0] = FIXED_TO_FLOAT(FLOAT_TO_FIXED(data[0]));

	for (int i = 1; i < 10; i++)
	{
		fixed conv = FLOAT_TO_FIXED(data[i]);
		fixed s_i = FIXED_MULT(alpha, conv) + FIXED_MULT(diff, FLOAT_TO_FIXED(out[i-1]));
		out[i] = FIXED_TO_FLOAT(s_i);
	}
}

void EMA_FLOAT(float data[10], float out[10])
{
	out[0] = data[0];
	for (int i = 1; i < 10; i++)
	{
		out[i] = (0.1 * data[i]) + (1 - 0.1) * out[i - 1];
	}
}
// For each of these datasets, find the Qm.n representation that minimizes the error.
// You only need to consider the following values of m : 4, 8, 16 and 24.
// float datasets[4][10] = {{429.53605647241415, 54.051172931707704, 163.58682870876768, 580.5297854212683, 699.4746398396891, 816.6555570791686, 552.2376581729087, 754.3120681461689, 85.19740340786541, 190.84266131541648},
// {120069.86565516416, 21469.916452351572, 193920.6012388812, 201536.93026739376, 29439.382625284743, 155926.95756308752, 244118.3902060167, 21616.645699486355, 113960.0450136428, 125626.1350580619},
// {0.07402272300410051, 4.0733941539004475, 2.7780734593791974, 4.647096565449923, 4.760313581913216, 1.0700010090755887, 5.999940306877167, 0.09692246551677397, 6.232799120276484, 4.409500084502266},
// {18.922375022588515, 9.976071114431022, 1.1578493397893121, 16.946195430873466, 13.258356573949856, 22.789397104263998, 4.467453001844939, 28.000955923708002, 9.65995387643512, 27.66319226833275}};

struct Q_format
{
	int m;
	float error;
};
typedef struct Q_format Q_format;

int main(int argc, char *argv[])
{

	// if(argc > 2){
	// 	printf("Too many arguments supplied - use argument 'full' to print out all intermediary outputs.\n");
	// 	return 0;
	// }

	Q_format m4, m8, m16, m24;

	m4.m = 4;
	m8.m = 8;
	m16.m = 16;
	m24.m = 24;

	Q_format formats[] = {m4, m8, m16, m24};
	// for(int i = 0; i < 4; i++){

	// 	m4.error = m8.error = m16.error = m24.error = 0.0;
	// 	int min_m = 0;
	// 	float min_err = FLT_MAX;

	// 	for(int j = 0; j < 4; j++){

	// 		// m4.error = m8.error = m16.error = m24.error = 0.0;
	// 		SET_Q_FORMAT(formats[j].m, 31-formats[j].m);
	// 		m4.error = m8.error = m16.error = m24.error = 0.0;
	// 		for(int k = 0; k < 10; k++){
	// 			int fixed_rep = FLOAT_TO_FIXED(datasets[i][k]);
	// 			float reconv = FIXED_TO_FLOAT(fixed_rep);
				
	// 			// printf("M=%d: %f => %d => %f\n", formats[j].m, datasets[i][k], fixed_rep, reconv);
				
	// 			float temp_err = fabs(datasets[i][k] - reconv);
	// 			// if(temp_err < 0) temp_err = temp_err * -1;
	// 			formats[j].error += temp_err;
	// 		}
	// 		formats[j].error = formats[j].error / 10;
	// 		if(formats[j].error < min_err){
	// 			min_err = formats[j].error;
	// 			min_m = formats[j].m;
	// 		}

	// 		printf("Error on sample set in Q%d.%d: %f\n", Q_M, Q_N, formats[j].error);
	// 		formats[j].error = 0.0;

	// 	}
	// 	printf("\n============================\nDataset %d Min Error: %f, using M=%d\n============================\n", i+1, min_err, min_m);
	// 	min_m = 0;
	// 	min_err = FLT_MAX;

	// }

	// float op1fl = 1.0;
	// float op2fl = 2.0;
	// int o1 = FLOAT_TO_FIXED(op1fl);
	// int o2 = FLOAT_TO_FIXED(op2fl);
	// printf("%f * %f in FIXED = %d, %f reconv. Orig: %f\n", op1fl, op2fl, FIXED_MULT(o1, o2), FIXED_TO_FLOAT(FIXED_MULT(o1, o2)), op1fl*op2fl );

	// printf("\nEMA FILTER ERROR TESTING:\n");

	float emafi[10], emafl[10];
	m4.error = m8.error = m16.error = m24.error = 0.0;
	int min_m = 0;
	float min_err = FLT_MAX;
	for (int i = 0; i < 4; i++)
	{

		SET_Q_FORMAT(formats[i].m, 31 - formats[i].m);
		EMA_FILTER(datasets[2], emafi);
		EMA_FLOAT(datasets[2], emafl);

	
		printf("M = %d, Dataset: [3]\n", formats[i].m, i);
		
		for (int j = 0; j < 10; j++)
		{
			float temp_err = fabs(emafl[j] - emafi[j]);
			// printf("Floating: %f, Fixed: %f, err: %f\n", emafl[j], emafi[j], temp_err);
			formats[i].error += temp_err;
		}
		formats[i].error = formats[i].error / 10;
		if (formats[i].error < min_err)
		{
			min_err = formats[i].error;
			min_m = formats[i].m;
		}

		printf("Error in EMA calculations in Q%d.%d: %f\n", Q_M, Q_N, formats[i].error);
		formats[i].error = 0;
	}

	printf("\n============================\nEMA on Dataset 3: Min Error: %f, using M=%d\n============================\n", min_err, min_m);
	m4.error = m8.error = m16.error = m24.error = 0.0;
	min_m = 0;
	min_err = FLT_MAX;
	for (int i = 0; i < 4; i++)
	{

		SET_Q_FORMAT(formats[i].m, 31 - formats[i].m);
		EMA_FILTER(datasets[3], emafi);
		EMA_FLOAT(datasets[3], emafl);


		printf("M = %d, Dataset: [4]\n", formats[i].m, i);
		
		for (int j = 0; j < 10; j++)
		{
			float temp_err = fabs(emafl[j] - emafi[j]);
			// printf("Floating: %f, Fixed: %f, err: %f\n", emafl[j], emafi[j], temp_err);
			formats[i].error += temp_err;
		}
		formats[i].error = formats[i].error / 10;
		if (formats[i].error < min_err)
		{
			min_err = formats[i].error;
			min_m = formats[i].m;
		}

		printf("Error in EMA calculations in Q%d.%d: %f\n", Q_M, Q_N, formats[i].error);
		formats[i].error = 0;
	}

	printf("\n============================\nEMA on Dataset 4: Min Error: %f, using M=%d\n============================\n", min_err, min_m);

	return 0;
}
