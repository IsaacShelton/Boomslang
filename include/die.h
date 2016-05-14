
#ifndef DIE_H_INCLUDED
#define DIE_H_INCLUDED

#include <iostream>
#include <stdlib.h>

#define die(a) {std::cerr << a << endl; exit(1);}
#define fail(a) {std::cerr << a << endl; error_count++;}

extern unsigned int error_count;

#endif // DIE_H_INCLUDED
