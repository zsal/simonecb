#include<iostream>
#include<string>
#include<vector>
#include<assert.h>
#include<cstdlib>
#include<climits>
#include"uberzahl.h"

// configuration options :
// smallType is the storage for uberzahl -- adjust this to adjust performance
// UBERZAHL_TYPE_MAX, maxBits, and mask should match smallType
// mediumType and largeType should be at least 2x the storage of small
#define largeType unsigned long long
#define mediumType unsigned long long
#define smallType unsigned int
#define UBERZAHL_TYPE_MAX UINT_MAX
#define maxBits 32
#define mask 0xffffffffL

// adjust for some different compiler RAND functions
#if RAND_MAX == 32767
  #define RAND_BITS 16
#elif RAND_MAX == 2147483647
  #define RAND_BITS 32
#else
  #define RAND_BITS 64
#endif

uberzahl::uberzahl ( void ){
  convert_to_numeric("0");
}

uberzahl::uberzahl ( largeType number ){
  value.push_back(number);
  while ( number != 0 ){
    number = number >> maxBits;
    value.push_back(number);
  }
}

uberzahl::uberzahl ( const char* number ){
  convert_to_numeric( number );
}
/*
uberzahl::uberzahl(const mpz_class& number){
	mpz_class scale = mask+1;
	mpz_class current = number;
	while(current>0) {
		mpz_class t = current%scale;
		value.push_back(t.get_ui());
		current/=scale;
	}
}*/

uberzahl::uberzahl ( const uberzahl& number ){
  for ( size_t i=0; i < number.value.size(); ++i )
    value.push_back( number.value[i] );
  clean_bits();
}

uberzahl::~uberzahl ( void )
{ ;;; }

const uberzahl& uberzahl::operator = ( const uberzahl& number )
{
  if ( this == &number ) return *this;
  value.clear();
  for ( size_t i=0; i < number.value.size(); ++i )
    value.push_back( number.value[i] );
  clean_bits();
  return *this;
}

void uberzahl::clean_bits ( void ){
  // remove leading 0's
  while ( value.size() > 1 && !value.back() )
    value.pop_back();
}


uberzahl uberzahl::operator << ( smallType shift ) const
{
  // binary left shift
  uberzahl retval;
  retval.value.pop_back();
  smallType largeshift = shift / maxBits;
  smallType smallshift = shift % maxBits;

  for ( size_t i=0; i < largeshift + value.size() + 1; ++i )
    retval.value.push_back(0);

  for ( size_t i=0; i < value.size(); ++i )
    retval.value[i+largeshift] = (value[i] << smallshift)&mask;
  for ( size_t i=0; i < value.size(); ++i ){
    mediumType workspace = value[i];
    workspace = workspace >> ( maxBits - smallshift );
    retval.value[i+largeshift+1] += workspace;
  }

  retval.clean_bits();
  return retval;
}

uberzahl uberzahl::operator >> ( smallType shift ) const
{
  // binary right shift
  uberzahl retval;
  smallType largeshift = shift / maxBits;
  smallType smallshift = shift % maxBits;
  for ( size_t i=0; i + largeshift + 1 < value.size(); ++i )
    retval.value.push_back(0);

  for ( size_t i=0; i + largeshift < value.size(); ++i )
    retval.value[i] = value[i + largeshift] >> smallshift;
  for ( size_t i=0; i + largeshift + 1 < value.size(); ++i ){
    mediumType workspace = value[i + largeshift + 1];
    workspace = workspace << ( maxBits - smallshift );
    retval.value[i] += workspace&mask;
  }

  retval.clean_bits();
  return retval;
}

