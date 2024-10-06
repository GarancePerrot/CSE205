/* 
README:
Garance Perrot, garance.perrot@polytechnique.fr
During this td, we use an important assumption about the reduction of cache misses:
In C, arrays are stored in row-major order by default, so the elements in the same 
row will be stored sequentially (close in memory). So iterating over rows induces a 
better locality than iterating over rows, so reduces the number of cache misses.  





 * trans.c - Matrix transpose B = A^T
 *
 * Each transpose function must have a prototype of the form:
 * void trans(int M, int N, int A[N][M], int B[M][N]);
 *
 * A transpose function is evaluated by counting the number of misses
 * on a 1KB direct mapped cache with a block size of 32 bytes.
 */ 
#include <stdio.h>
#include "cachelab.h"

int is_transpose(int M, int N, int A[N][M], int B[M][N]);

/* 
 * transpose_submit - This is the solution transpose function that you
 *     will be graded on for Part B of the assignment. Do not change
 *     the description string "Transpose submission", as the driver
 *     searches for that string to identify the transpose function to
 *     be graded. 
 */


/* 32x32: 287 misses
   64x64: 1179 misses
   61x67: 1993 misses
*/
char transpose_submit_desc[] = "Transpose submission";
void transpose_submit(int M, int N, int A[N][M], int B[M][N])
{ //we use a different transposition method for each size of A

    if (M==32 && N==32) { //using transpose_block8_irows():
        
        int i,j,k,a0,a1,a2,a3,a4,a5,a6,a7; //11 variables

            for(i=0; i<N; i+=8) //split into 8x8 submatrices
                for(j=0; j<M; j+=8)
                    for(k=i; k<i+8; k++) //iterate over rows in each submatrix
                    {
                        // store each element of the row in a variable
                        a0 = A[k][j];
                        a1 = A[k][j+1];
                        a2 = A[k][j+2];
                        a3 = A[k][j+3];
                        a4 = A[k][j+4];
                        a5 = A[k][j+5];
                        a6 = A[k][j+6];
                        a7 = A[k][j+7];

                        // update the column k of B
                        B[j][k] = a0;
                        B[j+1][k] = a1;
                        B[j+2][k] = a2;
                        B[j+3][k] = a3;
                        B[j+4][k] = a4;
                        B[j+5][k] = a5;
                        B[j+6][k] = a6;
                        B[j+7][k] = a7;         
                    }
    }

    else if (M==64 && N==64) { //using transpose_block8_irows_half(), (see explanation below)

        int i,j,k,a0,a1,a2,a3,a4,a5,a6,a7; //11 variables

        for(i=0; i<N; i+=8) //split into 8x8 submatrices
        for(j=0; j<M; j+=8)
         {
                
                for (k = i; k < i + 4; k++) //upper half of S: 1st step
				{   //store each element in a variable
					a0 = A[k][j];
                    a1 = A[k][j+1]; 
                    a2 = A[k][j+2]; 
                    a3 = A[k][j+3];
					a4 = A[k][j+4]; 
                    a5 = A[k][j+5]; 
                    a6 = A[k][j+6]; 
                    a7 = A[k][j+7];
					
                    //temporarily update J1
					B[j][k] = a0; 
                    B[j+1][k] = a1; 
                    B[j+2][k] = a2; 
                    B[j+3][k] = a3;
					B[j][k+4] = a4; 
                    B[j+1][k+4] = a5; 
                    B[j+2][k+4] = a6; 
                    B[j+3][k+4] = a7;
				}
				for (k = j; k < j + 4; k++) //2nd step
				{
                    //store the values of S3 and (S2)^T
					a0 = A[i+4][k]; 
                    a1 = A[i+5][k]; 
                    a2 = A[i+6][k]; 
                    a3 = A[i+7][k];
					a4 = B[k][i+4]; 
                    a5 = B[k][i+5]; 
                    a6 = B[k][i+6]; 
                    a7 = B[k][i+7];
					
                    //update correctly J2
					B[k][i+4] = a0; 
                    B[k][i+5] = a1; 
                    B[k][i+6] = a2; 
                    B[k][i+7] = a3;
					B[k+4][i] = a4; 
                    B[k+4][i+1] = a5; 
                    B[k+4][i+2] = a6; 
                    B[k+4][i+3] = a7;
				}
				for (k= i + 4; k < i + 8; k++) //3rd step
				{  //store the values of S4 and complete J3
					a0 = A[k][j+4]; 
                    a1 = A[k][j+5]; 
                    a2 = A[k][j+6]; 
                    a3 = A[k][j+7];
					B[j+4][k] = a0; 
                    B[j+5][k] = a1; 
                    B[j+6][k] = a2; 
                    B[j+7][k] = a3;
				}        
         }
    }


    else if (M==61 && N==67) { //using transpose_block16_irows_random():
        
        int i,j,k,a0,a1,a2,a3,a4,a5,a6,a7;//11 variables

        for(i=0; i<N-N%16; i+=16) //divide the matrix into submatrices of 16x16 
            for(j=0; j<M-M%16; j+=16) // N-N%16 and M-M%16 multiples of 16 
                for( k=i; k<i+16; k++) //iterate over rows
                {
                    // iterate over row k
                    a0 = A[k][j];
                    a1 = A[k][j+1];
                    a2 = A[k][j+2];
                    a3 = A[k][j+3];
                    a4 = A[k][j+4];
                    a5 = A[k][j+5];
                    a6 = A[k][j+6];
                    a7 = A[k][j+7];

                    // store in the transpose
                    B[j][k] = a0;
                    B[j+1][k] = a1;
                    B[j+2][k] = a2;
                    B[j+3][k] = a3;
                    B[j+4][k] = a4;
                    B[j+5][k] = a5;
                    B[j+6][k] = a6;
                    B[j+7][k] = a7;  

                    //finish iterating over row k
                    a0 = A[k][j+8];
                    a1 = A[k][j+9];
                    a2 = A[k][j+10];
                    a3 = A[k][j+11];
                    a4 = A[k][j+12];
                    a5 = A[k][j+13];
                    a6 = A[k][j+14];
                    a7 = A[k][j+15];

                    // strore in transpose
                    B[j+8][k] = a0;
                    B[j+9][k] = a1;
                    B[j+10][k] = a2;
                    B[j+11][k] = a3;
                    B[j+12][k] = a4;
                    B[j+13][k] = a5;
                    B[j+14][k] = a6;
                    B[j+15][k] = a7;

                }


        
        // deal with the rest of the matrix (if N%16 and/or M%16 !=0)
        for (i = 0; i < N; i++) //fill the remainder at the right
            for (j = M-M%16; j < M; j++)
                {
                    a0 = A[i][j];
                    B[j][i] = a0;
                }
        for (i = N-N%16; i < N; i++) //fill the remainder at the bottom
            for (j = 0; j < M; j++)
                {
                    a0 = A[i][j];
                    B[j][i] = a0;
                }
    }
}


