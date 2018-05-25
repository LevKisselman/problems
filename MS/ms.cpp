#include <iostream>

using namespace std;


int strspn(char* s1, char* s2, int n1, int n2)
{
    char lookupTable [32];
    for(int i = 0; i < 32; i++)
    {
        lookupTable[i] = 0;
    }  
    
    for(int i = 0; i < n2; i++)
    {
        int numByte = s2[i] / 8;
        int raisedBitOrder = s2[i] % 8;
        int rasedBit = 1 << raisedBitOrder;
        lookupTable[numByte] = lookupTable[numByte] | rasedBit; 
    }
    
    for(int i = 0; i < n1; i++)
    {
        int numByte = s1[i] / 8;
        int raisedBitOrder = s1[i] % 8;
        int rasedBit = 1 << raisedBitOrder;
        
        if(lookupTable[numByte] & rasedBit)
             return i;            
    }
    
    return -1;
}

int main()
{
    cout<<endl<<"Empty Sring Tests:"<<endl;
    cout<<(-1 == strspn("","",0,0) ? "Pass" : "Fail")<<endl;
    cout<<(-1 == strspn("abcd","",4,0) ? "Pass" : "Fail")<<endl;
    cout<<(-1 == strspn("","abcd",0,4)  ? "Pass" : "Fail")<<endl;
    
    cout<<endl<<"No Matches Tests:"<<endl;
    cout<<(-1 == strspn("abcd","e",4,1)  ? "Pass" : "Fail")<<endl;
    cout<<(-1 == strspn("abcd","fghi",4,4)  ? "Pass" : "Fail")<<endl;
    
    cout<<endl<<"Correct Matches Tests:"<<endl;
    cout<<(0 == strspn("abcd","a",4,1)  ? "Pass" : "Fail")<<endl;
    cout<<(3 == strspn("abcd","d",4,1)  ? "Pass" : "Fail")<<endl;
    
    cout<<(0 == strspn("abcd","xxxa",4,4)  ? "Pass" : "Fail")<<endl;
    cout<<(3 == strspn("abcd","xxxd",4,4)  ? "Pass" : "Fail")<<endl;
    
    // Also a good idea: large s1, large s2
    return 0;
}