uberzahl uberzahl::operator + ( const uberzahl& input ) const
{
  uberzahl x = *this;
  uberzahl y = input;
  mediumType workbench = 0;
  uberzahl retval = "0";
  retval.value.clear();

  // pad extra zeros onto the left of the smaller
  while ( x.value.size() != y.value.size() )
    if ( x.value.size() > y.value.size() )
      y.value.push_back(0);
    else
      x.value.push_back(0);

  // perform addition operation
  for ( size_t i = 0; i < x.value.size(); ++i ){
    workbench = workbench + x.value[i] + y.value[i];
    retval.value.push_back(workbench&mask);
    workbench = workbench >> maxBits;
  }

  // add carry bit
  retval.value.push_back(workbench);
  retval.clean_bits();
  return retval;
}

uberzahl uberzahl::operator - ( const uberzahl& input ) const
{
  uberzahl x = *this;
  uberzahl y = input;
  mediumType workbench = 0;
  uberzahl retval = "0";

  // constraint that left side !< right side
  if ( x < y ) return retval;
  retval.value.clear();

  // pad extra zeros onto the left of the smaller
  while ( x.value.size() != y.value.size() )
    if ( x.value.size() > y.value.size() )
      y.value.push_back(0);
    else
      x.value.push_back(0);

  // perform subtraction
  for ( size_t i = 0; i < x.value.size(); ++i ){
    workbench = x.value[i] - y.value[i] - workbench;
    retval.value.push_back(workbench&mask);
    workbench = workbench >> maxBits;
    if ( workbench ) workbench = 1;
  }

  retval.clean_bits();
  return retval;
}

uberzahl uberzahl::operator * ( const uberzahl& y ) const
{
  size_t n = value.size() - 1;
  size_t t = y.value.size() - 1;
  uberzahl retval = "0";
  retval.value.clear();

  smallType carry = 0;
  largeType workbench = 0;

  for ( size_t i = 0; i <= n + t + 1; ++i )
    retval.value.push_back(0);
  for ( size_t i = 0; i <= t; ++i ){
    carry = 0;
    for ( size_t j = 0; j <= n; ++ j ){
      workbench = retval.value[i+j] + ((largeType) value[j])*y.value[i] + carry;
      retval.value[i+j] = workbench & ((1ULL<<maxBits)-1);
      carry = workbench >> maxBits;
    }
	retval.value[n+i+1] += carry;
  }
  retval.clean_bits();
  return retval;
}

uberzahl uberzahl::operator / ( const uberzahl& number ) const
{
	uberzahl x = *this;
	uberzahl y = number;
	uberzahl q = 0ULL;
	assert( y != "0" ); // y can not be 0 in our division algorithm
	if ( x < y ) return q; // return 0 since y > x
	x.clean_bits();
  y.clean_bits();
	size_t n = x.value.size() - 1;
	size_t t = y.value.size() - 1;

	// initialize q to the correct size
	for ( size_t i = 0; i < n - t; ++i )
		q.value.push_back(0);
	y = y << (maxBits*(n - t + 1));
	for( size_t i = maxBits*t; i < maxBits*(n + 1); ++i ){
		y = y >> 1;
		q = q << 1;
		if( x >= y ){
			x = x - y;
			q = q + 1;
		}
	}
	return q;
/*
  uberzahl x = *this;
  uberzahl y = number;
  uberzahl q = "0";

  assert( y != "0" ); // y can not be 0 in our division algorithm
  if ( x < y ) return q; // return 0 since y > x

  x.clean_bits();
  x.value.push_back(0);
  y.clean_bits();

  size_t n = x.value.size() - 1;
  size_t t = y.value.size() - 1;

  // step 1 -- initialize q to the correct size
  for ( size_t i = 0; i < n - t - 1; ++i )
    q.value.push_back(0);

  // step 2 -- begin long division on first 2^16-digit
  for ( size_t i=n; i > t; --i ){
//    x.value.push_back(0);
	if(i+1>x.value.size())
		continue;
    largeType workbench = x.value[i];
    workbench = workbench << maxBits;
    workbench = workbench + x.value[i-1];
    // workbench = x[i]B + x[i-1]

    q.value[i-t-1] = workbench / y.value[t];
    while ( x < (y << ((i-t-1)*maxBits))*q.value[i-t-1] )
      q.value[i-t-1] = q.value[i-t-1] - 1;
    x = x - (y << ((i-t-1)*maxBits))*q.value[i-t-1];
  }

  q.clean_bits();
  return q;*/
}


