#include "simonecb.h"



using namespace std;


uberzahl key; //256 bit initially


//block size = 2n = 128bits


uberzahl test_plaintext_msg, test_ciphertext_msg;




int main() {
	//test vector values for key, plaintext, ciphertext:
	key = 0x1f1e1d1c1b1a1918;
	key = key << 64;
	key = key + 0x1716151413121110;
	key = key << 64;
	key = key + 0x0f0e0d0c0b0a0908;
	key = key << 64;
	key = key + 0x0706050403020100;


	// Expected values
	test_plaintext_msg = 0x74206e69206d6f6f;
	test_plaintext_msg = test_plaintext_msg << 64;
	test_plaintext_msg = test_plaintext_msg + 0x6d69732061207369;

	test_ciphertext_msg = 0x8d2b5579afc8a3a0;
	test_ciphertext_msg = test_ciphertext_msg << 64;
	test_ciphertext_msg = test_ciphertext_msg + 0x3bf72a87efe7b868;


	cout << "key         = " << getHex(key) << endl; 

    vector<uberzahl> plaintext;
    plaintext.push_back(test_plaintext_msg);
    plaintext.push_back(test_plaintext_msg);
    
    
	vector<uberzahl> result = encrypt(plaintext, key);
	cout << "Input: \t\t\t" << getHex(test_plaintext_msg) << endl;
	cout << "Encrypted(0): \t\t" << getHex(result[0]) << endl;
    cout << "Encrypted(1): \t\t" << getHex(result[1]) << endl;
	cout << "Expected Encrypted: \t" << getHex(test_ciphertext_msg) << endl;
	cout << "Decrypted(0): \t\t" << getHex(decrypt(result, key)[0]) << endl;
    cout << "Decrypted(1): \t\t" << getHex(decrypt(result, key)[1]) << endl;

}
