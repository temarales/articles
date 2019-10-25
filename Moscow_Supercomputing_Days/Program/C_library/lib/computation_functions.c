#include "computation_functions.h"

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

void MatrixMultiply(double **A, double **B, int signals_length)
{
	double res[signals_length][signals_length];

	for (int i = 0; i < signals_length; i++)
		for (int j = 0; j < signals_length; j++)
			{
				res[i][j] = 0;
				for (int k = 0; k < signals_length; k++)
					res[i][j] += A[i][k]*B[k][j];
			}
}

void MultiplyVectorByMatrix(double *res, double **A, double* B, int signals_length)
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

void Copy(double **a, double **b, int x, int y)
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

double SignalConversion(double **signals, double **S, double **T, int signals_number, int signals_length)
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
		MultiplyVectorByMatrix(G[p], T, signals[p], signals_length);
	}

	//Фильтрация
	Filter(G, signals_number, signals_length);
	
	//Обратное преобразование сигналов
	for (int p = 0; p < signals_number; p++)
	{
		MultiplyVectorByMatrix(F1[p], S, G[p], signals_length);
	}

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

double* SignalConversionByOne(double **signals, double **S, double **T, int signals_number, int signals_length)
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
		MultiplyVectorByMatrix(G[p], T, signals[p], signals_length);
	}

	//Фильтрация
	Filter(G, signals_number, signals_length);

	//Обратное преобразование сигналов
	for (int p = 0; p < signals_number; p++)
	{
		MultiplyVectorByMatrix(F1[p], S, G[p], signals_length);
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
