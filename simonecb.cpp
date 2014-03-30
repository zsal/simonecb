#include <iostream>
#include <string>
#include <cmath>
#include "uberzahl.h"

using namespace std;

//z constants
uberzahl z[5] = {4506230155203752166, 2575579794259089498, 3160415496042964403, 3957284701066611983, 3781244162168104175};

uberzahl key; //256 bit
uberzahl two = 2;

const int n = 64; //word size
const int m = 4; //# of key words
const int T = 72; //# of rounds
const int j = 4; //const sequence of z

//block size 2n = 128bits


uberzahl k[T];


uberzahl inverse(const uberzahl input) {
	return (two.exp(n) -1) ^ input;
}

void keyExpansion() {

	for (int i = m; i < T; i++){
		uberzahl tmp = k[i-1].rotateRight(3,0,n-1);
		if (m == 4){
			tmp = tmp ^ k[i-3];
		}
		tmp = tmp ^ tmp.rotateRight(1,0,n-1);
		k[i] = inverse(k[i-m]) ^ tmp ^ z[j].bit((i-m) % 62) ^ 3;
	}

}

pair<uberzahl, uberzahl> encrypt(uberzahl x, uberzahl y) {
		//Set x to first half of message

	/*while(message isn't done){
		uberzahl x = first half of block
		uberzahl y = second half of block
		for (int i = 0; i < T; i++){
			uberzahl tmp = x;
			x = y ^ (x.rotateLeft(1,0,n-1) & x.rotateLeft(8,0,n-1)) ^ x.rotateLeft(2,0,n-1) ^ k[i]
			y = tmp;
		}
		output x,y
	}
	*/


	for (int i = 0; i < T; i++){
		uberzahl tmp = x;
		x = y ^ (x.rotateLeft(1,0,n-1) & x.rotateLeft(8,0,n-1)) ^ x.rotateLeft(2,0,n-1) ^ k[i];
		y = tmp;
	}
	cout << "x: " << x << endl;
	cout << "y: " << y << endl;

	return make_pair(x,y);

}

pair<uberzahl, uberzahl> decrypt(uberzahl x, uberzahl y) {


	/*For decryption:
	while(message isn't done){
		uberzahl x = first half of block
		uberzahl y = second half of block
		for(int i = T-1; i >= 0; i--){
			uberzahl tmp = y;
			y = x ^ (y.rotateLeft(1,0,n-1) & y.rotateLeft(8,0,n-1)) ^ y.rotateLeft(2,0,n-1) ^ k[i]
			x = tmp;
		}
		output x,y
	}*/


	for(int i = T-1; i >= 0; i--){
		uberzahl tmp = y;
		y = x ^ (y.rotateLeft(1,0,n-1) & y.rotateLeft(8,0,n-1)) ^ y.rotateLeft(2,0,n-1) ^ k[i];
		x = tmp;
	}

	cout << "x: " << x << endl;
	cout << "y: " << y << endl;

	return make_pair(x,y);


}


int main() {

	//big number stuff
	key = 0x8a3f93d142d7ef30;
	key = key << 64;
	key = key + 0xa3f93d142d7ef308;
	key = key << 64;
	key = key + 0x3f93d142d7ef308a;
	key = key << 64;
	key = key + 0xf93d142d7ef308a3;

	//initial key secret sharing
	for (int i = m-1; i >=0; i--){
		int start = i*n;
		int end = start + n - 1;
		int shift = i*n;
		k[m-1-i] = key.extract(start, end) >> shift;
	}


	//todo: break up message into blocks
	//		blocks into x,y


	uberzahl x = "180849305"; //first half of block (n=64 bits)
	uberzahl y = "534737354"; //last half of block (n=64 bits)

	pair<uberzahl,uberzahl> e = encrypt(x,y);
	pair<uberzahl, uberzahl> d = decrypt(e.first, e.second);




}


void print_test(uberzahl key) {

	for (int i = 0; i < 5; i++){
		for (int j = 61; j >= 0; j--){
			//std::cout << z[i].bit(j);
		}
		//std::cout << std::endl;
	}

	
	/*for (int j = 61; j >=0; j--){
		std::cout << x.bit(j);
	}
	std::cout << std::endl;*/


	/*for (int i = 0; i < m; i++){
		cout << i << " ";
		for (int j = k[i].bitLength()-1; j >= 0; j--){

			//std::cout << k[i].bit(j);
		}
		//std::cout << std::endl;
	}*/
	//k[0] = 


		cout<<"key"<<key<<endl;
	for (int i = n*m-1; i >= 0 ; i--){
		cout << key.bit(i);
	}
	cout << endl;	

}

