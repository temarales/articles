#include "file.h"

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

void WriteResults(const double delta, const double deltaDFT, char *file_name)
{
	FILE *fw = fopen(file_name, "a");
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