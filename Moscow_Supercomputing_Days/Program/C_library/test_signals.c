#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include "file.h"
#include "computation_functions.h"


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
	char file_name[80];
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
