#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h> 
#define min 0
#define max 999999999

int HandleMenu(gPosition root)
{
    int choice;
    int inputNumber;

    do
    {
        printf("1-Add new number to the tree\n2-Delete number from the tree\n3-Print the tree\n0-Exit\n");
        scanf(" %d", &choice);

        switch (choice)
        {
        case 1:
            printf("Input the number you want to add\n");
            scanf(" %d", &inputNumber);
            root = InsertGElement(root, inputNumber, 0);
            CheckTreeBalance(root);
            break;
        case 2:
            printf("Input the number you want to delete\n");
            scanf(" %d", &inputNumber);
            root = DeleteGElement(root, inputNumber);
            break;
        case 3:
            PrintFromG(root);
            printf("\n");
            break;
        default:
            break;
        }
    }while(choice != 0);
}

wBPosition CreateWBNewElement(int val, int mod, int isWhite)
{
    wBPosition newNode=(wBPosition)malloc(sizeof(whiteBlackNode));
    if(!newNode)
    {
        printf("Can't allocate memory!\n");
        return newNode;
    }
    newNode->value=val;
    newNode->mod=mod;
    newNode->isWhite=isWhite;
    newNode->children=(gPosition*)malloc(mod*sizeof(gPosition));
    for (int i = 0; i < mod; i++)
    {
        newNode->children[i] = NULL;
    }
    return newNode;
}

gPosition CreateGNewElement(int val, int mod)
{
    gPosition newNode=(gPosition)malloc(sizeof(grayNode));
    if(!newNode)
    {
        printf("Can't allocate memory!\n");
        return newNode;
    }


    newNode->value=val;
    newNode->mod=mod;
    newNode->left=NULL;
    newNode->right=NULL;
    
    return newNode;
}

wBPosition InsertWBElement(wBPosition root,int val, int mod, int isWhite)
{
    if(root==NULL)
        return CreateWBNewElement(val, mod, isWhite);

    if(isWhite && val<root->value || !isWhite && val>root->value)
    {
        int temp = root->value;
        root->value = val;
        root->children[temp%mod]=InsertGElement(root->children[temp%mod], temp, mod);
    }
    else
    {
        root->children[val%mod]=InsertGElement(root->children[val%mod], val, mod);
    }

    return root;
}

gPosition InsertGElement(gPosition root,int val, int mod)
{
    int nextMod;

    if(root==NULL)
    {
        return CreateGNewElement(val, mod);
    }

    nextMod = CalculateNextMod(mod);

    if(root->value<val)
        root->right=InsertWBElement(root->right,val,nextMod,0);
    else
        root->left=InsertWBElement(root->left,val,nextMod,1);
    
    return root;
}

int CalculateNextMod(int currentMod)
{
    if(currentMod == 0)
        return 2;

    for (int i = 0; i < 4; i++)
    {
        if (primes[i] == currentMod)
        {
            return primes[i+1];
        }
    }
}

int PrintFromG(gPosition root)
{
    if(root==NULL)
    {
        printf("NULL ");
        return EXIT_SUCCESS;
    }

    printf("%d ", root->value);
    PrintFromWB(root->left);
    PrintFromWB(root->right);

    return EXIT_SUCCESS;
}

int PrintFromWB(wBPosition root)
{
    if(root==NULL)
    {
        printf("NULL ");
        return EXIT_SUCCESS;
    }

    printf("%d-(%d) ", root->value, root->mod);
    for(int i = 0; i < root->mod; i++)
    {
        PrintFromG(root->children[i]);
    }

    return EXIT_SUCCESS;
}

wBPosition DeleteWBElement(wBPosition root,int valToDelete)
{
    if(root == NULL)
        return root;


    if(valToDelete != root->value)
    {
        for(int i = 0; i < root->mod; i++)
        {
            root->children[valToDelete%root->mod]=DeleteGElement(root->children[valToDelete%root->mod], valToDelete);
            return root;
        }
    }

    int isEveryChildNull = 1;
    int childValue;

    for(int i = 0; i < root->mod; i++)
    {
        if(root->children[i] != NULL)
        {
            isEveryChildNull = 0;
            childValue = root->children[i]->value;
        }
    }

    if(isEveryChildNull)
    {
        free(root);
        return NULL;
    }


    if(root->isWhite)
    {
        int smallestElement = childValue;
        
        for(int i = 0; i < root->mod; i++)
        {
            FindElementFromG(root->children[i], &smallestElement, 1);
        }

        root->value = smallestElement;

        root->children[root->value%root->mod] = DeleteGElement(root->children[root->value%root->mod], smallestElement);
    }
    else
    {
        int largestElement = childValue;
        
        for(int i = 0; i < root->mod; i++)
        {
            FindElementFromG(root->children[i], &largestElement, 0);
        }

        root->value = largestElement;

        root->children[root->value%root->mod] = DeleteGElement(root->children[root->value%root->mod], largestElement);
    }

    return root;
}

