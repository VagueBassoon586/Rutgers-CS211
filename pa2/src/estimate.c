#include <stdio.h>
#include <stdlib.h>
#include <string.h>


typedef struct
{
  int row, column;
  double **data;
} Matrix;

Matrix initMatrix(int k, int n)
{
  Matrix M;
  M.row = n;
  M.column = k;
  M.data = (double **)malloc(n * sizeof(double *));
  if (!M.data)
    exit(EXIT_FAILURE);
  for (int i = 0; i < n; i++)
  {
    M.data[i] = (double *)malloc(k * sizeof(double));
    if (!M.data[i])
      exit(EXIT_FAILURE);
  }
  return M;
}

Matrix createIdentity(int n)
{
  Matrix M = initMatrix(n, n);
  for (int row = 0; row < n; row++)
    for (int column = 0; column < n; column++)
      M.data[row][column] = (row == column ? 1.0 : 0.0);
  return M;
}

void freeMatrix(Matrix *M)
{
  for (int i = 0; i < M->row; i++)
    free(M->data[i]);
  free(M->data);
  M->row = 0;
  M->column = 0;
  M->data = NULL;
}

Matrix transposeMatrix(Matrix *M)
{
  Matrix MT = initMatrix(M->row, M->column);
  for (int i = 0; i < M->row; i++)
    for (int j = 0; j < M->column; j++)
      MT.data[j][i] = M->data[i][j];
  return MT;
}

Matrix mulMatrix(Matrix *A, Matrix *B)
{
  Matrix C = initMatrix(B->column, A->row);
  for (int i = 0; i < A->row; i++)
    for (int j = 0; j < B->column; j++)
    {
      double sum = 0;
      for (int t = 0; t < B->row; t++)
        sum += A->data[i][t] * B->data[t][j];
      C.data[i][j] = sum;
    }
  return C;
}

Matrix invertMatrix(Matrix *M)
{
  int n = M->row;
  double f = 0.0;
  Matrix MI = createIdentity(n);
  for (int p = 0; p < n; p++)
  {
    f = M->data[p][p];
    for (int t = 0; t < n; t++)
    {
      M->data[p][t] /= f;
      MI.data[p][t] /= f;
    }
    for (int i = p + 1; i < n; i++)
    {
      f = M->data[i][p];
      for (int t = 0; t < n; t++)
      {
        M->data[i][t] -= M->data[p][t] * f;
        MI.data[i][t] -= MI.data[p][t] * f;
      }
    }
  }
  for (int p = n - 1; p >= 0; p--)
  {
    for (int i = p - 1; i >= 0; i--)
    {
      f = M->data[i][p];
      for (int t = 0; t < n; t++)
      {
        M->data[i][t] -= M->data[p][t] * f;
        MI.data[i][t] -= MI.data[p][t] * f;
      }
    }
  }
  return MI;
}

int main(int argc, char **argv)
{
  if (argc != 3)
    return EXIT_FAILURE;
  
  char *trainPath = argv[1];
  FILE *trainFile = fopen(trainPath, "r");
  if (!trainFile)
    return EXIT_FAILURE;
  char trainChar[8];
  if (fscanf(trainFile, "%7s", trainChar) != 1 || strcmp(trainChar, "train") != 0)
  {
    fclose(trainFile);
    return EXIT_FAILURE;
  }

  int k, n;
  if (fscanf(trainFile, "%d", &k) != 1 || fscanf(trainFile, "%d", &n) != 1)
  {
    fclose(trainFile);
    return EXIT_FAILURE;
  }

  Matrix X = initMatrix(k + 1, n);
  Matrix Y = initMatrix(1, n);

  for (int i = 0; i < n; i++)
  {
    X.data[i][0] = 1.0;
    for (int j = 1; j <= k; j++)
    {
      if (fscanf(trainFile, "%lf", &X.data[i][j]) != 1)
      {
        freeMatrix(&X);
        freeMatrix(&Y);
        fclose(trainFile);
        return EXIT_FAILURE;
      }
    }
    if (fscanf(trainFile, "%lf", &Y.data[i][0]) != 1)
    {
      freeMatrix(&X);
      freeMatrix(&Y);
      fclose(trainFile);
      return EXIT_FAILURE;
    }
  }

  fclose(trainFile);

  char *dataPath = argv[2];
  FILE *dataFile = fopen(dataPath, "r");
  if (!dataFile)
    return EXIT_FAILURE;
  char dataChar[8];
  if (fscanf(dataFile, "%7s", dataChar) != 1 || strcmp(dataChar, "data") != 0)
  {
    fclose(dataFile);
    return EXIT_FAILURE;
  }

  int attributes, houses;
  if (fscanf(dataFile, "%d", &attributes) != 1 || fscanf(dataFile, "%d", &houses) != 1)
  {
    fclose(dataFile);
    return EXIT_FAILURE;
  }

  Matrix Xd = initMatrix(attributes + 1, houses);
  for (int i = 0; i < houses; i++)
  {
    Xd.data[i][0] = 1.0;
    for (int j = 1; j <= attributes; j++)
    {
      if (fscanf(dataFile, "%lf", &Xd.data[i][j]) != 1)
      {
        freeMatrix(&Xd);
        fclose(dataFile);
        return EXIT_FAILURE;
      }
    }
  }
  Matrix XT = transposeMatrix(&X);
  Matrix XTX = mulMatrix(&XT, &X);
  Matrix Inv = invertMatrix(&XTX);
  Matrix XTY = mulMatrix(&XT, &Y);
  Matrix W = mulMatrix(&Inv, &XTY);
  Matrix Yd = mulMatrix(&Xd, &W);
  
  for (int i = 0; i < Yd.row; i++)
    printf("%.0f\n", Yd.data[i][0]);
  freeMatrix(&X);
  freeMatrix(&Y);
  freeMatrix(&Xd);
  freeMatrix(&XT);
  freeMatrix(&XTX);
  freeMatrix(&Inv);
  freeMatrix(&XTY);
  freeMatrix(&W);
  freeMatrix(&Yd);
  exit(0);
}
