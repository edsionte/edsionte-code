#include "list.h"
#define NODE_MAX 1024
#define FNAME_MAX 256

struct childNode {
	int child;
	struct list_head list;
};

struct dataNode {
	char fileName[FNAME_MAX];
	int isDir;
	struct childNode *firstChild;
	struct list_head list;
};

/*
struct childTree {
	dataNode nodes[MAX];
	int root;
	int num;
};
*/
