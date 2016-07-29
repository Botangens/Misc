// header with several examples

#pragma once

static int i1;
void       set1 (int i1_);
int        get1 ();
void       blah1();
inline int _get1() {return i1;}


extern int i2;
void       set2 (int i_);
int        get2 ();
void       blah2();
inline int _get2() {return i2;}


//int i3; // error: redefinition
void       set3 (int i_);
int        get3 ();
void       blah3();
//inline int _get3() {return i3;} // error: i3 was not declared in this scope


//int i4; // error: redefinition
void       set4 (int i_);
int        get4 ();
void       blah4();
//inline int _get4() {return i4;} // error: i4 was not declared in this scope


static int i5;
void       set5 (int i_);
int        get5 ();
void       blah5();
inline int _get5() {return i5;}
