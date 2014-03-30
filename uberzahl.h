#ifndef UBERZAHL_H
#define UBERZAHL_H
#include<iostream>
#include<string>
#include<vector>
//#include<gmpxx.h>

#define smallType unsigned int
#define mediumType unsigned long long
#define largeType unsigned long long

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
