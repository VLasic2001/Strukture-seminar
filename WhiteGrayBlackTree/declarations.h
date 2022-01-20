struct _whiteBlackNode;
typedef struct _whiteBlackNode* wBPosition;
struct _grayNode;
typedef struct _grayNode* gPosition;

typedef struct _whiteBlackNode
{
    int value;
    int mod;
    int isWhite;
	gPosition* children;
} whiteBlackNode;

typedef struct _grayNode
{
    int value;
    int mod;
    wBPosition left;
	wBPosition right;
} grayNode;
int primes[5] = {2,3,5,7,11};

int HandleMenu(gPosition root);
int CalculateNextMod(int currentMod);
int FindElementFromWB(wBPosition root, int* smallestElement, int isSearchSmallest);
int FindElementFromG(gPosition root, int* smallestElement, int isSearchSmallest);
wBPosition CreateWBNewElement(int val, int mod, int isWhite);
gPosition CreateGNewElement(int val, int mod);
wBPosition InsertWBElement(wBPosition root,int val, int mod, int isWhite);
gPosition InsertGElement(gPosition root,int val, int mod);
wBPosition DeleteWBElement(wBPosition root,int valToDelete);
gPosition DeleteGElement(gPosition root,int valToDelete);   
int PrintFromG(gPosition root);
int PrintFromWB(wBPosition root);
int CalculateWBDepth(wBPosition root, int* maxDepth, int currentDepth);
int CalculateGDepth(gPosition root, int* maxDepth, int currentDepth);
int CheckTreeBalance(gPosition root);