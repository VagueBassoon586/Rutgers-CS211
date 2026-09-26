#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Node
{
  int value;
  struct Node *next;
} Node;

Node *insert(Node *head, int x, int *len)
{
  Node *prev = NULL, *curr = head;
  while (curr != NULL && curr-> value < x)
  {
    prev = curr;
    curr = curr->next;
  }

  if (curr != NULL && curr->value == x)
    return head;

  Node *newNode = (Node *)malloc(sizeof(Node));
  newNode->value = x;
  newNode->next = curr;
  if (prev != NULL)
    prev->next = newNode;
  else 
    head = newNode;

  (*len)++;
  return head;
}

Node *delete(Node *head, int x, int *len)
{
  Node *prev = NULL, *curr = head;
  while (curr != NULL && curr->value != x)
  {
    prev = curr;
    curr = curr->next;
  }

  if (curr == NULL)
    return head;
  
  if (prev != NULL)
    prev->next = curr->next;
  else
    head = curr->next;
  free(curr);
  (*len)--;
  return head;
}

void printList(Node *head, int len)
{
  int first = 1;
  Node *outNode = head;
  printf("%d :", len);
  if (len != 0)
    printf(" ");
  while (outNode != NULL)
  {
    if (!first)
      printf(" ");
    printf("%d", outNode->value);
    first = 0;
    outNode = outNode->next;
  }
  printf("\n");
}

int main(void)
{
  Node *head = NULL;
  int len = 0;
  char line[256];

  while (fgets(line, sizeof(line), stdin) != NULL)
  {
    char operator;
    int n;

    sscanf(line, "%c %d", &operator, &n);
    switch (operator)
    {
      case 'i':
        head = insert(head, n, &len);
        break;
      case 'd':
        head = delete(head, n, &len);
        break;
      default:
        continue;
    }

    printList(head, len);
  }
  
  while (head != NULL)
  {
    Node *nextNode = head->next;
    free(head);
    head = nextNode;
  }

  return EXIT_SUCCESS;
}
