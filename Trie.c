#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#define MAX_NAME_SIZE 100
typedef struct _node node;

struct _node
{
	char name[MAX_NAME_SIZE];
	node* father;
	node** children;
	int childrenBufferSize;
	int numberOfChildren;
};

void extendChildren(node *n)
{
	node** temp = n->children;
	n->children = malloc(n->childrenBufferSize * 2 * sizeof(node*));
	for(int i=0; i<n->numberOfChildren;i++)
		n->children[i] = temp[i];
	
	n->childrenBufferSize *= 2;
	free(temp);
}

void add(node *base, node *newNode)
{
	if(base->numberOfChildren == base->childrenBufferSize)
		extendChildren(base);
	
	int pos;
	for(pos=0;pos<base->numberOfChildren;pos++)
		if(strcmp(base->children[pos]->name, newNode->name) >=0)
			break;
	
	for(int i = base->numberOfChildren-1; i>=pos; i--)
		base->children[i+1] = base->children[i];
		
	base->children[pos] = newNode;
	base->numberOfChildren++;
	newNode->father = base;
}

void removeFromFatherList(node *n)
{	
	node *father = n->father;
	int flag = 0;
	for(int i=0;i<father->numberOfChildren;i++)
	{
		if(flag == 1)
			father->children[i-1] = father->children[i];
		if(father->children[i] == n)
			flag = 1;
	}	
	father->numberOfChildren--;
}

void removeNode(node *n)
{
	node *father = n->father;

	removeFromFatherList(n);

	for(int i=0;i<n->numberOfChildren;i++)
		add(father,n->children[i]);

	free(n->children);
	free(n);
}

node* create(char* name)
{
	node* n = (node*) malloc(sizeof(node));
	strcpy(n->name, name);
	n->childrenBufferSize = 1;
	n->numberOfChildren = 0;
	n->children = malloc(n->childrenBufferSize * sizeof(node*));
	return n;
}

void changeHead(node* n, node* newHead)
{
	removeFromFatherList(n);
	add(newHead, n);
}

void freeMemory(node *n)
{
	for(int i=0;i<n->numberOfChildren;i++)
		freeMemory(n->children[i]);
	free(n->children);
	free(n);
}

void printNode(node *n, int depth)
{
	for(int i=0;i<depth;i++)
		printf("\t");
	printf("%s\n", n->name);
	for(int i=0;i<n->numberOfChildren;i++)
		printNode(n->children[i], depth+1);
	
}

node* findByName(char* name, node *n)
{
	if(strcmp(name,n->name) == 0)
		return n;
	for(int i=0;i<n->numberOfChildren;i++)
	{
		node* temp = findByName(name,n->children[i]);
		if(temp != NULL)
			return temp;
	}
	return NULL;
}

void printFathers(node *n)
{
	int pathLen = strlen(n->name);
	char* path = (char*) malloc(pathLen*sizeof(char));
	strcpy(path,n->name);
	node* temp = n;

	while(temp->father != NULL)
	{
		temp = temp->father;
		int additionLen = strlen(temp->name);
		char* newPath = (char*) malloc((pathLen+additionLen + 1) * sizeof(char));
		strcpy(newPath, temp->name);
		newPath[additionLen] = '/';
		strcpy(&newPath[additionLen+1],path);
		char *tempPointer = path;
		path = newPath;	
		pathLen += additionLen+1;
		free(tempPointer);
	}
	printf("%s\n", path);
	free(path);
}

char options()
{
	printf("\noptions:\n");
	printf("1 - print\n");
	printf("2 - print head\n");
	printf("3 - create\n");
	printf("4 - delete\n");
	printf("5 - change head\n");
	printf("6 - exit\n");
	
	return getchar();
}
void main()
{
	node *root = create("root");
	node *temp, *father;
	char name[MAX_NAME_SIZE], fatherName[MAX_NAME_SIZE];
			
	while(1)
	{

		int value = options();
		switch(value)
		{
			case '1':
				for(int i=0;i<root->numberOfChildren;i++)
					printNode(root->children[i],0);
				break;
			case '2':
				printf("enter node name - ");
				scanf("%s", name);
				temp = findByName(name, root);
				if(temp == NULL)
					printf("No such node found\n");
				else
					printFathers(temp);
				break;
			case '3':
				printf("enter node name and father name - ");
				scanf("%s %s", name, fatherName);
				temp = create(name);
				father = findByName(fatherName, root);
				if(father == NULL)
					father = root;
				add(father, temp);
				break;
			case '4':
				printf("enter node name to delete - ");
				scanf("%s", name);
				temp = findByName(name, root);
				if(temp == NULL)
					printf("No such node found\n");
				else
					removeNode(temp);
				break;
			case '5':
				printf("enter node name and the new father name - ");
				scanf("%s %s", name, fatherName);
				temp = findByName(name, root);
				father = findByName(fatherName, root);
				if(temp == NULL)
					printf("No such node found\n");
				else if(father == NULL)
					printf("No such father node found\n");
				else
					changeHead(temp, father);
					
				break;
			
			case '6':
				freeMemory(root);
				return;
			default:
				printf("unknown command, try again\n");
				break;
		}
		getchar();
	}
}