/* 
 * You can define additional transpose functions below. We've defined
 * a simple one below to help you get started. 
 */ 

/* 32x32:487 misses
*/
char block4_desc[] = "Block by 4 transpose"; 
void transpose_block4(int M, int N, int A[N][M], int B[M][N])
{//using 4 variables
int i,j,k,m;
    for( i=0; i<N; i+=4)  //first two lines : split into 4x4 submatrices 
        for( j=0; j<M; j+=4)
            for( k=i; k<i+4; k++) //now iterate over the submatrices
                for(m=j; m<j+4; m++)
                    B[m][k] = A[k][m];
}

/* 32x32:439 misses
*/
char block4_irows_desc[] = "Block by 4: iterate over rows transpose"; 
void transpose_block4_irows(int M, int N, int A[N][M], int B[M][N])
{   // using 7 variables
    int i,j,k,a0,a1,a2,a3;
    for(i=0; i<N; i+=4) //split into 4x4 submatrices
        for(j=0; j<M; j+=4)
            for(k=i; k<i+4; k++) //iterate only over rows 
            {
                //store the elements of each column in a variable
                a0 = A[k][j];    
                a1 = A[k][j+1];
                a2 = A[k][j+2];
                a3 = A[k][j+3];
                
                //update the transpose matrix
                B[j][k] = a0;
                B[j+1][k] = a1;
                B[j+2][k] = a2;
                B[j+3][k] = a3;     
            }
}

