#include <iostream>
#include <string>
#include <cmath>
#include <cassert>
#include "uberzahl.h"

using namespace std;

//z constants
uberzahl z[5] = {4506230155203752166, 2575579794259089498, 3160415496042964403, 3957284701066611983, 3781244162168104175};

uberzahl key; //256 bit initially
uberzahl one = 1;
uberzahl two = 2;

//block size = 2n = 128bits
const unsigned int n = 64; //word size
const unsigned int m = 4; //# of key words
const unsigned int T = 72; //# of rounds
const unsigned int j = 4; //const sequence of z

uberzahl test_plaintext_msg, test_ciphertext_msg;
uberzahl k[T];

uberzahl notOperation(const uberzahl input) {
	return ((one << n) - 1) ^ input;
}

void printB(const uberzahl input, int numBits){
	for(int i = numBits - 1; i >= 0; i--){
		cout << input.bit(i);
		if(i%4 == 0){
			cout << " ";
		}
	}
	cout << endl;
}

void keyExpansion() {
	uberzahl c = two.exp(n) - 4;

	for (int i = m; i < T; i++){

		uberzahl tmp = k[i-1].rotateRight(3,0,n-1);
		if (m == 4){
			tmp = tmp ^ k[i-3];
	 	}
		tmp = tmp ^ tmp.rotateRight(1,0,n-1);
		k[i] = notOperation(k[i-m]) ^ tmp ^ z[j].bit(61 - ((i-m) % 62)) ^ 3;

		//uberzahl tmp = k[i-m+3].rotateRight(3,0,n-1) ^ k[i-m+1];
		//k[i] = c ^ z[j].bit(61 - ((i-m) % 62)) ^ k[i-m] ^ tmp ^ tmp.rotateRight(1,0,n-1);
	}

}

uberzahl encrypt(uberzahl msg) {
	uberzahl x = msg >> n; //first half
	uberzahl y = msg & ((one << n) - 1); //second half

	for (int i = 0; i < T; i++){
		uberzahl tmp = x;
		x = y ^ ((x.rotateLeft(1,0,n-1) & x.rotateLeft(8,0,n-1)) ^ x.rotateLeft(2,0,n-1)) ^ k[i];
		y = tmp;
	}

	uberzahl cipher_msg = y + (x << n);

	return cipher_msg;
}

uberzahl decrypt(uberzahl cipher_msg) {
	uberzahl x = cipher_msg >> n; //first half
	uberzahl y = cipher_msg & ((one << n) - 1); //second half

	for(int i = T-1; i >= 0; i--){
		uberzahl tmp = y;
		y = x ^ ((y.rotateLeft(1,0,n-1) & y.rotateLeft(8,0,n-1)) ^ y.rotateLeft(2,0,n-1)) ^ k[i];
		x = tmp;
	}

	uberzahl msg = y + (x << n); 

	return msg;
}



string getHex(uberzahl u) {	
	string out = "";
	while(u > uberzahl("0")) {
		char hex = ((char)((u % 16)));
		if (hex <= 9)
			hex += '0';
		else
			hex += 'a' - 10;
		out = hex + out; 
		u = u / 16;
	}
	return out;
}


int main() {
	//test vector values for key, plaintext, ciphertext:
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

	//initial key secret sharing
	for (int i = m-1; i >=0; i--){
		int start = i*n;
		int end = start + n - 1;
		int shift = i*n;
		k[i] = key.extract(start, end) >> shift;
	}

	keyExpansion();

	uberzahl result = encrypt(test_plaintext_msg);
	cout << "Input: \t\t\t" << getHex(test_plaintext_msg) << endl;
	cout << "Encrypted: \t\t" << getHex(result) << endl;
	cout << "Expected Encrypted: \t" << getHex(test_ciphertext_msg) << endl;
	cout << "Decrypted: \t\t" << getHex(decrypt(result)) << endl;

}