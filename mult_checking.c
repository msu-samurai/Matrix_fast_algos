#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

//#include "rotation.h"
#include "output.h"
#include "initialize.h"
#include "output.c"
#include "initialize.c"
//#include "multiplication.h"

void multi_faster (double * a, double * b, double * c, int n, int m);
void multi_faster (double * a, double * b, double * c, int n, int m)
{

	/*
	pa указывает на блок av*ah, pb указывает на bv*bh
	*/

	int i, j , s , t , r , q;
	int k , l;
	int last;
	int av, bv, ah, bh, av2, bh2;
	double sum;
	double c00, c01, c10, c11;
	double *pa, *pb, *pc;
	k = n/m;
	l = n - k * m;
	last = (l > 0 ? k + 1 : k);
	for (i = 0; i < last; i++)
	{
		av = (i < k ? m : l);
		av2 = (av & (~1));
		//bh = av;
		for (j = 0; j < last; j ++)
		{

			bh = (j < k ? av : l);
			bh2 = (bh & (~1));
			pc = c + (i * n + j)*m;
			for(r = 0; r < av; r ++)
			{
				for(t = 0; t < bh; t ++)
				{
					pc [ r*n + t ]= 0;
				}
			}
			for (s = 0; s < last; s ++)
			{
				ah = (s < k ? m : l);
				bv = ah;
				pa = i * n * m + s*m + a;
				pb = s * n * m + j * m + b;
				//printf("i=%d, j=%d,s=%d\n",i,j,s );
				//printf("ah = %d, av=%d, bh=%d, bv=%d\n", ah, av, bh, bv);

				for (r = 0; r < av2; r += 2)
					for (t = 0; t < bh2; t += 2)
					{
						c00 = 0;
						c01 = 0;
						c10 = 0;
						c11 = 0;
						sum = 0; 
						for ( q = 0 ; q < ah ; q ++)
						{
							c00 += pa[r*n + q] * pb[q*n + t];
							c01 += pa[r*n + q] * pb[q*n + t+1];
							c10 += pa[(r+1)*n + q] * pb[q*n + t];
							c11 += pa[(r+1)*n + q] * pb[q*n + t+1];

						}

						pc[r*n + t] += c00;
						pc[r*n + t+1] += c01;
						pc[(r+1)*n + t] += c10;
						pc[(r+1)*n + (t+1)] += c11;

					}

				if (av2 < av)
				{
					
					for (t = 0; t < bh; t ++)
					{
						sum = 0;
						for (q = 0; q < ah; q ++)
							sum += pa[av*n + q] * pb[q*n + t];
					}
					pc[av*n + t] += sum;
				}
				if (bh2 < bh)
				{
					
					for (r = 0; r < av; r ++)
					{
						sum = 0;
						for (q = 0; q < ah; q ++)
							sum += pa[r*n + q] * pb[q*n + bh];
					}
					pc[r*n + bh] += sum;
				}

					//printf("%lf ", sum);
					
					

			}
		}
	}
}


void prod (double * a, double * b, double * c, int dim);
void prod (double * a, double * b, double * c, int dim)
{
//double * product = (double *)malloc(dim * dim * sizeof(double));
	for (int i = 0; i < dim; i ++)
	{
		for (int j = 0 ; j < dim; j ++)
		{
			c[j + i * dim] = 0;
			for (int k = 0; k < dim; k ++)
				c[j + i * dim] += a[k + i * dim] * b[j + k * dim];
			
		}
	}
}




int main (int argc, char **argv)
{
	//size of matr, output limit, type of matr initializion
	int dim = atoi(argv[1]), output_limit = atoi(argv[2]), input_type = atoi(argv[3]);

	//to check time of execution
	clock_t begin = clock();

	//given
	double *matr = (double *)malloc(dim * dim * sizeof(double));
	
	//for results
	//double *result_matr;

	//for mult
	double *mult;

	//check
	if (matr == NULL)
		return -1;


	//initializing matrix
	if(input_type <= 0)
	{
		if(initialize_matr_from_file(matr, dim, argv[4]) < 0)
			{
				printf("bad file\n");
				
				free(matr);
				return -1;
			}
	}
	else
		initialize_matr_by_func(matr, dim, input_type);

	//initialize result matr
	
	mult = (double *)malloc(dim * dim * sizeof(double));
	if (mult == NULL)
	{
		free(matr);
		return -1;
	}

	//print matr
	printf("given matr\n");
	output(dim, dim, output_limit, matr);
	printf("===\n");


	//matr^2
	multi_faster(matr, matr, mult, dim, 51);
	//prod(matr, matr, mult, dim);
	printf("matr^2\n");
	output(dim, dim, output_limit, mult);
	printf("====\n");

	//time of matrix mult
	execution_time(begin);


	


	
	printf("\n====\n");

	free(matr);
	free(mult);
	return 0;
}