#include "stdafx.h"
#include <omp.h>
#include <cstdio>
//#include <assert.h>
#include < conio.h >
#include <iostream>


using namespace std;

#define mx 1000

int a[mx][mx];
int b[mx][mx];
int c[mx][mx];
int d[mx][mx];
int e[1][mx];
int f[1][mx];
int g[1][mx];
int h[1][mx];

//#pragma omp parallel for schedule(static) default(shared) private(i,j)
void randMat(int n)
{
	int i;
	int j;
	for (i = 0; i<n; i++)
	{
		for (j = 0; j<n; j++)
		{
			int v = (i * 2 + j) % 100;
			a[i][j] = v;
			b[i][j] = v;

		}
	}
}
//#pragma omp parallel for schedule(static) default(shared) private(i)
void randVec(int n)
{
	int i;
	for (i = 0; i<n; i++)
	{
		int v = (i * 2 + 1) % 100;
		e[0][i] = v;
	}
}


bool check(int n)
{
	int i;
	int j;
	bool ok = true;
	for (i = 0; i<n; i++)
	{
		for (j = 0; j<n; j++)
			if (c[i][j] != d[i][j]) {
				ok = false; break;
			}
	}
	return ok;
}

void matSerial(int n)
{
	int i;
	int j;
	int k;
	double st = omp_get_wtime();
	for (i = 0; i<n; i++)
	{
		for (j = 0; j<n; j++)
		{
			for (k = 0; k<n; k++)
			{
				c[i][j] += a[i][k] * b[k][j];
			}
		}
	}
	double en = omp_get_wtime();
	printf("Serial:\t\t\t%7d\n", (int)((en - st) * 1000000));
}
bool matParallel1(int n)
{
	memset(d, 0, sizeof d);
	int i;
	int j;
	int k;
	double st = omp_get_wtime();
#pragma omp parallel for schedule(static) private(i,j,k)shared(a,b,c)
	for (i = 0; i<n; i++)
		for (j = 0; j<n; j++)
			for (k = 0; k<n; k++)
				d[i][j] += a[i][k] * b[k][j];

	double en = omp_get_wtime();
	printf("Static Scheduler\t%7d\n", (int)((en - st) * 1000000));
	return check(n);
}
bool matParallel2(int n)
{
	//Dynamic Scheduler
	memset(d, 0, sizeof d);
	int i;
	int j;
	int k;
	double st = omp_get_wtime();
	//#pragma omp parallel for schedule(dynamic,50) collapse(2) private(i,j,k) shared(a,b,c)
#pragma omp parallel for schedule(dynamic) private(i,j,k) shared(a,b,c)
	for (i = 0; i<n; i++)
		for (j = 0; j<n; j++)
			for (k = 0; k<n; k++)
				d[i][j] += a[i][k] * b[k][j];
	double en = omp_get_wtime();
	printf("Dynamic Scheduler\t%7d\n", (int)((en - st) * 1000000));
	return check(n);
}

void vec(int n)
{
	int i;
	int j;
	double st = omp_get_wtime();
	for (i = 0; i<n; i++)
		for (j = 0; j<n; j++)
			f[0][i] += a[i][j] * e[0][i];
	double en = omp_get_wtime();
	printf("serial:\t\t\t%7d\n", (int)((en - st) * 1000000));
}

void vecParralle1(int n)
{
	int i;
	int j;
	double st = omp_get_wtime();
#pragma omp parallel for schedule(static) private(i,j)shared(a,f,e)
	for (i = 0; i<n; i++)
		for (j = 0; j<n; j++)
			g[0][i] += a[i][j] * e[0][i];
	double en = omp_get_wtime();
	printf("Static Scheduler\t%7d\n", (int)((en - st) * 1000000));
}

void vecParralle2(int n)
{
	int i;
	int j;
	double st = omp_get_wtime();
#pragma omp parallel for schedule(dynamic,n/50) private(i,j)shared(a,f,e)
	for (i = 0; i<n; i++)
		for (j = 0; j<n; j++)
			h[0][i] += a[i][j] * e[0][i];
	double en = omp_get_wtime();
	printf("Dynamic Scheduler\t%7d\n", (int)((en - st) * 1000000));
}

