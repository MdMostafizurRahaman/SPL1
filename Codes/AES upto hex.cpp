#include<bits/stdc++.h>
using namespace std;

char keyInHex[4][4];
char plainTextInHex[4][4];
string keyInEnglish, plainTextInEnglish;


string calculateKeyInHex(string keyInEnglish)
{
    for(int i=0;i<4;i++)
    {
        for(int j=0;j<4;j++)
        {
            keyInHex[i][j] = int(keyInEnglish[i+4*j]); // converting each character to its ascii value 
            cout << hex << int(keyInHex[i][j]) << " ";
        }
        cout << endl;
    }
}

string calculatePlainTextInHex(string plainTextInEnglish)
{
    for(int i=0;i<4;i++)
    {
        for(int j=0;j<4;j++)
        {
            plainTextInHex[i][j] = int(plainTextInEnglish[i+4*j]); // converting each character to its ascii value 
            cout << hex << int(plainTextInHex[i][j]) << " ";
        }
        cout << endl;
    }
}



int main(){
    keyInEnglish="Thats my Kung Fu";
    plainTextInEnglish="Two One Nine Two";
    cout << "Key In Hex\n" << endl;
    calculateKeyInHex(keyInEnglish);
    cout << endl;
    cout << "Plain Text In Hex\n" << endl;
    calculatePlainTextInHex(plainTextInEnglish);
    
}