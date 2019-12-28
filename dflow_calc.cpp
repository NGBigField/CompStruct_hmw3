#include "dflow_calc.h"
#include <vector>
#include <algorithm>
#include <iostream>

using std::vector;

const int NUM_REG = 32;



/*    Here we announce stuff:        */

typedef struct Node_ {

	struct Node_* pf1 = NULL;
	struct Node_* pf2 = NULL;
	
	unsigned int oppCode ;
	int instNum ;
	
	int maxTime ;
	int excTime ;


} Node;
typedef Node* pNode;

typedef struct CTX_ {
	vector<pNode> instArray;
} CTX;
typedef CTX* pCTX;



/*          Here are the functions:      */


ProgCtx analyzeProg(const unsigned int opsLatency[], const InstInfo progTrace[], unsigned int numOfInsts) {
    
	try
	{
		/* Create a temp array of Archtectural Dynamic Registers: */
		vector<pNode> regArray;
		for (int regInd = 0; regInd < NUM_REG; regInd++)
		{
			regArray.push_back(NULL);
		}


		/*   */
		pCTX pctx = new CTX;

		for (unsigned int i = 0; i < numOfInsts; i++)
		{
			pNode pnode = new Node;

			pnode->instNum = i; //serial number of the instruction
			pnode->oppCode = progTrace[i].opcode;

			/*Dependencies are kept in the regArray:*/
			pnode->pf1 = regArray[progTrace[i].src1Idx];
			pnode->pf2 = regArray[progTrace[i].src2Idx];

			/*compute max time*/
			int time1 = (pnode->pf1 == NULL) ? 0 : (pnode->pf1->maxTime + opsLatency[pnode->pf1->oppCode]);
			int time2 = (pnode->pf2 == NULL) ? 0 : (pnode->pf2->maxTime + opsLatency[pnode->pf2->oppCode]);
			pnode->maxTime = std::max(time1, time2);

			/*For the calc_max_function: */
			pnode->excTime = opsLatency[pnode->oppCode];

			/* Keep the node at dynamic reg array    and    in stable instruction table :*/
			regArray[progTrace[i].dstIdx] = pnode; //Update regArray.
			pctx->instArray.push_back(pnode);   //update instArray 
		}

		/*end  and   return after cast*/
		return (ProgCtx)pctx;
	}
	catch (...)
	{
		std::cout << "error creating struct" << std::endl;
		return PROG_CTX_NULL; //if error
	}


}

void freeProgCtx(ProgCtx ctx) {
	// pCTX pctx = dynamic_cast<pCTX>(ctx);
	pCTX pctx = (pCTX)ctx ;
	
	for (unsigned int i = 0; i < pctx->instArray.size(); i++)
	{
		delete pctx->instArray[i];
	}
	delete pctx;
}

int getInstDepth(ProgCtx ctx, unsigned int theInst) {
	// pCTX pctx = dynamic_cast<pCTX>(ctx);
	pCTX pctx = (pCTX)ctx;

	if ((theInst >= pctx->instArray.size() )|| (theInst < 0 ) ) return -1;
	else return pctx->instArray[theInst]->maxTime;

}

int getInstDeps(ProgCtx ctx, unsigned int theInst, int* src1DepInst, int* src2DepInst) {
	// pCTX pctx = dynamic_cast<pCTX>(ctx);
	pCTX pctx = (pCTX)ctx;

	if ((theInst >= pctx->instArray.size()) || (theInst < 0)) return -1;
	else
	{
		pNode father1 = pctx->instArray[theInst]->pf1;
		pNode father2 = pctx->instArray[theInst]->pf2;
		*src1DepInst = (father1 == NULL) ? -1 : father1->instNum;
		*src2DepInst = (father2 == NULL) ? -1 : father2->instNum;
		return 0;
	}

}

int getProgDepth(ProgCtx ctx) {
	// pCTX pctx = dynamic_cast<pCTX>(ctx);
	pCTX pctx = (pCTX)ctx;

	int max = 0;

	for (unsigned int i = 0; i < pctx->instArray.size(); i++)
	{
		int crntTime = pctx->instArray[i]->maxTime + pctx->instArray[i]->excTime  ;
		max = (max < crntTime) ? crntTime  : max;
	}

	return max;
}


