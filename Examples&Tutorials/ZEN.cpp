#include <iostream>
#include <string>
#include <fstream>
#include <map>
using namespace std;

int main() 
{
    if( ifstream fin{ "in.txt" } ) {
        map<string, int> dict;

        for( string s; fin >> s; ) dict[s]++;

        for( auto& a : dict ) cout << a.first << " : " << a.second << endl;
    }
    else cout << "Error while opening the file!" << endl;
    
    return cin.get();
}