/* 32x32: 427 misses
*/
char block4_i2rows_desc[] = "Block by 4: iterate over rows 2 by 2 transpose"; 
void transpose_block4_i2rows(int M, int N, int A[N][M], int B[M][N])
{   // using 11 variables
    int i,j,k,a0,a1,a2,a3, a4,a5,a6,a7;
    for(i=0; i<N; i+=4)  //split into 4x4 submatrices
        for(j=0; j<M; j+=4)
            for(k=i; k<i+4; k+=2) //iterate over rows 2 by 2
            {
                //store the elements of each column in a variable
                a0 = A[k][j];
                a1 = A[k][j+1];
                a2 = A[k][j+2];
                a3 = A[k][j+3];
                
                // store the elements of row k+1
                a4 = A[k+1][j];
                a5 = A[k+1][j+1];
                a6 = A[k+1][j+2];
                a7 = A[k+1][j+3];

                 // update column k of B 
                B[j][k] = a0;
                B[j+1][k] = a1;
                B[j+2][k] = a2;
                B[j+3][k] = a3;

                //update column k+1 of B  
                B[j][k+1] = a4;
                B[j+1][k+1] = a5;
                B[j+2][k+1] = a6;
                B[j+3][k+1] = a7;
            }
}

/* 32x32:287, winner!
*/
char block8_irows_desc[] = "Block by 8: iterate over rows transpose";
void transpose_block8_irows(int M, int N, int A[N][M], int B[M][N])
{   // using 11 variables
    int i,j,k,a0,a1,a2,a3,a4,a5,a6,a7;
    for(i=0; i<N; i+=8) //split into 8x8 submatrices
        for(j=0; j<M; j+=8)
            for(k=i; k<i+8; k++) //iterate over rows in each submatrix
            {
                // store each element of the row in a variable
                a0 = A[k][j];
                a1 = A[k][j+1];
                a2 = A[k][j+2];
                a3 = A[k][j+3];
                a4 = A[k][j+4];
                a5 = A[k][j+5];
                a6 = A[k][j+6];
                a7 = A[k][j+7];

                // update the column k of B
                B[j][k] = a0;
                B[j+1][k] = a1;
                B[j+2][k] = a2;
                B[j+3][k] = a3;
                B[j+4][k] = a4;
                B[j+5][k] = a5;
                B[j+6][k] = a6;
                B[j+7][k] = a7;         
            }
}


