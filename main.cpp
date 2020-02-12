#include "ThreadTree.h"
#include <iostream>
using namespace std;
int main(int argc, const char * argv[])
{
    ThreadTree tree;
    ElemType Pre[7] = {1,2,4,5,3,6,7};
    ElemType In[7]  = {4,2,5,1,6,3,7};
    tree = PreInCreateTree(Pre, In, 0, 6, 0, 6);
    InOrderThreading(tree);
    InOrderThreadTree(tree);
    destroyInThreadTree(tree);
    cout << endl;
    return 0;
}
