#include "simonecb.h"
#include <random>
#include <ctime>

const int KEY_SIZE_BITS = 256;

using namespace std;

/* crypto functions */
vector<uberzahl> encrypt(vector<uberzahl> msg, uberzahl key) {

    uberzahl k[T];

    //initial key secret sharing
    for (int i = m-1; i >=0; i--){
		int start = i*n;
		int end = start + n - 1;
		int shift = i*n;
		k[i] = key.extract(start, end) >> shift;
	}

    


    /* key expansion */
	uberzahl c = two.exp(n) - 4;

	for (int i = m; i < T; i++){

		uberzahl tmp = k[i-1].rotateRight(3,0,n-1);
		if (m == 4){
			tmp = tmp ^ k[i-3];
	 	}
		tmp = tmp ^ tmp.rotateRight(1,0,n-1);
		k[i] = notOperation(k[i-m]) ^ tmp ^ z[j].bit(61 - ((i-m) % 62)) ^ 3;


	}

    vector<uberzahl> ciphertext;

    int size = msg.size();
    for(int l = 0; l < size; ++l) {

	uberzahl x = msg[l] >> n; //first half
	uberzahl y = msg[l] & ((one << n) - 1); //second half
	
	for (int i = 0; i < T; i++){
	    uberzahl tmp = x;
	    x = y ^ ((x.rotateLeft(1,0,n-1) & x.rotateLeft(8,0,n-1)) ^ x.rotateLeft(2,0,n-1)) ^ k[i];
	    y = tmp;
	}
	
	uberzahl cipher_msg = y + (x << n);
        ciphertext.push_back(cipher_msg);
    }

    return ciphertext;
}

vector<uberzahl> decrypt(vector<uberzahl> cipher_msg, uberzahl key) {

    uberzahl k[T];

    //initial key secret sharing
    for (int i = m-1; i >=0; i--){
		int start = i*n;
		int end = start + n - 1;
		int shift = i*n;
		k[i] = key.extract(start, end) >> shift;
	}


    /* key expansion */
	uberzahl c = two.exp(n) - 4;

	for (int i = m; i < T; i++){

		uberzahl tmp = k[i-1].rotateRight(3,0,n-1);
		if (m == 4){
			tmp = tmp ^ k[i-3];
	 	}
		tmp = tmp ^ tmp.rotateRight(1,0,n-1);
		k[i] = notOperation(k[i-m]) ^ tmp ^ z[j].bit(61 - ((i-m) % 62)) ^ 3;


	}


    
    vector<uberzahl> plaintext;
    int size = cipher_msg.size();
    for(int l = 0; l < size; ++l) {
	    uberzahl x = cipher_msg[l] >> n; //first half
	    uberzahl y = cipher_msg[l] & ((one << n) - 1); //second half


	    for(int i = T-1; i >= 0; i--){
		    uberzahl tmp = y;
		    y = x ^ ((y.rotateLeft(1,0,n-1) & y.rotateLeft(8,0,n-1)) ^ y.rotateLeft(2,0,n-1)) ^ k[i];
		    x = tmp;
	    }

	    uberzahl msg = y + (x << n); 
        plaintext.push_back(msg);
    }

	return plaintext;
}

/* helper functions */
uberzahl genKey() {
	unsigned long n = KEY_SIZE_BITS;	

	default_random_engine RNG;
	RNG.seed(time(NULL));
	bernoulli_distribution bTrial(.5);

	uberzahl key = "0";
	for(unsigned long i = 0; i < n; i += 1) {
		if (bTrial(RNG))
			key.setBit(i);
	}

	return key;
}

uberzahl notOperation(const uberzahl input) {
	return ((one << n) - 1) ^ input;
}

/* debugging functions */
void printB(const uberzahl input, int numBits){
	for(int i = numBits - 1; i >= 0; i--){
		cout << input.bit(i);
		if(i%4 == 0){
			cout << " ";
		}
	}
	cout << endl;
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
