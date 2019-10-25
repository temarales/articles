#ifndef FILE_H
#define FILE_H

#include <stdio.h>
#include <string.h>

void ReadData(double **signals, int signals_number, int signals_length);
void ReadMatrixFromFile(double **matrix, char *file_name, int x, int y);
void WriteMatrixToFile(double **matrix, char *file_name, int x, int y);
void WriteResults(const double delta, const double deltaDFT, char *file_name);
void WriteResultsByOne(double* delta, double* deltaDFT, int counter, int signals_number, int signals_length);

#endif /* FILE_H */