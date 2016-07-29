// secondary source file with local static data

#include "someHead.hpp"

//int i1; // error: redefinition
void set1 ( int i_ ) {  i1 = i_;   }
int  get1 ()         {  return i1; }
void blah1()         {  set1(5);   }


int i2;
void set2 ( int i_ ) {  i2 = i_;   }
int  get2 ()         {  return i2; }
void blah2()         {  set2(5);   }


int i3;
void set3 ( int i_ ) {  i3 = i_;   }
int  get3 ()         {  return i3; }
void blah3()         {  set3(5);   }


static int i4;
void set4 ( int i_ ) {  i4 = i_;   }
int  get4 ()         {  return i4; }
void blah4()         {  set4(5);   }


extern int i5;
void set5 ( int i_ ) {  i5 = i_;   }
int  get5 ()         {  return i5; }
void blah5()         {  set5(5);   }
