#ifndef __TRAINER__
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <assert.h>

#define nullptr NULL

typedef struct TNode
{
	int type;
	double value;
	struct TNode *left, *right;
} TNode;

// use direct stored value
#define OP_VALUE 0

// basic arithmetic
#define OP_PLUS 1
#define OP_MINUS 2
#define OP_MULTIPLY 3
#define OP_DIVIDE 4

// left^right power
#define OP_POWER 5
#endif

// allocate tree node on heap, fill it with data and return pointer to it
// no argument checking needed, test env will always pass correct data
TNode *createNode(int type, double value, TNode *left, TNode *right)
{
	TNode *newNode = (TNode *)malloc(sizeof(TNode));
	newNode->type = type;
	newNode->value = value;
	newNode->left = left;
	newNode->right = right;
	return newNode;
}

// free given (sub)tree recursively, including the node
void freeNode(TNode *node)
{
	// TODO
}

// evaluate the tree, return value of expression
double evaluate(TNode *node)
{
	// TODO
}

// print the tree
void print(FILE *f, TNode *node)
{
	// TODO
}

#ifndef __TRAINER__
int main()
{

	TNode *root = nullptr;

	root = createNode(OP_PLUS, 0,
					  createNode(OP_VALUE, 42, nullptr, nullptr),
					  createNode(OP_VALUE, 13, nullptr, nullptr));

	assert(evaluate(root) == 55);
	freeNode(root);

	root = createNode(OP_DIVIDE, 0,
					  createNode(OP_VALUE, 1000, nullptr, nullptr),
					  createNode(OP_VALUE, 4, nullptr, nullptr));

	assert(evaluate(root) == 250);
	root->right->value = 4000;
	assert(evaluate(root) == 0.25);
	root->right->value = 0;
	assert(!isfinite(evaluate(root)));
	freeNode(root);

	root = createNode(OP_POWER, 0,
					  createNode(OP_VALUE, 2, nullptr, nullptr),
					  createNode(OP_MINUS, 0,
								 createNode(OP_VALUE, 7, nullptr, nullptr),
								 createNode(OP_MULTIPLY, 0,
											createNode(OP_VALUE, 1.5, nullptr, nullptr),
											createNode(OP_VALUE, 6, nullptr, nullptr))));

	assert(evaluate(root) == 0.25);
	root->left->value = 1;
	assert(evaluate(root) == 1);
	root->left->value = 0.5;
	assert(evaluate(root) == 4);

	char buf[100];
	FILE *f = fmemopen(buf, sizeof(buf), "w");
	print(f, root);
	fclose(f);
	assert(strcmp(buf, "(0.5 ^ (7 - (1.5 * 6)))") == 0);

	freeNode(root);

	freeNode(nullptr);

	return EXIT_SUCCESS;
}
#endif