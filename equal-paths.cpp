#include "equal-paths.h"
#include <algorithm>
#include <cmath>
using namespace std;


// You may add any prototypes of helper functions here

bool checkheights(Node* node, int currlevel, int &maxlevel)
{
    if(node == nullptr) //basecase we are at the end of the tree
    {
        return true;
    }
    if(node->left == nullptr && node->right == nullptr) //this is a leaf node so evalute level
    {
        if(maxlevel == 0) //if this is the first leaf node then set maxlevel to currlevel
        {
            maxlevel = currlevel;
            return true;
        } 
        return (currlevel == maxlevel); //if this is not the first then compare currlevel with max level 
    } 
    //recurse through trees and all levels
    return checkheights(node->left, currlevel+1, maxlevel) && checkheights(node->right, currlevel+1, maxlevel);
}

bool equalPaths(Node * root)
{
    int maxlevel = 0;
    int currlevel = 0;
    return (checkheights(root,currlevel, maxlevel));

}


