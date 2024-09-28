/*
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

char transpose_submit_desc[] = "Transpose submission";
void transpose_submit(int M, int N, int A[N][M], int B[M][N])
{
    int v[12]; //12个int局部变量

    if (M == 48 && N == 48)
    {
        for (int i = 0; i < 48; i += 12)
            for (int j = 0; j < 48; j += 12)
                for (int k = i; k < i + 12; k++)
                {
                    for (int t = 0; t < 12; t++) v[t] = A[k][j + t];
                    for (int t = 0; t < 12; t++) B[j + t][k] = v[t];
                }
    }

    else if (M == 96 && N == 96)
    {
        for (int i = 0; i < 96; i += 12)
            for (int j = 0; j < 96; j += 12)
            {
                for (int x = i; x < i + 6; ++x)
                {
                    for (int t = 0; t < 12; t++) v[t] = A[x][j + t];
                    for (int t = 0; t < 6; t++) B[j + t][x] = v[t];
                    for (int t = 0; t < 6; t++) B[j + t][x + 6] = v[t + 6];
                }
                for (int y = j; y < j + 6; ++y)
                {
                    for (int t = 0; t < 6; t++) v[t] = A[i + t + 6][y];
                    for (int t = 0; t < 6; t++) v[t + 6] = B[y][i + t + 6];
                    for (int t = 0; t < 6; t++) B[y][i + t + 6] = v[t];
                    for (int t = 0; t < 6; t++) B[y + 6][i + t] = v[t + 6];
                }
                for (int x = i + 6; x < i + 12; ++x)
                {
                    for (int t = 0; t < 6; t++) v[t] = A[x][j + t + 6];
                    for (int t = 0; t < 6; t++) B[j + t + 6][x] = v[t];
                }
            }
    }

    else  //93 * 99
    {
        for (int j = 0; j < M / 12 * 12; j += 12)
            for (int i = 0; i < N / 12 * 12; ++i)
            {
                for (int t = 0; t < 12; t++) v[t] = A[i][j + t];
                for (int t = 0; t < 12; t++) B[j + t][i] = v[t];
            }
        for (int i = N / 12 * 12; i < N; ++i)
            for (int j = M / 12 * 12; j < M; ++j)
            {
                v[0] = A[i][j];
                B[j][i] = v[0];
            }
        for (int i = 0; i < N; ++i)
            for (int j = M / 12 * 12; j < M; ++j)
            {
                v[0] = A[i][j];
                B[j][i] = v[0];
            }
        for (int i = N / 12 * 12; i < N; ++i)
            for (int j = 0; j < M; ++j)
            {
                v[0] = A[i][j];
                B[j][i] = v[0];
            }
    }
}

/*
 * You can define additional transpose functions below. We've defined
 * a simple one below to help you get started.
 */


 /*
  * trans - A simple baseline transpose function, not optimized for the cache.
  */
char trans_desc[] = "Simple row-wise scan transpose";
void trans(int M, int N, int A[N][M], int B[M][N])
{
    int i, j, tmp;

    for (i = 0; i < N; i++)
    {
        for (j = 0; j < M; j++)
        {
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
    // registerTransFunction(trans, trans_desc);
}

/*
 * is_transpose - This helper function checks if B is the transpose of
 *     A. You can check the correctness of your transpose by calling
 *     it before returning from the transpose function.
 */
int is_transpose(int M, int N, int A[N][M], int B[M][N])
{
    int i, j;

    for (i = 0; i < N; i++)
    {
        for (j = 0; j < M; ++j)
        {
            if (A[i][j] != B[j][i])
            {
                return 0;
            }
        }
    }
    return 1;
}
