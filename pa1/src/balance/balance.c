#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Node
{
  char character;
  struct Node *next;
} Node;

typedef struct
{
  Node *top;
} Stack;

void iniStack(Stack *s)
{
  s->top = NULL;
}

int isEmpty(Stack *s)
{
  return s->top == NULL;
}

int size(Stack *s)
{
  if (s->top == NULL)
  {
    return 0;
  }
  int length = 0;
  Node *currNode = s->top;
  while (currNode != NULL)
  {
    currNode = currNode->next;
    length++;
  }
  return length;
}

void push(Stack *s, char value)
{
  Node *newNode = malloc(sizeof(Node));
  if (newNode == NULL)
  {
    free(newNode);
    exit(EXIT_FAILURE);
  }
  newNode->character = value;
  newNode->next = s->top;
  s->top = newNode;
}

int pop(Stack *s)
{
  if (isEmpty(s))
  {
    return -1;
  }

  Node *temp = s->top;
  char val = temp->character;
  s->top = temp->next;
  free(temp);
  return val;
}

int match(char open, char close)
{
  return (open == '(' && close == ')') || (open == '[' && close == ']') || (open == '{' && close == '}') || (open == '<' && close == '>');
}

int isOpen(char c)
{
  return c == '(' || c == '[' || c == '{' || c == '<';
}

int isClose(char c)
{
  return c == ')' || c == ']' || c == '}' || c == '>';
}

char expectedCloser(char open)
{
  switch (open)
  {
    case '(': return ')';
    case '[': return ']';
    case '{': return '}';
    case '<': return '>';
    default: return ')';
  }
}

void clear(Stack *s)
{
  while(!isEmpty(s))
  {
    pop(s);
  }
  return;
}

int main(int argc, char *argv[])
{
  char *input = argv[1];
  size_t length = strlen(input);
  Stack charStack;
  iniStack(&charStack);
  for (size_t i = 0; i < length; i++)
  {
    if (isOpen(input[i]))
    {
      push(&charStack, input[i]);
    }
    else if (isClose(input[i]))
    {
      if (isEmpty(&charStack))
      {
        printf("%zu: %c\n", i, input[i]);
        clear(&charStack);
        return EXIT_FAILURE;
      }
      else
      {
        char open = pop(&charStack);
        if (!match(open, input[i]))
        {
          printf("%zu: %c\n", i, input[i]);
          clear(&charStack);
          return EXIT_FAILURE;
        }
      }
    }
  }
  if (isEmpty(&charStack))
  {
    return EXIT_SUCCESS;
  }
  else 
  {
    int len = size(&charStack);
    char *expecting = malloc(len + 1);
    for (int i = 0; i < len; i++)
    {
      char popped = pop(&charStack);
      expecting[i] = expectedCloser(popped);
    }
    expecting[len] = '\0';
    printf("open: %s", expecting);
    free(expecting);
    return EXIT_FAILURE;
  }
}
