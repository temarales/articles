#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include </home/temarales/intel/advisor_2019.3.0.591490/include/advisor-annotate.h>
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
}
void CalcForces1()
{
 for (int i = 0; i < N; i++)
   {
     ANNOTATE_SITE_BEGIN();
     for (int j = 0; j < N; j++)
       {
	 ANNOTATE_TASK_BEGIN();
	 if (i == j) continue;
	 double dx = p[j].x - p[i].x, dy = p[j].y - p[i].y,
	   r_2 = 1 / (dx * dx + dy * dy),
	   r_1 = sqrt(r_2),
	   fabs = gravity * m[i] * m[j] * r_2;
	 if (fabs > fmax) fabs = fmax;
	 f[i].x = f[i].x + fabs * dx * r_1;
	 f[i].y = f[i].y + fabs * dy * r_1;
	 ANNOTATE_TASK_END();
       }
     ANNOTATE_SITE_END();
   }
}
void MoveParticlesAndFreeForces()
{
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
 Init();
 //double t = omp_get_wtime();
 //ANNOTATE_SITE_BEGIN(allRows);
 for (int i = 0; i < Niter; i++)
 {
  // ANNOTATE_TASK_BEGIN(eachRow);
 CalcForces1();
 MoveParticlesAndFreeForces();
 //ANNOTATE_TASK_END(eachRow);
 }
 //ANNOTATE_SITE_END(allRows);
 //t = omp_get_wtime() - t;
}