/* 32x32:319 misses
   64x64: 1179 misses, winner!
*/
char block8_irows_half_desc[] = "Block by 8: iterate over rows by half transpose"; 
void transpose_block8_irows_half(int M, int N, int A[N][M], int B[M][N])
{   // using 11 variables
    int i,j,k,a0,a1,a2,a3,a4,a5,a6,a7;

/* Explanation:

Let S be a 8x8 submatrix of A. Split S into 4 4x4 submatrices that we call S1,S2,S3,S4. 

S = | S1  S2 |   
    | S3  S4 |    

The goal here is to create a matrix J of size 8x8 which is the transpose of S. 

1st step: Iterate over rows for S1 and S2, store the values in some variables and complete 
J1 over columns as follows:

J1 = | (S1)^T (S2)^T |
     |               |

Notice that (S2)^T is not at the right position yet but we used the fact that dealing with rows 
(for both iterating over S and completing J) increases the locality (see intro README) 

2nd step: Iterate over columns for S3 and over rows for (S2)^T to transfer (S2)^T at the right 
position and complete (S3)^T both over rows in J2.

J2 = | (S1)^T (S3)^T |
     | (S2)^T        |

3rd step: Iterate over rows of S4 and complete over columns in J3:

J3 = | (S1)^T (S3)^T |
     | (S2)^T (S4)^T |       and J3 = S^T

We do the same process for each 8x8 submatrix S and the resulting matrix 
will be B = A^T. 
*/

    
    for(i=0; i<N; i+=8) //split into 8x8 submatrices
        for(j=0; j<M; j+=8)
         {
                
                for (k = i; k < i + 4; k++) //upper half of S: 1st step
				{   //store each element in a variable
					a0 = A[k][j];
                    a1 = A[k][j+1]; 
                    a2 = A[k][j+2]; 
                    a3 = A[k][j+3];
					a4 = A[k][j+4]; 
                    a5 = A[k][j+5]; 
                    a6 = A[k][j+6]; 
                    a7 = A[k][j+7];
					
                    //temporarily update J1
					B[j][k] = a0; 
                    B[j+1][k] = a1; 
                    B[j+2][k] = a2; 
                    B[j+3][k] = a3;
					B[j][k+4] = a4; 
                    B[j+1][k+4] = a5; 
                    B[j+2][k+4] = a6; 
                    B[j+3][k+4] = a7;
				}
				for (k = j; k < j + 4; k++) //2nd step
				{
                    //store the values of S3 and (S2)^T
					a0 = A[i+4][k]; 
                    a1 = A[i+5][k]; 
                    a2 = A[i+6][k]; 
                    a3 = A[i+7][k];
					a4 = B[k][i+4]; 
                    a5 = B[k][i+5]; 
                    a6 = B[k][i+6]; 
                    a7 = B[k][i+7];
					
                    //update correctly J2
					B[k][i+4] = a0; 
                    B[k][i+5] = a1; 
                    B[k][i+6] = a2; 
                    B[k][i+7] = a3;
					B[k+4][i] = a4; 
                    B[k+4][i+1] = a5; 
                    B[k+4][i+2] = a6; 
                    B[k+4][i+3] = a7;
				}
				for (k= i + 4; k < i + 8; k++) //3rd step
				{  //store the values of S4 and complete J3
					a0 = A[k][j+4]; 
                    a1 = A[k][j+5]; 
                    a2 = A[k][j+6]; 
                    a3 = A[k][j+7];
					B[j+4][k] = a0; 
                    B[j+5][k] = a1; 
                    B[j+6][k] = a2; 
                    B[j+7][k] = a3;
				}        
         }
}


/* 32x32:439 misses
*/
char block4_irows_random_desc[] = "Block by 4 iterate over rows for random shape matrix transpose";
void transpose_block4_irows_random(int M, int N, int A[N][M], int B[M][N])
{//using 7 variables
    int i,j,k,a0,a1,a2,a3;
    for(i=0; i<N-N%4; i+=4) //divide the matrix into submatrices of 4x4 
        for(j=0; j<M-M%4; j+=4)//N-N%4 and M-M%4 multiples of 4
            for(k=i; k<i+4; k++)//iterate over rows
            {
                //iterate over row k
                a0 = A[k][j];
                a1 = A[k][j+1];
                a2 = A[k][j+2];
                a3 = A[k][j+3];

                //store in the transpose
                B[j][k] = a0;
                B[j+1][k] = a1;
                B[j+2][k] = a2;
                B[j+3][k] = a3; 

            }


    
     // fill the rest (if N%4 and/or M%4 !=0) 
    for (i = 0; i < N; i++) //fill the remainder at the right
		for (j = M-M%4; j < M; j++)
			{
				a0 = A[i][j];
				B[j][i] = a0;
			}
	for (i = N-N%4; i < N; i++) //fill the remainder at the bottom
		for (j = 0; j < M; j++)
			{
				a0 = A[i][j];
				B[j][i] = a0;
			}
}

