// DisjointSet implementation using union by size and path compression
// By Mary Elaine Califf and Priya Gillan

#include "DisjointSet.h"
#include <iostream>
using namespace std;
DisjointSet::DisjointSet(int numObjects)
{
    //to do
    numValues = numObjects;
    
    //assign all indexes -1
    theArray.assign(numObjects, -1);
}

//recursive method to find the item -- does path compression on the way out of the recursion
int DisjointSet::find(int objectIndex)
{
    // to do -- see assignment instructions for details
    //figure 8.16 from the text book
    if(theArray[objectIndex] < 0)
        return objectIndex;
    else 
        return theArray[objectIndex] = find(theArray[objectIndex]);

}

bool DisjointSet::doUnion(int objIndex1, int objIndex2)
{
    // to do -- see assignment instructions for details

    //checking which set the objects are in 
    int root1 = find(objIndex1);
    int root2 = find(objIndex2);

    //getting the sum of both roots
    int sumOfRoot  = theArray[root1] + theArray[root2];
    
    //if the size of the second root is less than the first root 
    if(theArray[root2]<theArray[root1]){
        theArray[root1] = root2;
        theArray[root2] = sumOfRoot;
    }
    //if the size of the first root is less than the second root 
    else{
        theArray[root2] = root1;
        theArray[root1] = sumOfRoot;
    }
    //return if all values are in the same set
    return abs(sumOfRoot) == numValues;
}

void DisjointSet::printArrayValues(std::ostream &outputStream)
{
    for (int i = 0; i < numValues; i++)
    {
        outputStream << theArray[i] << " ";
    }
    outputStream << std::endl;
}
