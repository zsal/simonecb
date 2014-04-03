#ifndef H_SIMONECB_INCLUDED
#define H_SIMONECB_INCLUDED

#include <iostream>
#include <string>
#include <cmath>
#include <cassert>
#include <vector>
#include "uberzahl.h"

const unsigned int n = 64; //word size
const unsigned int m = 4; //# of key words
const unsigned int T = 72; //# of rounds
const unsigned int j = 4; //const sequence of z

const uberzahl one  = "1";
const uberzahl two  = "2";
const uberzahl zero = "0";




// z onstant
const uberzahl z[5] = {4506230155203752166,
                       2575579794259089498,
                       3160415496042964403, 
                       3957284701066611983, 
                       3781244162168104175};



/* utility */
uberzahl notOperation(const uberzahl input);
void printB(const uberzahl input, int numBits);
std::string getHex(uberzahl u);





/* Enc / Dec */
uberzahl genKey();

void keyExpansion();

std::vector<uberzahl> encrypt(std::vector<uberzahl> msg, uberzahl key);
std::vector<uberzahl> decrypt(std::vector<uberzahl> cipher_msg, uberzahl key);


#endif
