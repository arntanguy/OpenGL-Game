#ifndef __Debug__
#define __Debug__


// Debug functionality: just use dout << ... as you would use cout
// Set DEBUG from cmake

#ifndef DEBUG
    #define DEBUG 1
#endif

// Every debug
#define ALL_DEBUG 1
// Warnings and errors
#define WARNING_DEBUG 2
// Only errors
#define ERROR_DEBUG 3
// Only warnings
#define ONLY_WARNINGS 4

#if ( DEBUG == ALL_DEBUG)
#define dout std::cout
#define derr std::cout
#define dwarn std::cout

#elif (DEBUG == WARNING_DEBUG)
#define derr 0 && dout std::cout
#define derr 0 && derr std::cout
#define dwarn std::cout

#elif (DEBUG == ERROR_DEBUG)
#define dout 0 && dout std::cout
#define dwarn 0 && derr std::cout
#define derr std::cout

#elif (DEBUG == ONLY_WARNINGS)
#define dout 0 && dout std::cout
#define dwarn std::cout
#define derr 0 && derr std::cout
#endif


// Show information messages
#ifndef INFO
    #define INFO 1
#endif
#if (INFO == 1)
    #define dinf std::cout
#else
    #define dinf 0 && dinf std::cout
#endif

#endif
