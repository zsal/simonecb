#include <iostream>
#include "bitblock.h"

using namespace std;

bitblock::bitblock ( int numBits ){
    bits.resize(numBits, 0);
}

std::ostream& operator << (std::ostream& os, const bitblock& block ) {
    for (int i = 0; i < block.bits.size(); i++){
        os << block.bits[i];
    }
    return os;
}

void bitblock::valSet( const char * input ){
    int i = 0;
    while (i < bits.size() and input[i]){
        if(input[i] == '1'){
            bits[i] = 1;
        }
        else{
            bits[i] = 0;
        }
        i++;
    }
    while(i < bits.size()){
        bits[i] = 0;
        i++;
    }
}

bitblock bitblock::operator & (const bitblock& right) const{
    bitblock left = *this;
    //assert(left.bits.size() == right.bits.size());
    bitblock retval(left.bits.size());
    for(int i = 0; i < left.bits.size(); i++){
        retval.bits[i] = left.bits[i] & right.bits[i];
    }
    return retval;
}

bitblock bitblock::operator ^ (const bitblock& right) const{
    bitblock left = *this;
    //assert(left.bits.size() == block.bits.size());
    bitblock retval(left.bits.size());
    for(int i = 0; i < left.bits.size(); i++){
        retval.bits[i] = left.bits[i] ^ right.bits[i];
    }
    return retval;
}

bitblock bitblock::operator ! ( void ) const{
    bitblock retval(bits.size());
    for(int i = 0; i < retval.bits.size(); i++){
        retval.bits[i] = !bits[i];
    }
    return retval;

}

bitblock bitblock::operator << (const int shift) const{
    bitblock retval(bits.size());
    for(int i = 0; i < retval.bits.size(); i++){
        int spot = i - shift;
        if(spot < 0){
            spot = spot + bits.size();
        }
        retval.bits[spot] = bits[i];
    }
    return retval;
}