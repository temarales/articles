#ifndef COMPUTATION_FUNCTIONS_H
#define COMPUTATION_FUNCTION_H

#include <math.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

void ComputeDFT(double ** matrix, int signals_length);
void MatrixMultiply(double **A, double **B, int signals_length);
void MultiplyVectorByMatrix(double *res, double **A, double* B, int signals_length);
void Copy(double **a, double **b, int x, int y);
double CalculateDelta(double **signals, double **F1, int signals_number, int signals_length);
void Filter(double **G, int signals_number, int signals_length);
double SignalConversion(double **signals, double **S, double **T, int signals_number, int signals_length);
double* SignalConversionByOne(double **signals, double **S, double **T, int signals_number, int signals_length);

#endif /* COMPUTATION_FUNCTIONS_H */