gPosition DeleteGElement(gPosition root,int valToDelete)
{
    if(root == NULL)
    {
        return root;
    }

    if(valToDelete > root->value)
    {
        root->right = DeleteWBElement(root->right, valToDelete);
        return root;
    }
    if(valToDelete < root->value)
    {
        root->left = DeleteWBElement(root->left, valToDelete);
        return root;
    }

    if(root->value == valToDelete)
    {
        if(root->left == NULL && root->right == NULL)
        {
            free(root);
            return NULL;
        }

        int isEveryLeftGrandChildNull = 1;
        int isEveryRightGrandChildNull = 1;

        for(int i = 0; i < (root->left != NULL ? root->left->mod : root->right->mod); i++)
        {
            if(root->left != NULL)
            {
                if(root->left->children[i] != NULL)
                {
                    root->value = root->left->children[i]->value;
                    root->left->children[i] = DeleteGElement(root->left->children[i], root->value);
                    isEveryLeftGrandChildNull = 0; 
                    return root;
                }
            }

            if(root->right != NULL)
            {
                if(root->right->children[i] != NULL)
                {
                    root->value = root->right->children[i]->value;
                    root->right->children[i] = DeleteGElement(root->right->children[i], root->value); 
                    isEveryRightGrandChildNull = 0; 
                    return root;
                }
            }
        }

        if(isEveryLeftGrandChildNull && root->left != NULL)
        {
            root->value = root->left->value;
            free(root->left);
            root->left = NULL;
        }
        else if(isEveryRightGrandChildNull)
        {
            root->value = root->right->value;
            free(root->right);
            root->right = NULL;
        }
    }

    return root;
}

int FindElementFromWB(wBPosition root, int *element, int isSearchSmallest)
{
    if(root == NULL)
        return EXIT_SUCCESS;

    if(element == NULL)
        *element = root->value;

    if(isSearchSmallest)
    {
        if(root->value < *element)
        {
            *element = root->value;
        }
    }
    else
    {
        if(root->value > *element)
        {
            *element = root->value;
        }
    }

    for(int i = 0; i < root->mod; i++)
    {
        FindElementFromG(root->children[i], element, isSearchSmallest);
    }

    return EXIT_SUCCESS;
}

int FindElementFromG(gPosition root, int *element, int isSearchSmallest)
{
    if(root == NULL)
        return EXIT_SUCCESS;

    if(element == NULL)
        *element = root->value;

    if(isSearchSmallest)
    {
        if(root->value < *element)
        {
            *element = root->value;
        }
    }
    else
    {
        if(root->value > *element)
        {
            *element = root->value;
        }
    }

    FindElementFromWB(root->left, element, isSearchSmallest);
    FindElementFromWB(root->right, element, isSearchSmallest);

    return EXIT_SUCCESS;
}


int CalculateWBDepth(wBPosition root, int* maxDepth, int currentDepth)
{
    
    if(root == NULL)
    {
        return EXIT_SUCCESS;
    }

    if(currentDepth > *maxDepth)
    {
        *maxDepth = currentDepth;
    }

    for(int i = 0; i < root->mod; i++)
    {
        CalculateGDepth(root->children[i], maxDepth, currentDepth+1);
    }

    return EXIT_SUCCESS;
}
int CalculateGDepth(gPosition root, int* maxDepth, int currentDepth)
{
    if(root == NULL)
        return EXIT_SUCCESS;

    if(currentDepth > *maxDepth)
    {
        *maxDepth = currentDepth;
    }

    CalculateWBDepth(root->left, maxDepth, currentDepth+1);
    CalculateWBDepth(root->right, maxDepth, currentDepth+1);

    return EXIT_SUCCESS;    
}

int CheckTreeBalance(gPosition root)
{
    int leftDepth = 0;
    int rightDepth = 0;
    int isBalanced = 0;

    while(!isBalanced)
    {
        leftDepth = 0;
        rightDepth = 0;
        isBalanced = 0;

        CalculateWBDepth(root->left, &leftDepth, 1);
        CalculateWBDepth(root->right, &rightDepth, 1);

        if(leftDepth-rightDepth >= 2)
        {
            int maxTemp = max, grayTemp = 0; 
            FindElementFromWB(root->left, &maxTemp, 0);
            DeleteGElement(root, maxTemp);
            grayTemp = root->value;
            root->value=maxTemp;
            InsertGElement(root, grayTemp, 0);
        }
        else if(rightDepth-leftDepth >= 2)
        {
            int minTemp = min, grayTemp = 0; 
            FindElementFromWB(root->right, &minTemp, 1);
            DeleteGElement(root, minTemp);
            grayTemp = root->value;
            root->value=minTemp;
            InsertGElement(root, grayTemp, 0);
        }
        else
        {
            isBalanced = 1;
        }

    }
}