#include <iostream>
#include <fstream>
#include <string>
#include <map>
using namespace std;

int main()
{
    if( ifstream fin{ []{ string s; getline(cin, s); return s; }() } )
    {
        map<string, int> dict;

        for( string s; fin >> s; ) dict[s]++;

        for( auto& a : dict ) cout << a.first << " : " << a.second << endl;
    }
    else cout << "Error while opening the file!" << endl;

    return cin.get();
}
