// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <time.h> 
#include "lib/computation_functions.h"
#include "lib/file.h"

#define eps 0.01
#define accuracy 1000
#define compressionLevel 0.8

int main(int argc, char ** argv) {
	int signals_number;	
	int signals_length;
	printf("Количество сигналов в выборке = ");
	scanf("%d", &signals_number);
	printf("Длина сигналов в выборке = ");
	scanf("%d", &signals_length);

	double **signals = (double **)malloc(signals_number * sizeof(double*));
	for (int i = 0; i < signals_number; i++)
	{
		signals[i] = (double *)malloc(signals_length * sizeof(double));
	}
	ReadData(signals, signals_number, signals_length);

	double **S = (double **)malloc(signals_length * sizeof(double*));
	double **T = (double **)malloc(signals_length * sizeof(double*));
	double **S1 = (double **)malloc(signals_length * sizeof(double*));
	double **T1 = (double **)malloc(signals_length * sizeof(double*));
	double **Sbuf = (double **)malloc(signals_length * sizeof(double*));
	double **Tbuf = (double **)malloc(signals_length * sizeof(double*));
	for (int i = 0; i < signals_length; i++)
	{
		S[i] = (double *)malloc(signals_length * sizeof(double));
		T[i] = (double *)malloc(signals_length * sizeof(double));
		S1[i] = (double *)malloc(signals_length * sizeof(double));
		T1[i] = (double *)malloc(signals_length * sizeof(double));
		Sbuf[i] = (double *)malloc(signals_length * sizeof(double));
		Tbuf[i] = (double *)malloc(signals_length * sizeof(double));
	}
	double *alpha = (double *)malloc(signals_length * sizeof(double));

	ComputeDFT(S, signals_length);  
	
	printf("Сохранить матрицу ДПФ в файл? (y/n) ");
	char answer;
	scanf(" %c", &answer);
	if(answer == 'y') 
	{
		WriteMatrixToFile(S, "DFT.txt", signals_length, signals_length);
	}

	Copy(T, S, signals_length, signals_length);
	double deltaDFT = SignalConversion(signals, S, T, signals_number, signals_length);
	//Выбор оптимальной матрицы
	double delta = deltaDFT;
 	int better = 1;

	clock_t time = clock(); 

  	while (better)
	{
		better = 0;
      	for (int i = 0; i < signals_length; i++)
		{
			int best = 0;
			//Генерация преобразования по i-ой строке
			for (int q = 1; q <= round(exp(signals_length * log(3))); q++) 
			{
				Copy(S1, S, signals_length, signals_length);
				Copy(T1, T, signals_length, signals_length);
				int a = q-1;
				int b;
				int j_local = 0;
				while (a > 0)
				{
				  b = a % 3;
				  a = round((float)((a-b)/3.0));
				  S1[i][j_local] += (b-1)*eps;
				  j_local++;
				}
				//Нахождение обратной матрицы
				for (int j = 0; j < signals_length; j++)
				{
				  	alpha[j] = 0;
				  	for (int k = 0; k < signals_length; k++)
					{
			   			alpha[j] += (S1[i][k]-S[i][k])*T[k][j];
					}
				}
				double buf = 1 + alpha[i];
				for (int j = 0; j < signals_length; j++)
				{
					alpha[j] = -alpha[j]/buf;
				}
				for (int k = 0; k < signals_length; k++)
				{
					for (int j = 0; j < signals_length; j++)
					{
				  		T1[k][j] = T[k][j] + alpha[j]*T[k][i];
					}
				}
				
				//Применение преобразования к сигналам и подсчет среднего отклонения
				double delta1 = SignalConversion(signals, S1, T1, signals_number, signals_length);
				if (delta1 < delta)
				{
					best = q;
					Copy(Sbuf, S1, signals_length, signals_length);
					Copy(Tbuf, T1, signals_length, signals_length);
					delta = delta1;
                }   
			}
			//Сохранение наилучшего преобразования
			if (best > 0)
			{
				better = 1;
				Copy(S, Sbuf, signals_length, signals_length);
				Copy(T, Tbuf, signals_length, signals_length);
			}
		}
	}
	WriteMatrixToFile(S, "S.txt", signals_length, signals_length);
	WriteMatrixToFile(T, "T.txt", signals_length, signals_length);
	printf("Имя файла для записи результата: ");
	char file_name[80];
	scanf("%s", file_name);
	WriteResults(delta, deltaDFT, file_name);

	time = clock() - time;
	printf("It took me %d clicks (%f seconds).\n",(int)time,((float)time)/CLOCKS_PER_SEC);

	for (int i = 0; i < signals_length; i++)
	{
		free(S[i]);
		free(T[i]);
		free(S1[i]);
		free(T1[i]);
		free(Sbuf[i]);
		free(Tbuf[i]);
	}
	for (int i = 0; i < signals_number; i++)
	{
		free(signals[i]);
	}
	free(signals);
	free(S);
	free(T);
	free(S1);
	free(T1);
	free(Sbuf);
	free(Tbuf);
	free(alpha);
	return 0;
}
