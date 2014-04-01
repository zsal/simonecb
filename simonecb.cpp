#include <iostream>
#include <string>
#include <cmath>
#include <cassert>
#include "uberzahl.h"

using namespace std;

//z constants
uberzahl z[5] = {4506230155203752166, 2575579794259089498, 3160415496042964403, 3957284701066611983, 3781244162168104175};

uberzahl key; //256 bit
uberzahl one = 1;
uberzahl two = 2;


const unsigned int n = 64; //word size
const unsigned int m = 4; //# of key words
const unsigned int T = 72; //# of rounds
const unsigned int j = 4; //const sequence of z

uberzahl test_plaintext_msg, test_ciphertext_msg;

//block size 2n = 128bits

uberzahl k[T];

uberzahl inverse(const uberzahl input) {
	return ((one << n) - 1) ^ input;
}

void keyExpansion() {

	uberzahl c = two.exp(n) - 4;

	for (int i = m; i < T; i++){
		uberzahl tmp = c ^ k[i-1].rotateRight(3,0,n-1);
		if (m == 4){
			tmp = tmp ^ k[i-3];
		}
		tmp = tmp ^ tmp.rotateRight(1,0,n-1);
		k[i] = inverse(k[i-m]) ^ tmp ^ z[j].bit((i-m) % 62) ^ 3;
	}

}

uberzahl encrypt(uberzahl msg) {
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
	uberzahl x = msg >> 64; //first half
	uberzahl y = msg & ((one << 64) - 1); //second half


	for (int i = 0; i < T; i++){
		uberzahl tmp = x;
		x = y ^ (x.rotateLeft(1,0,n-1) & x.rotateLeft(8,0,n-1)) ^ x.rotateLeft(2,0,n-1) ^ k[i];
		y = tmp;
	}
	//cout << "x: " << x << endl;
	//cout << "y: " << y << endl;

	uberzahl cipher_msg = y + (x << 64);

	cout << "cipher msg: " << cipher_msg << endl;

	return cipher_msg;

}

uberzahl decrypt(uberzahl cipher_msg) {


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

	uberzahl x = cipher_msg >> 64; //first half
	uberzahl y = cipher_msg & ((one << 64) - 1); //second half


	for(int i = T-1; i >= 0; i--){
		uberzahl tmp = y;
		y = x ^ (y.rotateLeft(1,0,n-1) & y.rotateLeft(8,0,n-1)) ^ y.rotateLeft(2,0,n-1) ^ k[i];
		x = tmp;
	}

	//cout << "x: " << x << endl;
	//cout << "y: " << y << endl;

	uberzahl msg = y + (x << 64); 
	cout << "msg: " << msg <<endl;


	return msg;


}


int main() {
	/*
	//big number stuff
	key = 0x8a3f93d142d7ef30;
	key = key << 64;
	key = key + 0xa3f93d142d7ef308;
	key = key << 64;
	key = key + 0x3f93d142d7ef308a;
	key = key << 64;
	key = key + 0xf93d142d7ef308a3;
	*/

	//test vector key:
	//1f1e1d1c1b1a1918 1716151413121110 0f0e0d0c0b0a0908 0706050403020100
	key = 0x1f1e1d1c1b1a1918;
	key = key << 64;
	key = key + 0x1716151413121110;
	key = key << 64;
	key = key + 0x0f0e0d0c0b0a0908;
	key = key << 64;
	key = key + 0x0706050403020100;

	test_plaintext_msg = 0x74206e69206d6f6f;
	test_plaintext_msg = test_plaintext_msg << 64;
	test_plaintext_msg = test_plaintext_msg + 0x6d69732061207369;
	test_ciphertext_msg = 0x8d2b5579afc8a3a0;
	test_ciphertext_msg = test_ciphertext_msg << 64;
	test_ciphertext_msg = test_ciphertext_msg + 0x3bf72a87efe7b868;

	cout << "test P:" << test_plaintext_msg <<endl;
	cout << "test C:" << test_ciphertext_msg <<endl;



	//initial key secret sharing
	for (int i = m-1; i >=0; i--){
		int start = i*n;
		int end = start + n - 1;
		int shift = i*n;
		k[m-1-i] = key.extract(start, end) >> shift;
	}

	//assert(test_ciphertext_msg == encrypt(test_plaintext_msg));
	//assert(test_plaintext_msg == decrypt(test_ciphertext_msg));
	encrypt(test_plaintext_msg);
	decrypt(test_ciphertext_msg);

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

