simonecb
========

A lightweight block cipher encryption and decryption library 

<b>Created by:</b> Team MDes5 (EECS475)
Johnathan Corkery, John Rabideau, and Zane Salem

[<b>Performance Data</b>](https://docs.google.com/a/umich.edu/spreadsheet/ccc?key=0Apc2HgKn2fAEdGRGSzNQZWJVQWRJYkZVQjdxWmNWZVE&usp=sharing#gid=0)

Background
----------

The SIMON and SPECK Families of Lightweight Block Ciphers come from [this 2013 IACR paper](https://eprint.iacr.org/2013/404)


Compilation
-----------
To compile simonecb, be sure to use a compiler that is C++11 compliant. For example, to compile
the test vector with g++, do the following:

	module load gcc (If on CAEN)
	g++ -std=c++11 simonecb.cpp driver.cpp uberzahl.cpp -o simonecb




Usage
-----
This algorithm is a 128/256 Simon encryption, meaning the encryption takes in
128-bit blocks and a 256-bit key. To run the encryption/decryption, include "simonecb.h" and in a main
function, first call the key generation function, genKey().

	uberzahl key = genKey();


This will produce a random 256-bit key.
Next, create a vector of uberzahls for the plaintext message. Each entry in the
vector should only contain 128 bits.

	vector<uberzahl> plaintext;

Create a vector to store the result.

	vector<uberzahl> encrypted;

Then call the function encrypt(vector<uberzahl> plaintext, uberzahl key);

	encrypted = encrypt(plaintext, key);

To decipher, call decrypt(vector<uberzahl> ciphertext, uberzahl key);

	vector<uberzahl> result = decrypt(encrypted, key);

The test vector provided in the document was as follows:

<b>Key:</b> 1f1e1d1c1b1a1918 1716151413121110 0f0e0d0c0b0a0908 0706050403020100
	
<b>Plaintext:</b> 74206e69206d6f6f 6d69732061207369
	
<b>Ciphertext:</b> 8d2b5579afc8a3a0 3bf72a87efe7b868

To run this example:

	uberzahl key = 0x1f1e1d1c1b1a1918;
	key = key << 64;
	key = key + 0x1716151413121110;
	key = key << 64;
	key = key + 0x0f0e0d0c0b0a0908;
	key = key << 64;
	key = key + 0x0706050403020100;

	vector<uberzahl> msg;
	msg[0] = 0x74206e69206d6f6f;
	msg[0] = msg[0] << 64;
	msg[0] = msg[0] + 0x6d69732061207369;

	vector<uberzahl> result = encrypt(msg, key);

When the result is printed out, it should be the ciphertext listed above.