uberzahl uberzahl::operator % ( const uberzahl& number ) const
{
  uberzahl retval = *this - number*( *this / number );
  retval.clean_bits();
  return retval;
}

uberzahl uberzahl::operator / (smallType divisor) const
{
  uberzahl retval = *this;
  largeType current = 0;
  for(size_t i=value.size(); i > 0; --i) {
    current <<= maxBits;
    current+=value[i-1];
    retval.value[i-1] = current/divisor;
    current %=divisor;
  }
  retval.clean_bits();
  return retval;
}

smallType uberzahl::operator % (smallType modulus) const
{
  largeType retval = 0;
  largeType coefficient = 1;
  for(size_t i=0; i<value.size(); ++i) {
    retval+=coefficient*value[i];
    retval%=modulus;
    coefficient<<=maxBits;
    coefficient%=modulus;
  }
  return retval;
}

// convert the stored numeric_value into a string
std::string uberzahl::convert_to_string ( void ) const
{
  if(*this == "0")
    return "0";

  uberzahl temp = *this;
  std::string reversed = "";
  while(temp>"0") {
    reversed+=temp%10+'0';
    temp=temp/10;
  }

  std::string retval = "";
  for(size_t i=reversed.size(); i>0; --i)
    retval+=reversed[i-1];

  return retval;
}

// take the string_value and convert it into a numeric_value
void uberzahl::convert_to_numeric ( const char* string_value ){
  std::string workbench = string_value;
  value.clear();
  smallType numeric_value = 0;
  size_t bits = 0;

  while ( workbench.length() ){
    ++bits;

    size_t len = workbench.length();
    numeric_value = numeric_value >> 1;
    if ( workbench[len-1] % 2 == 1 ){
      numeric_value = numeric_value | ( 1 << (maxBits-1) );
      workbench[len-1] = workbench[len-1] ^ 1;
    }

    for ( size_t i = workbench.length(); i > 0; --i ){
      // constant consistancy check! if this algorithm fails I want to know
      assert( workbench[i-1] >= '0' && workbench[i-1] <= '9' );

      if ( workbench[i-1] % 2 ){ // odd decimal digit
        workbench[i] += 0x05; // adjust the lower term up
      }
      workbench[i-1] -= '0'; // cut off 0x30 ascii
      workbench[i-1] = workbench[i-1] >> 1; // divide by 2
      workbench[i-1] += '0'; // add back the ascii
    }

    while ( workbench[0] == '0' )
      workbench = workbench.substr(1,len);

    // we have run out of room in numeric_value
    if ( bits % maxBits == 0 ){
      value.push_back(numeric_value);
      numeric_value = 0;
    }
  }

  // take care of the remaining bits
  numeric_value = numeric_value >> (maxBits - (bits % maxBits));
  value.push_back(numeric_value);
}

std::ostream& operator << ( std::ostream& ost, const uberzahl& number ){
  /*  ost << "string : " << number.convert_to_string() << std::endl << "base-2**" << maxBits << " : ";
      for ( size_t i = 0; i < number.value.size(); ++ i )
      ost << number.value[i] << ' ';
      ost << std::endl;
      */
  ost << number.convert_to_string();
  return ost;
}

// Comparator operators
// Removed the padding necessity to allow them to be 
// true const& passes
bool uberzahl::operator <= (const uberzahl& rhs) const
{
  size_t lhs_size = value.size();
  size_t rhs_size = rhs.value.size();
  if ( lhs_size < rhs_size )
    return rhs >= *this;

  for ( size_t i=rhs_size; i < lhs_size; ++i )
    if ( value[i] > 0 )
      return false;

  for ( size_t i=rhs_size; i > 0; --i )
    if ( value[i-1] > rhs.value[i-1] )
      return false;
    else if ( value[i-1] < rhs.value[i-1] )
      return true;

  return true; 
}