/* 32x32:898 misses
   61x67: 1993, winner!
*/
char block16_irows_random_desc[] = "Block by 16 iterate over rows for random shape matrix transpose";
void transpose_block16_irows_random(int M, int N, int A[N][M], int B[M][N])
{//using 11 variables
    int i,j,k,a0,a1,a2,a3,a4,a5,a6,a7;

    for(i=0; i<N-N%16; i+=16) //divide the matrix into submatrices of 16x16 
        for(j=0; j<M-M%16; j+=16) // N-N%16 and M-M%16 multiples of 16 
            for( k=i; k<i+16; k++) //iterate over rows
            {
                // iterate over row k
                a0 = A[k][j];
                a1 = A[k][j+1];
                a2 = A[k][j+2];
                a3 = A[k][j+3];
                a4 = A[k][j+4];
                a5 = A[k][j+5];
                a6 = A[k][j+6];
                a7 = A[k][j+7];

                // store in the transpose
                B[j][k] = a0;
                B[j+1][k] = a1;
                B[j+2][k] = a2;
                B[j+3][k] = a3;
                B[j+4][k] = a4;
                B[j+5][k] = a5;
                B[j+6][k] = a6;
                B[j+7][k] = a7;  

                //finish iterating over row k
                a0 = A[k][j+8];
                a1 = A[k][j+9];
                a2 = A[k][j+10];
                a3 = A[k][j+11];
                a4 = A[k][j+12];
                a5 = A[k][j+13];
                a6 = A[k][j+14];
                a7 = A[k][j+15];

                // strore in transpose
                B[j+8][k] = a0;
                B[j+9][k] = a1;
                B[j+10][k] = a2;
                B[j+11][k] = a3;
                B[j+12][k] = a4;
                B[j+13][k] = a5;
                B[j+14][k] = a6;
                B[j+15][k] = a7;

            }


    
     // deal with the rest of the matrix (if N%16 and/or M%16 !=0)
    for (i = 0; i < N; i++) //fill the remainder at the right
		for (j = M-M%16; j < M; j++)
			{
				a0 = A[i][j];
				B[j][i] = a0;
			}
	for (i = N-N%16; i < N; i++) //fill the remainder at the bottom
		for (j = 0; j < M; j++)
			{
				a0 = A[i][j];
				B[j][i] = a0;
			}
}

/* 
 * trans - A simple baseline transpose function, not optimized for the cache.
 */

/* 32x32:870 misses
   64x64: 
   61x67: 
*/
char trans_desc[] = "Simple row-wise scan transpose";
void trans(int M, int N, int A[N][M], int B[M][N])
{
    int i, j, tmp;

    for (i = 0; i < N; i++) {
        for (j = 0; j < M; j++) {
            tmp = A[i][j];
            B[j][i] = tmp;
        }
    }    

}


/*
 * registerFunctions - This function registers your transpose
 *     functions with the driver.  At runtime, the driver will
 *     evaluate each of the registered functions and summarize their
 *     performance. This is a handy way to experiment with different
 *     transpose strategies.
 */
void registerFunctions()
{
    /* Register your solution function */
    registerTransFunction(transpose_submit, transpose_submit_desc); 

    /* Register any additional transpose functions */
    //registerTransFunction(trans, trans_desc); 
    //registerTransFunction(transpose_block4, block4_desc);
    //registerTransFunction(transpose_block4_irows,block4_irows_desc);
    //registerTransFunction(transpose_block4_i2rows,block4_i2rows_desc);
    //registerTransFunction(transpose_block8_irows, block8_irows_desc);
    //registerTransFunction(transpose_block8_irows_half,block8_irows_half_desc);
    //registerTransFunction(transpose_block4_irows_random, block4_irows_random_desc);
    //registerTransFunction(transpose_block16_irows_random, block16_irows_random_desc);
}

/* 
 * is_transpose - This helper function checks if B is the transpose of
 *     A. You can check the correctness of your transpose by calling
 *     it before returning from the transpose function.
 */
int is_transpose(int M, int N, int A[N][M], int B[M][N])
{
    int i, j;

    for (i = 0; i < N; i++) {
        for (j = 0; j < M; ++j) {
            if (A[i][j] != B[j][i]) {
                return 0;
            }
        }
    }
    return 1;
}

