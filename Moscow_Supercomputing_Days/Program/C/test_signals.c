#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>


void ReadData(double **signals, int signals_number, int signals_length)
{
	printf("Имя файла с данными о сигналах: ");
	char file_name[80];
	scanf("%s", file_name);
	FILE *f = fopen(file_name, "r");
	for (int i = 0; i < signals_number; i++)
	{
		for(int j = 0; j < signals_length; j++)
		{
			fscanf(f,"%lf ", &signals[i][j]);
		}
		fscanf(f,"\n");
	}	
	fclose(f);
}

void ReadMatrixFromFile(double ** matrix, char *file_name, int x, int y)
{
	FILE *fr = fopen(file_name, "r");
	for (int i = 0; i < x; i++)
	{
		for(int j = 0; j < y; j++)
		{
			fscanf(fr, "%lf ", &matrix[i][j]);
		}
		fscanf(fr, "\n");
	}	
	fclose(fr);
}

void WriteMatrixToFile(const double ** matrix, char *file_name, int x, int y)
{
	FILE *fw = fopen(file_name, "w");
	for (int i = 0; i < x; i++)
	{
    	for (int j = 0; j < y; j++)
		{
			fprintf(fw, "%f ", matrix[i][j]);
		}
		fprintf(fw, "\n");
	}
	fclose(fw);
}

void WriteResults(double delta, double deltaDFT)
{
	printf("Имя файла для сохранения результата: ");
	char file_name[80];
	scanf("%s", file_name);
	FILE *fw = fopen(file_name, "w");
	fprintf(fw, "Среднее отклонение при сжатии найденной матрицей: %f\n", delta);
	fprintf(fw, "Среднее отклонение при сжатии матрицей ДПФ: %f\n", deltaDFT);
	fclose(fw);
}

void WriteResultsByOne(double* delta, double* deltaDFT, int counter, int signals_number, int signals_length)
{
	printf("Имя файла для сохранения результата: ");
	char file_name[80];
	scanf("%s", file_name);
	FILE *fw = fopen(file_name, "w");
	fprintf(fw, "Среднее отклонение сигналов при сжатии найденной матрицей:");
	for (int i = 0; i < signals_number; i++)
	{
		fprintf(fw, " %6.3f", delta[i]);
	}
	fprintf(fw, "\nСреднее отклонение сигналов при сжатии матрицей ДПФ:");
	for (int i = 0; i < signals_number; i++)
	{
		fprintf(fw, " %6.3f", deltaDFT[i]);
	}
	fprintf(fw, "\n");
	fprintf(fw, "Количество сигналов, для которых ДПФ эффективнее найденной матрциы %d", counter);
	fclose(fw);
}

void Filter(double **G, int signals_number, int signals_length)
{
	int *index = (int *)malloc(signals_length * sizeof(int));
	for (int p = 0; p < signals_number; p++)
	{
		for (int k = 0; k < signals_length; k++)
		{
			index[k] = 1;
			for (int k1 = 0; k1 < signals_length; k1++)
			{
				if (fabs(G[p][k]) < fabs(G[p][k1]))
				{
			  		index[k]++;
				}
			}
		}
		for (int k = 0; k < signals_length; k++)
		{
			if (index[k] > 2)
			{
				G[p][k] = 0;
			}
		}
	}
	free(index);
}

void ComputeDFT(double ** matrix, int signals_length)
{
	matrix[0][0] = 1 / sqrt(2 * signals_length - 1);
 	for (int j = 1; j < signals_length; j++)
	{
		matrix[0][j] = 2 / sqrt(2 * signals_length - 1);
	}
  	for (int i = 1; i < signals_length; i++)
	{
		matrix[i][0] = 1 / sqrt(2 * signals_length - 1);
		for (int j = 1; j < signals_length; j++)
		{
			matrix[i][j] = 2 * cos(2 * acos(-1.0) * i * j / (2 * signals_length - 1)) / sqrt(2 * signals_length - 1);
		}
	}
}

void multiply_vector_by_matrix(double *res, const double **A, const double* B, int signals_length)
{
	for (int i = 0; i < signals_length; i++)
	{
		res[i] = 0;
		for (int j = 0; j < signals_length; j++)
		{
			res[i] += A[i][j] * B[j];
		}
	}
}