bool uberzahl::operator >= (const uberzahl& rhs) const
{
  size_t lhs_size = value.size();
  size_t rhs_size = rhs.value.size();
  if ( lhs_size < rhs_size )
    return rhs <= *this;

  for ( size_t i=rhs_size; i < lhs_size; ++i )
    if ( value[i] > 0 )
      return true;

  for ( size_t i=rhs_size; i > 0; --i )
    if ( value[i-1] < rhs.value[i-1] )
      return false;
    else if ( value[i-1] > rhs.value[i-1] )
      return true;

  return true; 
}

bool uberzahl::operator < ( const uberzahl& rhs ) const
{
  return !( *this >= rhs );
}

bool uberzahl::operator > ( const uberzahl& rhs ) const
{
  return !( *this <= rhs );
}

bool uberzahl::operator == ( const uberzahl& rhs ) const
{
  return ( *this >= rhs ) && ( *this <= rhs );
}

/*bool uberzahl::operator == ( const mpz_class& rhs ) const
{
	mpz_class scale= mask+1;
	mpz_class rhstemp = rhs;
	for(size_t i=0; i<value.size(); ++i){
		if(rhstemp%scale != value[i])
			return false;
		rhstemp/=scale;
  }	
  return rhstemp == 0;
}*/


bool uberzahl::operator != ( const uberzahl& rhs ) const
{
  return !( *this == rhs );
}

// Bitwize operators done with true pass by reference
uberzahl uberzahl::operator | ( const uberzahl& rhs ) const
{
  if ( value.size() > rhs.value.size() )
    return rhs | *this;

  uberzahl retval = "0";
  retval.value.pop_back();
  for ( size_t i=0; i < value.size(); ++i ){
    smallType workbench = value[i] | rhs.value[i];
    retval.value.push_back( workbench );
  }
  for ( size_t i=value.size(); i < rhs.value.size(); ++i )
    retval.value.push_back( rhs.value[i] );

  retval.clean_bits();
  return retval;
}

uberzahl uberzahl::operator & ( const uberzahl& rhs ) const
{
  if ( value.size() > rhs.value.size() )
    return rhs & *this;

  uberzahl retval = "0";
  retval.value.pop_back();
  for ( size_t i=0; i < value.size(); ++i ){
    smallType workbench = value[i] & rhs.value[i];
    retval.value.push_back( workbench );
  }

  retval.clean_bits();
  return retval;
}

uberzahl uberzahl::operator ^ ( const uberzahl& rhs ) const
{
  if ( value.size() > rhs.value.size() )
    return rhs ^ *this;

  uberzahl retval = "0";
  retval.value.pop_back();
  for ( size_t i=0; i < value.size(); ++i ){
    smallType workbench = value[i] ^ rhs.value[i];
    retval.value.push_back( workbench );
  }
  for ( size_t i=value.size(); i < rhs.value.size(); ++i )
    retval.value.push_back( rhs.value[i] );

  retval.clean_bits();
  return retval;
}

uberzahl uberzahl::random ( mediumType bits ){
  assert( bits > 0 );
  value.clear();

  smallType shortbits = bits % maxBits;
  smallType longbits = bits / maxBits;

  // compiler fix for 32k RAND generators
  // fill the value vector with randomly generated bits
  for ( size_t j = 0; j <= longbits; ++j ){
    value.push_back( 0 );
    for ( size_t i = 0; i <= maxBits / RAND_BITS; ++i ){
      value[i] = value[i] << RAND_BITS/2;
      value[i] = value[i] << RAND_BITS/2;
      value[i] = (value[i] + rand()) & mask;
    }
  }

  // shave off the extra bits that were placed
  if ( shortbits ){
    value[longbits] = value[longbits] >> (maxBits - shortbits);
    value[longbits] = value[longbits] | (1 << (shortbits - 1));
  }
  else{
    value.pop_back();
    value[longbits-1] = value[longbits-1] | (1 << (maxBits - 1));
  }

  return *this;
}

