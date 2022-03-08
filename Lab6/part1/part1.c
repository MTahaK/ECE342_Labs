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

	

}

// For each of these datasets, find the Qm.n representation that minimizes the error.
// You only need to consider the following values of m : 4, 8, 16 and 24. 
float datasets[4][10] = {{429.53605647241415, 54.051172931707704, 163.58682870876768, 580.5297854212683, 699.4746398396891, 816.6555570791686, 552.2376581729087, 754.3120681461689, 85.19740340786541, 190.84266131541648},
{120069.86565516416, 21469.916452351572, 193920.6012388812, 201536.93026739376, 29439.382625284743, 155926.95756308752, 244118.3902060167, 21616.645699486355, 113960.0450136428, 125626.1350580619},
{0.07402272300410051, 4.0733941539004475, 2.7780734593791974, 4.647096565449923, 4.760313581913216, 1.0700010090755887, 5.999940306877167, 0.09692246551677397, 6.232799120276484, 4.409500084502266},
{18.922375022588515, 9.976071114431022, 1.1578493397893121, 16.946195430873466, 13.258356573949856, 22.789397104263998, 4.467453001844939, 28.000955923708002, 9.65995387643512, 27.66319226833275}};

struct Q_format{
	int m;
	float error;
};
typedef struct Q_format Q_format;

int main(int argc, char *argv[]){

	// Add your code here.

	if(argc > 2){
		printf("Too many arguments supplied - use argument 'full' to print out all intermediary outputs.\n");
		return 0;
	}


	Q_format m4, m8, m16, m24;

	m4.m = 4; 
	m8.m = 8;
	m16.m = 16;
	m24.m = 24;
	
	Q_format formats[] = {m4, m8, m16, m24};
	
	for(int i = 0; i < 4; i++){
		m4.error = m8.error = m16.error = m24.error = 0.0;
		float *samples = datasets[i];
		int min_m = 0;
		float min_err = FLT_MAX;

		for(int j = 0; j < 4; j++){
			
			SET_Q_FORMAT(formats[j].m, 31-formats[j].m);
			if(argc == 2 && !strcmp("full",argv[1])){
				printf("\n\nM = %d, Dataset: [%d]\n", formats[j].m, i);
			}
			for(int k = 0; k < 10; k++){
				int fixed_rep = FLOAT_TO_FIXED(samples[k]);
				float reconv = FIXED_TO_FLOAT(fixed_rep);
				if(argc == 2 && !strcmp("full",argv[1])){
					printf("M=%d: %f => %d => %f\n", formats[j].m, samples[k], fixed_rep, reconv);
				}
				formats[j].error += samples[k] - reconv;
				
			}
			formats[j].error = formats[j].error / 10;
			if(formats[j].error < min_err){
					min_err = formats[j].error;
					min_m = formats[j].m;
			}
			
			printf("Error on sample set in Q%d.%d: %\n", Q_M, Q_N, formats[j].error);
			
			
		}
		printf("\n============================\nDataset %d Min Error: %f, using M=%d\n============================\n", i, min_err, min_m);
		
	}
	
	
	
	
	
	return 0;
} 
