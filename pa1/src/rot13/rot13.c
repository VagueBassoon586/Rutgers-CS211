#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char charCipher(char c)
{
  if (c >= 'A' && c <= 'Z') {
    return ((c - 'A' + 13) % 26) + 'A';
  }
  else if (c >= 'a' && c <= 'z') {
    return ((c - 'a' + 13) % 26) + 'a';
  }
  else {
    return c;
  }
}

int main(int argc, char *argv[]) {
  if (argc == 1) {
    printf("\n\n");
    return EXIT_SUCCESS;
  }
  else if (argc == 2) {
    char *input = argv[1];
    size_t length = strlen(input);

    char *encoded = malloc(length + 1);
    if (encoded == NULL) {
      return EXIT_FAILURE;
    }

    for (size_t i = 0; i < length; i++){
      encoded[i] = charCipher(input[i]);
    }
    encoded[length] = '\0';

    printf("%s\n", encoded);

    free(encoded);

    return EXIT_SUCCESS;
  }
}
