The four files contain different modifications of the same code

The example demonstrates possibilities of statefull preprocessor
metaprogramming in C++. The difference is caused by several 
implementation-defined features, different inner compilers` rules
and some actual bugs. Also two files intentionally contain almost
no comments. This is for easier reading as the code is pretty 
difficult. 
These file are marked as 'NoComments'. The code itself is 
duplicated in unmarked files those do contain comments.

------------------------------------------------------------------

'example_vc_g_NoComments.cpp' - simplest example for the very 
beginning.

'example_vc_g.cpp' - same, but with comments (should be helpfull).

'example_vc_g_clang_NoComments.cpp' - more complicated code with 
some workarounds for clang. Involves more mechanisms yet more
stable.

'example_vc_g_clang.cpp' - same, but with comments. 

------------------------------------------------------------------

NOTE: the more complicated the code - the more stable it is.
Large, detailed pieces of code involve more C++ rules and 
mechanisms, thus the behavior becomes less implementation-defined
and more reliable and accurate. (Paradox, is it not?)