// This should be some sort of example of desirable virtual machine. 
// Created only to demonstrate several main concepts and differences 
// (compared with commonly used VMs).
// By now - just a development log.

#include <unordered_map>
#include <vector>
#include <string>
#include <iostream>
#include <sstream>

using namespace std;

// ==========================================================================

using Instruction = int ( *)(int);

struct InstInst  // instruction instance (similar to common 'Token' concept)
{
    int id;
    int par;
};

vector<InstInst> InstQ;  // execution sequence

vector<int> values;  // kinda system stack

struct InstReg // single entry for instructions table
{
    string name;
    Instruction inst;
};

// ==========================================================================

// just some (not so)simple instructions

int push( int I )
{
    values.push_back( I );
    return 0;
}
int add( int I )
{
    if( I < 2 || values.size() < I )
        return -1;
    else
    {
        int S = values.back();
        for( int i = 1; i < I; ++i )
        {
            values.pop_back();
            S += values.back();
        }
        values.pop_back();
        values.push_back( S );
    }
    return 0;
}
int mul( int I )
{
    if( I < 2 || values.size() < I )
        return -1;
    else
    {
        int S = values.back();
        for( int i = 1; i < I; ++i )
        {
            values.pop_back();
            S *= values.back();
        }
        values.pop_back();
        values.push_back( S );
    }
    return 0;
}
int prnt( int I )
{
    cout << ":";
    if( I < 1 || values.size() < I )
        for( auto& a : values )
            cout << a << "  ";
    else
        for( int i = 0; i < I; ++i )
            cout << *(values.rbegin() + i) << "  ";
    cout << endl;
    return 0;
}

// instructions table, should be dynamically set in the future.
InstReg InstTab[] = {
    { { "`" }, push },
    { { "+" }, add },
    { { "*" }, mul },
    { { "$" }, prnt }
};

// ==========================================================================

bool isSep( const char& c )
{
    return c == ' ' || c == '\t' || c == '\n';
}

vector<string> input()  //stupid kind of parser-tokenizer (tokens are dissolved)
{
    vector<string> ret;
    string text;
    
    cout << ">> ";
    
    getline( cin, text );
    
// does not work
//    while( text.size() )
//    {
//        string word;
//        for( int i = 0;; ++i )
//        {
//            if( isSep( text[i] ) || i == text.size()-1 )
//                if( word.size() )
//                {
//                    ret.push_back( word );
//                    text = text.substr( i+1 );
//                    cout << "'" << word << "'   |" << text << "|" << endl;
//                    break;
//                }
//                else
//                    continue;
//            else
//                word.push_back( text[i] );
//        }
//    }
    string word;
    istringstream str( text.c_str() );
    while( !str.eof() )
    {
        str >> word;
        ret.push_back( word );
    }
    return ret;
}

// hard-coded Tokenizer + Code Generator. No AST processing at all. No
// optimization. No flexibility. No taste, no color, nothing...
int interpret( vector<string> tokens )
{
    for( auto& a : tokens )
    {
        if( a == "+" )
        {
            InstQ.push_back( {1, 2} );
        }
        else if( a == "*" )
        {
            InstQ.push_back( { 2, 2 } );
        }
        else if( a == "$" )
        {
            InstQ.push_back( { 3, 0 } );
        }
        else
        {
            try
            {
                int n = stoi( a );
                InstQ.push_back( { 0, n } );
            }
            catch( invalid_argument )
            {
                return -1;
            } 
        }
    }
    return 0;
}

int clear()
{
    values.clear();
    InstQ.clear();
    return 0;
}

// THE LOOP! This is the best part and seems to be the only one close to the
// very idea of desirable VM.
int loop()
{
    while( interpret( input() ) == 0 )
    {
        for( auto& a : InstQ )
            if( InstTab[a.id].inst( a.par ) < 0 )
                cout << "Runtime error!" << endl
                     << InstTab[a.id].name << endl
                     << a.par << endl;
            
        clear();
    }
    return 0;
}

// ==========================================================================

int main ()  // just some main. Don't pay attention.
{
    loop();

    cin.get();
    return 0;
}