uberzahl uberzahl::inverse ( const uberzahl& b) const
{
  // computes (*this)^(-1) mod b
  if(*this=="1")
    return "1";
  std::pair<std::pair<uberzahl,uberzahl>,bool> inv = inverse(*this,b);
  inv.first.first = inv.first.first % b;
  if(inv.second && inv.first.first!="0")
    return b-inv.first.first;
  else
    return inv.first.first;
}

uberzahl uberzahl::gcd( const uberzahl& that ) const
{
  // computes gcd(*this, that)
  if ( *this < that )
    return that.gcd( *this );
  else if( that == "0" )
    return *this;
  else 
    return that.gcd( *this % that );
}

std::pair<std::pair<uberzahl,uberzahl>,bool> uberzahl::inverse ( const uberzahl& a, const uberzahl& b) const
{
  // helper function for inverse
  uberzahl nexta = b%a, coeff = b/a;
  if(nexta == "0")
    return std::make_pair(std::make_pair("0","0"),true);
  else if(nexta == "1")
    return std::make_pair(std::make_pair(coeff,"1"),true);
  std::pair<std::pair<uberzahl,uberzahl>,bool> nextinv = inverse(nexta,a);
  return std::make_pair(std::make_pair(nextinv.first.second+nextinv.first.first*coeff,nextinv.first.first),!nextinv.second);
}

smallType uberzahl::bit ( mediumType n ) const
{
  // returns the nth bit (0 indexed)
  mediumType largeBit = n / maxBits;
  smallType smallBit = n % maxBits;

  if ( largeBit >= value.size() )
    return 0;

  smallType bits = value[largeBit];
  return (bits>>smallBit) & 1;
}

smallType uberzahl::bitLength ( void ) const
{
  // returns the bit length (size) of a uberzahl
  for(size_t i=value.size(); i>0; --i) {
    if(value[i-1]!=0) {
      largeType k=1;
      size_t j=0;
      while(k<=value[i-1]) {
        ++j;
        k = k<<1;
      }
      return (i-1)*maxBits + j;
    }
  }
  return 1;
}

uberzahl uberzahl::exp ( const uberzahl& exponent ) const
{
  if ( exponent == "0" ) // exponent of 0
    return 1;
  else if ( exponent == "1" ) // exponent of 1
    return *this;

  if ( (exponent&1) == "1" ) // odd exponent
    return (this->exp(exponent^1)) * (*this);
  else {
    uberzahl tmp = this->exp(exponent >> 1);
    return tmp * tmp;
  }
}

// this is the recursive version, you need to implement the
// iterative version in your code!
uberzahl uberzahl::expm( const uberzahl& n, const uberzahl& mod ) const
{
  if ( n == "0" )
    return 1;
  else if ( n == "1" )
    return ( *this % mod );
  
  if ( (n&1) == "1" )
    return ( (this->expm(n^1, mod)) * (*this) ) % mod;
  else {
    uberzahl tmp = this->expm(n>>1, mod);
    return (tmp * tmp) % mod;
  }
}

uberzahl random ( const uberzahl& a, const uberzahl& b )
{
  if ( a > b ) return random( b, a );

  uberzahl retval;
  retval.random( b.bitLength() + 1 );
  retval = (retval % ( b-a )) + a;
  return retval;
}

