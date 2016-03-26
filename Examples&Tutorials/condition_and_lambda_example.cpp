#include <iostream>
using namespace std;

int main()
{
    int x = 0, y = 0;
    auto prnt = [&]{ cout << x << "\t" << y << endl; };
    prnt();

    false && x++;
    true && y++;
    prnt();

    if( false ) x++;
    if( true ) y++;
    prnt();

    false ? x++ : x;
    true ? y++ : y;
    prnt();
    
    // look at the output!
    // !INPORTANT!: resulting machine code is different in all cases so
    // always search for and use the best solution of all.
    cin.get();
}
