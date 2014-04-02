#include "simonecb.h"

using namespace std;




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


}

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

		//uberzahl tmp = k[i-m+3].rotateRight(3,0,n-1) ^ k[i-m+1];
		//k[i] = c ^ z[j].bit(61 - ((i-m) % 62)) ^ k[i-m] ^ tmp ^ tmp.rotateRight(1,0,n-1);
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

		//uberzahl tmp = k[i-m+3].rotateRight(3,0,n-1) ^ k[i-m+1];
		//k[i] = c ^ z[j].bit(61 - ((i-m) % 62)) ^ k[i-m] ^ tmp ^ tmp.rotateRight(1,0,n-1);
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



