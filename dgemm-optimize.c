#include <stdlib.h>

/* void dgemm( int m, int n, float *A, float *C ) //Unrolling
{
    for( int i = 0; i < m; i++ )
    {
        for( int k = 0; k < n; k++ ) 
        {
            for( int j = 0; j < m; j+= 4) 
            {
                int bound = m - j; 

				if (bound > 3 ) 
				{
					C[i + j * m] += A[i + k * m] * A[j + k * m];
					C[i + (j + 1)*m] += A[i + k * m] * A[(j + 1) + k * m];
					C[i + (j + 2)*m] += A[i + k * m] * A[(j + 2) + k * m];
					C[i + (j + 3)*m] += A[i + k * m] * A[(j + 3) + k * m];
				}
				
				else  if (bound == 3) 
				{
					C[i + j * m] += A[i + k * m] * A[j + k * m];
					C[i + (j + 1)*m] += A[i + k * m] * A[(j + 1) + k * m];
					C[i + (j + 2)*m] += A[i + k * m] * A[(j + 2) + k * m];
				}

				else if (bound == 2)  
				{
					C[i + j * m] += A[i + k * m] * A[j + k * m];
					C[i + (j + 1)*m] += A[i + k * m] * A[(j + 1) + k * m];
				}

				else C[i + j * m] += A[i + k * m] * A[j + k * m];  


            }
        }
    } 
}/* */

/* */void dgemm( int m, int n, float *A, float *C ) //Pre-fetching
{
    for( int i = 0; i < n; i++ )
    {
        for( int k = 0; k < m; k++ ) 
        {
            float* constantAddress = &A[k + i * m];
            float* movingAddress = &A[i * m];
            for( int j = 0; j < m; j++) 
            {
                C[k+j*m]+= (*constantAddress)*(*movingAddress++); 
                //*movingAddress++; //prefetches the next address
            }
        }
    }
}/* */

/* void dgemm(int m, int n, float *A, float *C ) //reordering
{
	for (int i = 0; i < n; i++)
    {
		for (int j = 0; j < m; j++) 
        {
			for (int k = 0; k < m; k++) 
            {
			C[j + k * m] += A[j + i * m] * A[k + i * m];
			}
		}
	}
}
/**/