bool rabinmiller ( const uberzahl& n, mediumType k ){
  // take care of corner cases 1,2,3 and even
  if ( n < 2 ) return false;
  else if ( n < 4 ) return true;
  else if ( (n&1) == "0" ) return false;

  uberzahl d = (n - 1) >> 1;
  mediumType s = 1;
  while ( (d&1) == "0" ){
    d = d >> 1;
    s = s + 1;
  }

  // witness loop
  uberzahl a;
  uberzahl x;
  for ( mediumType i = 0; i < k; ++i ){
    a = random( 2, n-2 );
    x = a.expm(d,n);

    if ( x == "1" || x == n-1 ) continue;

    for ( mediumType j = 0; j < s - 1; ++j ){
      x = x.expm(2,n);
      if ( x == "1" ) return false; // composite
      if ( x == n-1 ) break;
    }

    if ( x == n-1 ) continue;
    
    return false; // composite
  }

  return true; // probably prime
}

uberzahl nextprime ( const uberzahl& n, mediumType accuracy ){
  // checks every odd number greater or equal to n for primality
  uberzahl retval = n;
  if ( (retval&1) == "0" ) retval = retval + 1;
  while ( !rabinmiller(retval, accuracy) )
    retval = retval + 2;
  return retval;
}

uberzahl uberzahl::extract ( smallType start, smallType end ){
  uberzahl retval = *this;
  for ( size_t i = 0; i < start/maxBits && i < value.size(); ++i )
    retval.value[i] = 0; // zero everything before start block
  for ( size_t i = end/maxBits + 1; i < value.size(); ++i )
    retval.value[i] = 0; // zero everything after end block
  if ( start/maxBits < value.size() ){
    retval.value[start/maxBits] = retval.value[start/maxBits] >> start%maxBits; 
    retval.value[start/maxBits] = retval.value[start/maxBits] << start%maxBits; 
  } // zero first start bits
  if ( end/maxBits < value.size() ){
    retval.value[end/maxBits] = retval.value[end/maxBits] << (maxBits - ((end+1)%maxBits));
    retval.value[end/maxBits] = retval.value[end/maxBits] >> (maxBits - ((end+1)%maxBits));
  } // zero final maxBits - end bits
  return retval;
}

uberzahl uberzahl::rotateLeft ( smallType rotation, smallType start, smallType end ){
  uberzahl retval1 = extract(start, end); // look only at the bits from start -> end
  uberzahl retval2 = retval1;
  uberzahl retval3 = *this ^ retval1; // clear out extracted bits

  smallType rotateVal = rotation % ( end - start + 1 );
  retval1 = retval1 << rotateVal;
  retval2 = retval2 >> ((end-start+1) - rotateVal);

  retval1 = retval1.extract(start, end);
  retval2 = retval2.extract(start, end);

  return retval3 | retval1 | retval2;
}
uberzahl uberzahl::rotateRight ( smallType rotation, smallType start, smallType end ){
  uberzahl retval1 = extract(start, end); // look only at the bits from start -> end
  uberzahl retval2 = retval1;
  uberzahl retval3 = *this ^ retval1; // clear out extracted bits

  smallType rotateVal = rotation % ( end - start + 1 );
  retval1 = retval1 >> rotateVal;
  retval2 = retval2 << (end-start+1) - rotateVal;

  retval1 = retval1.extract(start, end);
  retval2 = retval2.extract(start, end);

  return retval3 | retval1 | retval2;
}
void uberzahl::setBit ( smallType index ){
  smallType largeindex = index / maxBits;
  smallType smallindex = index % maxBits;

  for ( size_t i = value.size(); i < largeindex + 1; ++i )
    value.push_back(0);

  value[largeindex] = value[largeindex] | ( 1 << smallindex );
}
void uberzahl::clearBit ( smallType index ){
  smallType largeindex = index / maxBits;
  smallType smallindex = index % maxBits;

  for ( size_t i = value.size()-1; i < largeindex; ++i )
    value.push_back(0);

  value[largeindex] = value[largeindex] & ~( 1 << smallindex );
}
void uberzahl::toggleBit ( smallType index ){
  smallType largeindex = index / maxBits;
  smallType smallindex = index % maxBits;

  for ( size_t i = value.size()-1; i < largeindex; ++i )
    value.push_back(0);

  value[largeindex] = value[largeindex] ^ ( 1 << smallindex );
}
