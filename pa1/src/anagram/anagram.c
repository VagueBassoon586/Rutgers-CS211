#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

int getIndex(char c)
{
  if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z'))
  {
    return ((char)tolower(c)) - 'a';
  }
  else 
  {
    return -1;
  }
}

int main(int argc, char *argv[])
{
  char *input = argv[1];
  int count[26]= {0};
  size_t length = strlen(input);
  size_t letters = 0;

  for (size_t i = 0; i < length; i++) {
    unsigned char lc = getIndex(input[i]);
    if (lc < 26 && lc >= 0)
    {
      count[lc]++;
      letters++;
    }
  }

  char *output = (char *)malloc(letters + 1);
  if (output == NULL)
  {
    return EXIT_FAILURE;
  }

  size_t outputIndex = 0;
  for(int i = 0; i < 26; i++)
  {
    for(int j = count[i]; j > 0; j--)
    {
      output[outputIndex++] = (char)('a' + i);
    }
  }
  output[outputIndex] = '\0';

  printf("%s\n", output);
  free(output);

  return EXIT_SUCCESS;
}