void checkVec(int n) {
	bool ok = true;
	for (int i = 0; i<n; i++)
		if (!(f[0][i] == g[0][i] && f[0][i] == h[0][i])) {
			ok = false; break;
		}
	if (ok)cout << "OK." << endl; else cout << "NOT OK." << endl;
}

int main(int argc, char *argv[])
{
	int n = 100;
	randMat(n);
	cout << "Matrix" << endl;
	matSerial(n);
	bool matrixOk = matParallel1(n) ? matParallel2(n) : false;
	if (matrixOk)cout << "OK." << endl; else cout << "NOT OK." << endl;
	cout << endl << "Vector" << endl;
	randVec(n);
	vec(n);
	vecParralle1(n);
	vecParralle2(n);
	checkVec(n);
	scanf_s("%d", &n);//lock screen
	return 0;
}
/*
Matrix
Serial:                 8564644
Static Scheduler        3959169
Dynamic Scheduler       3967427
OK.

Vector
serial:                    4219
Static Scheduler           1972
Dynamic Scheduler          2054
OK.
*/











































































































//#include "stdafx.h"
//#include <omp.h>
//#include <cstdio>
//#include <assert.h>
//
//
//using namespace std;
//
//#define mx 1002
//
//int a[mx][mx];
//int b[mx][mx];
//int c[mx][mx];
//int d[mx][mx];
//void generate_matrix(int n)
//{
//	int i, j;
//	for (i = 0; i<n; i++)
//	{
//		for (j = 0; j<n; j++)
//		{
//			int v = (i * 2 + j) % 100;
//			a[i][j] = v;
//			b[i][j] = v;
//
//		}
//	}
//}
//void check(int n)
//{
//	int i, j;
//	for (i = 0; i<n; i++)
//	{
//		for (j = 0; j<n; j++)
//			assert(c[i][j] == d[i][j]);
//	}
//}
//void matrix_mult_serial(int n)
//{
//	int i, j, k;
//	double st = omp_get_wtime();
//	for (i = 0; i<n; i++)
//	{
//		for (j = 0; j<n; j++)
//		{
//			for (k = 0; k<n; k++)
//			{
//				c[i][j] += a[i][k] * b[k][j];
//			}
//		}
//	}
//	double en = omp_get_wtime();
//	printf("Serial: %lf\n", en - st);
//}
//void matrix_mult_parallel1(int n)
//{
//	//Static Scheduler
//	memset(d, 0, sizeof d);
//	int i, j, k;
//	double st = omp_get_wtime();
////#pragma omp parallel for schedule(static,50) collapse(2) private(i,j,k)shared(a,b,c)
//#pragma omp parallel for schedule(static) private(i,j,k)shared(a,b,c)
//	for (i = 0; i<n; i++)for (j = 0; j<n; j++)for (k = 0; k<n; k++)d[i][j] += a[i][k] * b[k][j];
//	double en = omp_get_wtime();
//	printf("Parallel-1(Static Scheduler) %lf\n", en - st);
//	check(n);
//}
//void matrix_mult_parallel2(int n)
//{
//	//Dynamic Scheduler
//	memset(d, 0, sizeof d);
//	int i, j, k;
//	double st = omp_get_wtime();
////#pragma omp parallel for schedule(dynamic,50) collapse(2) private(i,j,k) shared(a,b,c)
//#pragma omp parallel for schedule(dynamic,50) private(i,j,k) shared(a,b,c)
//	for (i = 0; i<n; i++)for (j = 0; j<n; j++)for (k = 0; k<n; k++)d[i][j] += a[i][k] * b[k][j];
//	double en = omp_get_wtime();
//	printf("Parallel-2(Dynamic Scheduler) %lf\n", en - st);
//	check(n);
//}
//
//
//int main(int argc, char *argv[])
//{
//	int n = 500;
//	generate_matrix(n);
//	matrix_mult_serial(n);
//
//	matrix_mult_parallel1(n);
//
//	matrix_mult_parallel2(n);
//	scanf_s("%d", &n);//lock screen
//	return 0;
//}