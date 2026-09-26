#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Node
{
	int value;
	struct Node *left;
	struct Node *right;
} Node;

Node *newNode(int value)
{
	Node *n = (Node *)malloc(sizeof(Node));
	if (n == NULL)
		exit(EXIT_FAILURE);
	n->value = value;
	n->left = NULL;
	n->right = NULL;
	return n;
}

Node *successor(Node *node)
{
	node = node->right;
	while (node != NULL && node->left != NULL)
		node = node->left;
	return node;
}

void printNode(Node *root)
{
	if (root == NULL)
		return;
	printf("(");
	printNode(root->left);
	printf("%d", root->value);
	printNode(root->right);
	printf(")");
}

void freeTree(Node *root)
{
	if (root == NULL)
		return;
	freeTree(root->left);
	freeTree(root->right);
	free(root);
}

Node *insert(Node *root, int value, int *inserted)
{
	*inserted = 0;
	if (root == NULL)
	{
		Node *temp = newNode(value);
		if (temp == NULL)
			return NULL;
		*inserted = 1;
		return temp;
	}
	Node *parent = NULL;
	Node *current = root;
	while (current != NULL)
	{
		if (current->value > value)
		{
			parent = current;
			current = current->left;
		}
		else if (current->value < value)
		{
			parent = current;
			current = current->right;
		}
		else
		{
			*inserted = 0;
			return root;
		}
	}
	Node *temp = newNode(value);
	if (temp == NULL)
		return root;
	if (parent->value > value)
		parent->left = temp;
	else if (parent->value < value)
		parent->right = temp;
	*inserted = 1;
	return root;
}

int search(Node *root, int value)
{
	Node *current = root;
	while (current != NULL)
	{
		if (current->value < value)
			current = current->right;
		else if (current->value > value)
			current = current->left;
		else
			return 1;
	}
	return 0;
}

Node *delete(Node *root, int value, int *deleted)
{
	*deleted = 0;
	if (root == NULL)
		return NULL;

	// -1 = left
	// 0 = root
	// 1 = right
	signed int left_root_right = 0;
	Node *parent = NULL;
	Node *current = root;
	while (current != NULL && current->value != value)
	{
		if (current->value > value)
		{
			parent = current;
			current = current->left;
			left_root_right = -1;
		}
		else if (current->value < value)
		{
			parent = current;
			current = current->right;
			left_root_right = 1;
		}
	}
	if (current == NULL)
	{
		*deleted = 0;
		return root;
	}

	*deleted = 1;
	// Cases when current has 0 or 1 child
	if (current->left == NULL || current->right == NULL) {
		Node *child = current->left ? current->left : current->right; /* may be NULL */

		if (!parent) {
			free(current);
			return child;
		}

		if (left_root_right == -1) 
			parent->left = child;
		else                 
			parent->right = child;
		free(current);
		return root;
	}
	Node *tempPar = current;
	Node *temp = current->right;
	while (temp->left != NULL)
	{
		tempPar = temp;
		temp = temp->left;
	}
	if (parent == NULL)
		root->value = temp->value;
	else
		current->value = temp->value;
	Node *tempChild = temp->right;
	if (tempPar->left == temp)
		tempPar->left = tempChild;
	else
		tempPar->right = tempChild;
	free(temp);
	return root;
}

int main(void)
{
	Node *root = NULL;
	char line[256];
	while(fgets(line, sizeof(line), stdin) != NULL)
	{
		char operator;
		int val;
		if (sscanf(line, "%c %d", &operator, &val) == 2)
		{
			switch(operator)
			{
				case 'i':
					int inserted = 0;
					root = insert(root, val, &inserted);
					printf("%s\n", inserted ? "inserted" : "not inserted");
					break;
				case 's':
					printf("%s\n", search(root, val) ? "present" : "absent");
					break;
				case 'd':
					int deleted = 0;
					root = delete(root, val, &deleted);
					printf("%s\n", deleted ? "deleted" : "absent");
					break;
				default:
					break;
			}
		}
		else if (sscanf(line, "%c", &operator) == 1)
		{
			if (operator == 'P' || operator == 'p')
			{
				if (root == NULL)
					printf("\n");
				else
				{
					printNode(root);
					printf("\n");
				}
			}
		}
	}
	freeTree(root);
	return EXIT_SUCCESS;
}
