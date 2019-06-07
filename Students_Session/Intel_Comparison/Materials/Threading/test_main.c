#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>
#include <omp.h>
#define Nthr 2 
#define gravity 10 // гравитационная постоянная
#define dt 0.1 // шаг по времени
#define N 800 // количество частиц
#define fmax 1 // максимальное значение силы
#define Niter 100 // число итераций
typedef struct Particle
{
 double x, y, vx, vy;
} Particle;
typedef struct Force
{
 double x, y;
} Force;
Particle p[N];
Force f[N];
double m[N];
Force tf[N][Nthr];
void Init()
{
 for (int i = 0; i < N; i++)
 {
 p[i].x = 20 * (i / 20 - 20) + 10;
 p[i].y = 20 * (i % 20 - 10) + 10;
 p[i].vx = p[i].y / 15;
 p[i].vy = -p[i].x / 50;
 m[i] = 100 + i % 100;
 f[i].x = 0;
 f[i].y = 0;
 }
 for (int i = 0; i < N; i++)
 for (int j = 0; j < Nthr; j++)
 {
 tf[i][j].x = 0;
 tf[i][j].y = 0;
 }
}
void CalcForces2ParB()
{
#pragma omp parallel for num_threads(Nthr)
 for (int i = 0; i < N - 1; i++)
 {
 int k = omp_get_thread_num();
 for (int j = i + 1; j < N; j++)
 {
 double dx = p[j].x - p[i].x, dy = p[j].y - p[i].y,
 r_2 = 1 / (dx * dx + dy * dy),
 r_1 = sqrt(r_2),
 fabs = gravity * m[i] * m[j] * r_2;
 if (fabs > fmax) fabs = fmax;
 tf[i][k].x += dx = fabs * dx * r_1;
 tf[i][k].y += dy = fabs * dy * r_1;
 tf[j][k].x -= dx;
 tf[j][k].y -= dy;
 }
 }
#pragma omp parallel for num_threads(Nthr)
 for (int i = 0; i < N; i++)
 for (int j = 0; j < Nthr; j++)
 {
 f[i].x += tf[i][j].x;
 f[i].y += tf[i][j].y;
 tf[i][j].x = 0;
 tf[i][j].y = 0;
 }
}
void MoveParticlesAndFreeForcesPar()
{
#pragma omp parallel for num_threads(Nthr)
 for (int i = 0; i < N; i++)
 {
 double dvx = f[i].x * dt / m[i],
 dvy = f[i].y * dt / m[i];
 p[i].x += (p[i].vx + dvx / 2) * dt;
 p[i].y += (p[i].vy + dvy / 2) * dt;
 p[i].vx += dvx;
 p[i].vy += dvy;
 f[i].x = 0;
 f[i].y = 0;
 }
}

void main()
{
double start_time = omp_get_wtime();
 Init();
 for (int i = 0; i < Niter; i++)
 {
 CalcForces2ParB();
 MoveParticlesAndFreeForcesPar();
 }
double end_time = omp_get_wtime();
printf("%f", end_time-start_time);
}
