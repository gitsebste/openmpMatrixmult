#include "stdafx.h"
#include <omp.h>
#include <cstdio>
//#include <assert.h>
#include < conio.h >
#include <iostream>
#include <ctime>


using namespace std;

#define mx 1000

double a[mx][mx];
double b[mx][mx];
double c[mx][mx];
double d[mx][mx];
double e[1][mx];
double f[1][mx];
double g[1][mx];
double h[1][mx];

//#pragma omp parallel for schedule(static) default(shared) private(i,j)
void randMat(int n)
{
	int i;
	int j;
	for (i = 0; i<n; i++)
	{
		for (j = 0; j<n; j++)
		{
			int v = rand() % 11;
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
		int v = rand()% 11;
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
			f[0][i] += a[i][j] * e[0][j];
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
			g[0][i] += a[i][j] * e[0][j];
	double en = omp_get_wtime();
	printf("Static Scheduler\t%7d\n", (int)((en - st) * 1000000));
}

void vecParralle2(int n)
{
	int i;
	int j;
	double st = omp_get_wtime();
#pragma omp parallel for schedule(dynamic) private(i,j)shared(a,f,e)
	for (i = 0; i<n; i++)
		for (j = 0; j<n; j++)
			h[0][i] += a[i][j] * e[0][j];
	double en = omp_get_wtime();
	printf("Dynamic Scheduler\t%7d\n", (int)((en - st) * 1000000));
}

void showAE(int n) {
	cout << "\n\n\nMAT_A | VEC_E" << endl;
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			printf("%2f\t", a[i][j]);
		}
		printf("\t\t\%f\n", e[0][i]);
	}
	printf("\n\n\n");
}
void showFGH(int n) {
	cout << "VEC_F | VEC_G | VEC_H" << endl;
	for (int i = 0; i < n; i++) {
		printf("%f\t%f\t%f\n", f[0][i], g[0][i], h[0][i]);
	}
}
void checkVec(int n) {
	bool ok = true;
	for (int i = 0; i<n; i++)
		if (!(f[0][i] == g[0][i] && f[0][i] == h[0][i])) {
			ok = false; break;
		}
	showAE(n);
	showFGH(n);
	if (ok)cout << "OK." << endl; else cout << "NOT OK." << endl;
}


int main(int argc, char *argv[])
{
	srand(time(NULL));
	int n = 3;
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
Serial:                       5
Static Scheduler           3279
Dynamic Scheduler            80
OK.

Vector
serial:                       7
Static Scheduler             12
Dynamic Scheduler            12



MAT_A | VEC_E
2.000000        6.000000        1.000000                        4.000000
2.000000        5.000000        5.000000                        4.000000
0.000000        7.000000        2.000000                        6.000000



VEC_F | VEC_G | VEC_H
38.000000       38.000000       38.000000
58.000000       58.000000       58.000000
40.000000       40.000000       40.000000
OK.

*/
