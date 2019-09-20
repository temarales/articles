// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <time.h> 

#define eps 0.01
#define accuracy 1000
#define compressionLevel 0.8


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

void ReadMatrixFromFile(double **matrix, char *file_name, int x, int y)
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

void WriteMatrixToFile(double **matrix, char *file_name, int x, int y)
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

void WriteResults(const double delta, const double deltaDFT)
{
	//printf("Имя файла для сохранения результата: ");
	//scanf("%s", file_name);
	char file_name[80] = "res.txt";
	FILE *fw = fopen(file_name, "a");
	fprintf(fw, "Среднее отклонение при сжатии найденной матрицей: %f\n", delta);
	fprintf(fw, "Среднее отклонение при сжатии матрицей ДПФ: %f\n", deltaDFT);
	fclose(fw);
}

void ComputeDFT(double ** matrix, int signals_length)
{
	matrix[0][0] = 1/sqrt(2 * signals_length - 1);
 	for (int j = 1; j < signals_length; j++)
	{
		matrix[0][j] = 2/sqrt(2 * signals_length - 1);
	}
  	for (int i = 1; i < signals_length; i++)
	{
		matrix[i][0] = 1/sqrt(2 * signals_length - 1);
		for (int j = 1; j < signals_length; j++)
		{
			matrix[i][j] = 2 * cos(2 * acos(-1.0) * i * j/(2 * signals_length - 1))/sqrt(2 * signals_length - 1);
		}
	}
}

void matrix_multiply(double **A, double **B, int signals_length)
{
	double res[signals_length][signals_length];

	for (int i = 0; i < signals_length; i++)
		for (int j = 0; j < signals_length; j++)
			{
				res[i][j] = 0;
				for (int k = 0; k < signals_length; k++)
					res[i][j] += A[i][k]*B[k][j];
			}

	for (int i = 0; i < signals_length; i++)
	{
    	for (int j = 0; j < signals_length; j++)
			printf("%f ", res[i][j]);
		printf("\n");
	}
	
}

void multiply_vector_by_matrix(double *res, const double **A, const double* B, int signals_length)
{
  	for (int i = 0; i < signals_length; i++)
	{
		res[i] = 0;
		for (int j = 0; j < signals_length; j++)
		{	
			res[i] += A[i][j]*B[j];
		}
	}
}

void Copy(double **a, const double **b, int x, int y)
{
	for(int i = 0; i < x; i++)
	{
		for(int j = 0; j < y; j++)
		{
			a[i][j] = b[i][j];
		}
	}
}

double CalculateDelta(double **signals, double **F1, int signals_number, int signals_length)
{
	double delta1 = 0;
	double delta_buf = 0;
	for (int p = 0; p < signals_number; p++)
	{
		delta_buf = 0;
		for (int k = 0; k < signals_length; k++)
		{
			delta_buf += (signals[p][k]-F1[p][k])*(signals[p][k]-F1[p][k])/(signals_length * signals_number * signals_number);
		}
		delta1 += sqrt(delta_buf);
	}
	return delta1;
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

double SignalConversion(double **signals, const double **S, const double **T, int signals_number, int signals_length)
{
	double **G = (double **)malloc(signals_number * sizeof(double*));
	double **F1 = (double **)malloc(signals_number * sizeof(double*));
	for (int i = 0; i < signals_number; i++)
	{
		G[i] = (double *)malloc(signals_length * sizeof(double));
		F1[i] = (double *)malloc(signals_length * sizeof(double));
	}

	//Прямое преобразование сигналов
	for (int p = 0; p < signals_number; p++)
	{
		multiply_vector_by_matrix(G[p], T, signals[p], signals_length);
	}
	//WriteMatrixToFile(G, "G.txt", signals_number, signals_length);

	//Фильтрация
	Filter(G, signals_number, signals_length);
	
	//Обратное преобразование сигналов
	//WriteMatrixToFile(G, "G_null.txt", signals_number, signals_length);
	for (int p = 0; p < signals_number; p++)
	{
		multiply_vector_by_matrix(F1[p], S, G[p], signals_length);
	}
	//WriteMatrixToFile(F1, "F1.txt", signals_number, signals_length);

	//Расчет среднего отклонения
	double delta1 = CalculateDelta(signals, F1, signals_number, signals_length);
	for (int i = 0; i < signals_number; i++)
	{
		free(G[i]);	
		free(F1[i]);
	}
	free(G);
	free(F1);
    return delta1;
}

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
	FILE *fw = fopen("delta_dynamic.txt", "a");
	
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
				//WriteResults(delta1, deltaDFT);
				if (delta1 < delta)
				{
					best = q;
					Copy(Sbuf, S1, signals_length, signals_length);
					Copy(Tbuf, T1, signals_length, signals_length);
					delta = delta1;
					fprintf(fw, "%f\n", delta);
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
	fclose(fw);
	WriteMatrixToFile(S, "S.txt", signals_length, signals_length);
	WriteMatrixToFile(T, "T.txt", signals_length, signals_length);
	WriteResults(delta, deltaDFT);

	time = clock() - time;
  	printf ("It took me %d clicks (%f seconds).\n",(int)time,((float)time)/CLOCKS_PER_SEC);

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
