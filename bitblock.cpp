#include <iostream>
#include "bitblock.h"
#include <cmath>


using namespace std;

bitblock::bitblock ( int numBits ){
	resize(numBits);
}

void bitblock::resize(int newSize) {
    bits.resize(newSize, 0);
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
	if(spot >= bits.size()){

		spot = spot - bits.size();
	}
   	 retval.bits[spot] = bits[i];
    }
    return retval;
}

long long bitblock::getInt64() const {
	long long out    = 0;
	int size         = bits.size();
	int maxBitLength = sizeof(long long)*8; 

	if (size <= 64) {
		for(int i = 0; i < size; ++i) {
			if (bits[size - 1 - i])
				out += (1 << i);
		}
	}	

	return out;
}



void bitblock::valSet(long long in) {
	int    size = sizeof(long long)*8;
	string bstr = "";
	while(in) {
		if (in % 2) {
			bstr = '1' + bstr;
		} else {
			bstr = '0' + bstr;
		}
		in /= 2;
	}

	bits.clear();
	resize(bstr.size());	
	(*this).valSet(bstr.c_str());
}
