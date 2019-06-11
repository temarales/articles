#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

#define N 10
#define M 50
#define accuracy 1000
#define compressionLevel 0.8


void ReadData(double **signals)
{
	printf("Имя файла с данными о сигналах: ");
	char *file_name = (char*)malloc(20*sizeof(char));
	scanf("%s", file_name);
	FILE *f = fopen(file_name, "r");
	for (int i = 0; i < M; i++)
	{
		for(int j = 0; j < N; j++)
			fscanf(f,"%lf ", &signals[i][j]);
		fscanf(f,"\n");
	}	
	fclose(f);
	free(file_name);
}

void ReadMatrixFromFile(double ** matrix, char *file_name)
{
	FILE *fr = fopen(file_name, "r");
	for (int i = 0; i < N; i++)
	{
		for(int j = 0; j < N; j++)
			fscanf(fr, "%lf ", &matrix[i][j]);
		fscanf(fr, "\n");
	}	
	fclose(fr);
	//free(file_name);
}

void WriteMatrixToFile(double ** matrix, char *file_name)
{
	FILE *fw = fopen(file_name, "w");
	for (int i = 0; i < N; i++)
	{
    		for (int j = 0; j < N; j++)
			fprintf(fw, "%lf ", matrix[i][j]);
		fprintf(fw, "\n");
	}
	fclose(fw);
}

void WriteResults(double delta, double deltaDFT)
{
	printf("Имя файла для сохранения результата: ");
	char *file_name = (char*)malloc(20*sizeof(char));
	scanf("%s", file_name);
	FILE *fw = fopen(file_name, "w");
	fprintf(fw, "Среднее отклонение при сжатии найденной матрицей: %lf\n", delta);
	fprintf(fw, "Среднее отклонение при сжатии матрицей ДПФ: %lf\n", deltaDFT);
	fclose(fw);
}

void ComputeDFT(double ** matrix)
{
	matrix[0][0] = 1/sqrt(2*N-1);
 	for (int j = 1; j < N; j++)
    		matrix[0][j] = 2/sqrt(2*N-1);
  	for (int i = 1; i < N; i++)
	{
		matrix[i][0] = 1/sqrt(2*N-1);
      		for (int j = 1; j < N; j++)
        		matrix[i][j] = 2*cos(2*acos(-1.0)*i*j/(2*N-1))/sqrt(2*N-1);
	}
	printf("Сохранить матрицу ДПФ в файл? (y/n) ");
	char answer;
	scanf("%s", &answer);
	if(answer == 'y')
		WriteMatrixToFile(matrix, "DFT.txt");
}

void multiply(double *res, double **A, double* B)
{
  	for (int i = 0; i < N; i++)
	{
	      res[i] = 0;
	      for (int j = 0; j < N; j++)
		res[i] += A[i][j]*B[j];
	}
}

void BubbleSort(double* a)
{
	for(int i = 0; i < N; i++)
		for( int j = 0; j < N-i; j++)
			if(a[j] > a[j+1])
			{
				double b = a[j];
				a[j] = a[j+1];
				a[j+1] = b;
			}		
}

double SignalConversion(double **signals, double **S, double **T)
{
	double **G = (double **)malloc(M * sizeof(double));
	for (int i = 0; i < M; i++)
		G[i] = (double *)malloc(N * sizeof(double));
	double **F1 = (double **)malloc(M * sizeof(double));
	for (int i = 0; i < M; i++)
		F1[i] = (double *)malloc(N * sizeof(double));
	int *index = (int *)malloc(N * sizeof(int));

	for (int p = 0;  p < M; p++)
               multiply(G[p], T, signals[p]);
	//Сжатие
	/*for (int p = 0;  p < M; p++)
	{
		BubbleSort(G[p]);
		int n = N*compressionLevel;
		for(int i = 0; i < n; i++)
			G[p][i] = 0;
	}*/
	for (int p = 0;  p < M; p++)
	{
		for (int k = 0; k < N; k++)
		{
			index[k] = 1;
			for (int k1 = 0; k < N; k++)
				if (abs(G[p][k]) < abs(G[p][k1]))
			  		index[k]++;
		}
		for (int k = 0; k < N; k++)
			if (index[k] > 2)
				G[p][k] = 0;
	}
	//Обратное преобразование сигналов
	for (int p = 0;  p < M; p++)
               multiply(F1[p], S, G[p]);
	//Расчет среднего отклонения
	double delta1 = 0;
	for (int p = 0;  p < M; p++)
		for (int k = 0; k < N; k++)
			delta1 += (signals[p][k]-F1[p][k])*(signals[p][k]-F1[p][k])/M*M*N*N;
	for (int i = 0; i < M; i++)
	{
		free(G[i]);	
		free(F1[i]);
	}
	free(G);	
	free(index);
	free(F1);
        return sqrt(delta1);
}

int main(int argc, char ** argv) {
	double **signals = (double **)malloc(M * sizeof(double));
	for (int i = 0; i < M; i++)
		signals[i] = (double *)malloc(N * sizeof(double));
	ReadData(signals);

	double **S = (double **)malloc(N * sizeof(double));
	for (int i = 0; i < N; i++)
		S[i] = (double *)malloc(N * sizeof(double));
	double **T = (double **)malloc(N * sizeof(double));
	for (int i = 0; i < N; i++)
		T[i] = (double *)malloc(N * sizeof(double));
	double **DFT = (double **)malloc(N * sizeof(double));
	for (int i = 0; i < N; i++)
		DFT[i] = (double *)malloc(N * sizeof(double));

	printf("Имя файла с данными о матрице ДПФ: ");
	char *file_name = (char*)malloc(20*sizeof(char));
	scanf("%s", file_name);
	ReadMatrixFromFile(DFT, file_name);
	printf("Имя файла с данными о найденной матрице S: ");
	scanf("%s", file_name);
	ReadMatrixFromFile(S, file_name);
	printf("Имя файла с данными о найденной матрице T: ");
	scanf("%s", file_name);
	ReadMatrixFromFile(T, file_name);
	double delta = SignalConversion(signals, S, T);
	double deltaDFT = SignalConversion(signals, DFT, DFT);
	WriteResults(delta, deltaDFT);

	free(signals);
	free(S);
	free(T);
	free(DFT);
	free(file_name);
	return 0;
}
