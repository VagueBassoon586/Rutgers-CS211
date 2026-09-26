#include <stdio.h>
#include <stdlib.h>

int **initMatrix(size_t k)
{
  int **M = (int **)malloc(k * sizeof(int *));
  if (!M)
  {
    free(M);
    exit(EXIT_FAILURE);
  }
  for (size_t i = 0; i < k; i++)
  {
    M[i] = (int *)malloc(k * sizeof(int));
    if (!M[i])
    {
      for (size_t j = 0; j < k; j++)
        free(M[j]);
      free(M);
      exit(EXIT_FAILURE);
    }
  }
  return M;
}

void freeMatrix(int **M, size_t k)
{
  if (!M)
    return;
  for (size_t i = 0; i < k; i++)
    free(M[i]);
  free(M);
}

void printMatrix(int **M, size_t k)
{
  for (size_t i = 0; i < k; i++)
  {
    for (size_t j = 0; j < k; j++)
    {
      if (j != 0)
        printf(" ");
      printf("%d", M[i][j]);
    }
    printf("\n");
  }
}

int **mulMatrix(int **A, int **B, size_t k)
{
  int **C = initMatrix(k);
  for (size_t i = 0; i < k; i++)
  {
    for (size_t j = 0; j < k; j++)
    {
      long long tot = 0;
      for (size_t t = 0; t < k; t++)
      {
        tot += (long long)(A[i][t]) * (long long)B[t][j];
      }
      C[i][j] = (int)tot;
    }
  }
  return C;
}

int **powMatrix(int **M, int pow, size_t k)
{
  int **res = initMatrix(k);
  if (pow == 0)
  {
    for (size_t i = 0; i < k; i++)
    {
      for (size_t j = 0; j < k; j++)
        res[i][j] = (i == j) ? 1 : 0;
    }
    return res;
  }
  else if (pow == 1)
  {
    for (size_t i = 0; i < k; i++)
    {
      for (size_t j = 0; j < k; j++)
      {
        res[i][j] = M[i][j];
      }
    }
    return res;
  }
  for (size_t i = 0; i < k; i++)
  {
    for (size_t j = 0; j < k; j++)
      res[i][j] = M[i][j];
  }
  while (pow-- > 1)
  {
    int **temp = mulMatrix(res, M, k);
    freeMatrix(res, k);
    res = temp;
  }
  return res;
}

int main(int argc, char *argv[])
{
  FILE *txtFile = fopen(argv[1], "r");
  
  if (!txtFile)
    return EXIT_FAILURE;

  size_t k;
  fscanf(txtFile, "%zu", &k);
  int **M = initMatrix(k);

  for (size_t i = 0; i < k; i++)
  {
    for (size_t j = 0; j < k; j++)
      fscanf(txtFile, "%d", &M[i][j]);
  }
  unsigned int n;
  fscanf(txtFile, "%u", &n);
  fclose(txtFile);

  int **result = powMatrix(M, n, k);
  printMatrix(result, k);
  freeMatrix(M, k);
  freeMatrix(result, k);
  return EXIT_SUCCESS;
}
