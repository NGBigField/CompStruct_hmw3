//Hello World!

#include "dflow_calc.h"


/*    Here we announce stuff:        */

typedef struct Node_ {
	Node* pLSon = NULL;
	Node* pRSon = NULL;
	int opCode = 0;
	int Ltime = 0;
	int Rtime = 0;
	int Llength = 0;
	int Rlength = 0;
} Node;
typedef Node* pNode;

typedef struct DTree_ {
	Node* Exit = NULL;
	int Ltime = 0;
	int Rtime = 0;
	int Llength = 0;
	int Rlength = 0;
} DTree;



/*          Here are the functions:      */


ProgCtx analyzeProg(const unsigned int opsLatency[], const InstInfo progTrace[], unsigned int numOfInsts) {
    return PROG_CTX_NULL;
}

void freeProgCtx(ProgCtx ctx) {
}

int getInstDepth(ProgCtx ctx, unsigned int theInst) {
    return -1;
}

int getInstDeps(ProgCtx ctx, unsigned int theInst, int* src1DepInst, int* src2DepInst) {
    return -1;
}

int getProgDepth(ProgCtx ctx) {
    return 0;
}


