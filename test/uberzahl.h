#ifndef UBERZAHL_H
#define UBERZAHL_H
#include<iostream>
#include<string>
#include<vector>
#include<climits>
//#include<gmpxx.h>

// CONFIGURATIONS :
// smallType is used for storage -- uberzahl.value[i]
//    * Adjusting this will adjust the memory efficiency
//    * For most efficient use of memory use your processor's word size
//  mediumType and largeType should be capable of holding 2x MAX(smallType)
//    * These are used mainly for bit manipulation
#define smallType unsigned int
#define mediumType unsigned long long
#define largeType unsigned long long

// maxBits controls how many bits to use our of your smallType container
// you must ensure that maxBits <= BITLEN(smallType)
#define maxBits 32

// mask is used to truncate numbers to the correct bitlength
// mask must be a value of all 1's maxBits long
#define mask 0xffffffffL

// RAND_MAX can be different between compilers and so it is handled here
// If your RAND_MAX is different from below (not 16, 32, 64 bit) you should add it below
// and compile with the correct RAND_MAX length
// ( specifying a smaller RAND_MAX than you have will result in a non-uniform distribution
#if RAND_MAX == 32767
  #define RAND_BITS 16
#elif RAND_MAX == 2147483647
  #define RAND_BITS 32
#else
  #define RAND_BITS 64
#endif

// UBERZAHL CLASS DEFINITION :
class uberzahl {
  public:
    uberzahl ( void );
    ~uberzahl ( void );
    uberzahl ( const char* );
    uberzahl ( largeType );
    uberzahl ( const uberzahl& );
//    uberzahl( const mpz_class& );
    const uberzahl& operator = ( const uberzahl& );

    // uses the rand function - to seed use srand (unsigned int seed);
    uberzahl random ( mediumType );
    friend std::ostream& operator << ( std::ostream&, const uberzahl& );

    // standard arithmetic operations
    uberzahl operator + ( const uberzahl& ) const;
    uberzahl operator - ( const uberzahl& ) const;
    uberzahl operator * ( const uberzahl& ) const;
    uberzahl operator / ( const uberzahl& ) const;
    uberzahl operator / ( smallType ) const;
    uberzahl operator % ( const uberzahl& ) const;
    smallType operator % ( smallType ) const;
    uberzahl exp ( const uberzahl& ) const;
    uberzahl expm( const uberzahl&, const uberzahl& ) const;

    // standard numeric comparators
    bool operator > ( const uberzahl& ) const;
    bool operator < ( const uberzahl& ) const;
    bool operator >= ( const uberzahl& ) const;
    bool operator <= ( const uberzahl& ) const;
    bool operator == ( const uberzahl& ) const;
//    bool operator == ( const mpz_class& ) const;
    bool operator != ( const uberzahl& ) const;

    // standard c/c++ bitwize operators
    uberzahl operator | ( const uberzahl& ) const;
    uberzahl operator & ( const uberzahl& ) const;
    uberzahl operator ^ ( const uberzahl& ) const;
    uberzahl operator >> ( smallType ) const;
    uberzahl operator << ( smallType ) const;

    uberzahl extract ( smallType, smallType );
    uberzahl rotateLeft ( smallType, smallType, smallType );
    uberzahl rotateRight ( smallType, smallType, smallType );
    void setBit ( smallType );
    void clearBit ( smallType );
    void toggleBit ( smallType );

    // auxilary functions
    uberzahl inverse ( const uberzahl& ) const;
    uberzahl gcd ( const uberzahl& ) const;

    smallType bit ( mediumType ) const;
    smallType bitLength ( void ) const;

  private:
    std::vector<smallType> value;
    void convert_to_numeric ( const char* );
    std::string convert_to_string ( void ) const;
    void clean_bits ( void );
    std::pair<std::pair<uberzahl,uberzahl>,bool> inverse ( const uberzahl&, const uberzahl& ) const;
};

uberzahl random ( const uberzahl&, const uberzahl& );
bool rabinmiller ( const uberzahl&, unsigned int );
uberzahl nextprime ( const uberzahl&, unsigned int );

#endif