double SignalConversion(const double **signals, const double **S, const double **T, int signals_number, int signals_length)
{
	double **G = (double **)malloc(signals_number * sizeof(double*));
	double **F1 = (double **)malloc(signals_number * sizeof(double*));

	for (int i = 0; i < signals_number; i++)
	{
		G[i] = (double *)malloc(signals_length * sizeof(double));
		F1[i] = (double *)malloc(signals_length * sizeof(double));
	}

	for (int p = 0; p < signals_number; p++)
	{
		multiply_vector_by_matrix(G[p], T, signals[p], signals_length);
	}

	//Фильтрация
	Filter(G, signals_number, signals_length);

	//Обратное преобразование сигналов
	for (int p = 0; p < signals_number; p++)
	{
		multiply_vector_by_matrix(F1[p], S, G[p], signals_length);
	}
	//Расчет среднего отклонения
	double delta1 = 0;
	double delta_buf = 0;
	for (int p = 0; p < signals_number; p++)
	{
		delta_buf = 0;
		for (int k = 0; k < signals_length; k++)
		{
			delta_buf += (signals[p][k]-F1[p][k])*(signals[p][k]-F1[p][k])/(signals_number * signals_number * signals_length);
		}
		delta1 += sqrt(delta_buf);
	}
	for (int i = 0; i < signals_number; i++)
	{
		free(G[i]);	
		free(F1[i]);
	}
	free(G);	
	free(F1);
	return delta1;
}

double* SignalConversionByOne(const double **signals, const double **S, const double **T, int signals_number, int signals_length)
{
	double **G = (double **)malloc(signals_number * sizeof(double*));
	for (int i = 0; i < signals_number; i++)
	{
		G[i] = (double *)malloc(signals_length * sizeof(double));
	}
	double **F1 = (double **)malloc(signals_number * sizeof(double*));
	for (int i = 0; i < signals_number; i++)
	{
		F1[i] = (double *)malloc(signals_length * sizeof(double));
	}
	double *delta = (double *)malloc(signals_number * sizeof(double));

	for (int p = 0; p < signals_number; p++)
	{
		multiply_vector_by_matrix(G[p], T, signals[p], signals_length);
	}

	//Фильтрация
	Filter(G, signals_number, signals_length);

	//Обратное преобразование сигналов
	for (int p = 0; p < signals_number; p++)
	{
		multiply_vector_by_matrix(F1[p], S, G[p], signals_length);
	}
	//Расчет среднего отклонения
	for (int p = 0; p < signals_number; p++)
	{
		double delta_buf = 0;
		for (int k = 0; k < signals_length; k++)
		{
			delta_buf += (signals[p][k]-F1[p][k]) * (signals[p][k]-F1[p][k]) / signals_length;
		}
		delta[p] = sqrt(delta_buf);
	}
	for (int i = 0; i < signals_number; i++)
	{
		free(G[i]);	
		free(F1[i]);
	}
	free(G);
	free(F1);
	return delta;
}

int main(int argc, char ** argv) {
	int signals_number;	
	int signals_length;
	printf("Количество сигналов в выборке = ");
	scanf("%d", &signals_number);
	printf("Длина сигналов в выборке = ");
	scanf("%d", &signals_length);

	double **signals = (double **)malloc(signals_number * sizeof(double*));
	double **S = (double **)malloc(signals_length * sizeof(double*));
	double **T = (double **)malloc(signals_length * sizeof(double*));
	double **DFT = (double **)malloc(signals_length * sizeof(double*));

	for (int i = 0; i < signals_number; i++)
	{
		signals[i] = (double *)malloc(signals_length * sizeof(double));
	}
	ReadData(signals, signals_number, signals_length);

	for (int i = 0; i < signals_length; i++)
	{
		S[i] = (double *)malloc(signals_length * sizeof(double));
		T[i] = (double *)malloc(signals_length * sizeof(double));
		DFT[i] = (double *)malloc(signals_length * sizeof(double));
	}
	ComputeDFT(DFT, signals_length);
	//printf("Имя файла с данными о матрице ДПФ: ");
	char file_name[80];
	//ComputeDFT(DFT);
	//scanf("%s", file_name);
	//ReadMatrixFromFile(DFT, file_name);
	printf("Имя файла с данными о найденной матрице S: ");
	scanf("%s", file_name);
	ReadMatrixFromFile(S, file_name, signals_length, signals_length);
	printf("Имя файла с данными о найденной матрице T: ");
	scanf("%s", file_name);
	ReadMatrixFromFile(T, file_name, signals_length, signals_length);

	double delta = SignalConversion(signals, S, T, signals_number, signals_length);
	double deltaDFT = SignalConversion(signals, DFT, DFT, signals_number, signals_length);
	double *deltas = SignalConversionByOne(signals, S, T, signals_number, signals_length);
	double *deltasDFT = SignalConversionByOne(signals, DFT, DFT, signals_number, signals_length);
	
	int counter = 0;
	for (int i = 0; i < signals_number; i++)
	{
		if(deltas[i] > deltasDFT[i])
		{
			counter += 1;
		}
	}
	WriteResults(delta, deltaDFT);
	WriteResultsByOne(deltas, deltasDFT, counter, signals_number, signals_length);
	
	free(deltas);
	free(deltasDFT);

	for (int i = 0; i < signals_length; i++)
	{
		free(S[i]);
		free(T[i]);
		free(DFT[i]);
	}
	for (int i = 0; i < signals_number; i++)
	{
		free(signals[i]);
	}

	free(signals);
	free(S);
	free(T);
	free(DFT);
	return 0;
}
