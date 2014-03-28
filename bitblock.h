#ifndef BITBLOCK_H
#define BITBLOCK_H
#include <iostream>
#include <string>
#include <vector>

using namespace std;

class bitblock{
    public:
   	 vector<bool> bits;

   	 //Instantiate with an int for num bits
   	 bitblock ( int ) ;

         //resizes the bitblock to n bits
         void resize(int n);

   	 //Set the value to a binary string "11001101..."
   	 void valSet( const char*);
         void valSet( long long);

   	 //Printing out the binary
   	 friend std::ostream& operator << (std::ostream&, const bitblock&);

   	 //& overload
   	 bitblock operator & (const bitblock&) const;

   	 //^ (XOR) overload
   	 bitblock operator ^ (const bitblock&) const;

   	 //! overload
   	 bitblock operator ! ( void ) const;

   	 //<< overload
   	 bitblock operator << (const int) const;


	 // returns a 64-bit number equivalent of the bitBlock
         long long getInt64() const;
};

#endif
