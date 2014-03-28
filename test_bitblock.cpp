#include <iostream>
#include <string>

#include "bitblock.h"

using namespace std;

int main(){
    bitblock block1(10);
    bitblock block2(10);
    bitblock block3(20);

    block1.valSet("1110111011");
    block2.valSet("100110");
    block3.valSet("111111");
    cout << "block1 " << block1 << endl;
    cout << "block2 " << block2 << endl;
    cout << "block3 " << block3 << endl;
    cout << "B1 & B2" << endl;
    cout << (block1 & block2) << endl;
    cout << "B1 ^ B2" << endl;
    cout << (block1 ^ block2) << endl;
    cout << "!B1" << endl;
    cout << (!block1) << endl;
    cout << "!B2" << endl;
    cout << (!block2) << endl;

    cout << "B1 Left Shift 4" << endl;
    cout << (block1 << 4) << endl;


    // bitblock get and set.

    bitblock b(8);
    b.valSet("11111111");
    cout << "The bit string of " << b.getInt64()
         << " is " << b << endl;

    b.valSet(254);
    cout << "The bit string of " << b.getInt64()
         << " is " << b << endl;

    return 0;
}
