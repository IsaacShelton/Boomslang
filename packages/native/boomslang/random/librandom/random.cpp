
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#include "C:/Boomslang/core/boomslangcore.h"
#include "random.h"

void boomslang_randomize(){
    srand( boomslang_random_seed().data );
}

void boomslang_random_set_seed(boomslang_UnsignedInteger seed){
    srand( seed.data );
}

boomslang_UnsignedInteger boomslang_random_seed(){
    time_t now = time(0);
    unsigned char *ptr = (unsigned char *)&now;
    unsigned seed = 0;

    for (size_t i = 0; i < sizeof now; i++){
        seed = seed * (UCHAR_MAX + 2U) + ptr[i];
    }

    return boomslang_UnsignedInteger(seed);
}

boomslang_Number boomslang_random_under(boomslang_Integer number){
    return boomslang_Number( rand() / (RAND_MAX / number.data + 1) );
}
boomslang_Number boomslang_random_between(boomslang_Integer minimum, boomslang_Integer maximum){
    return boomslang_Number( minimum.data + rand() / (RAND_MAX / (maximum.data - minimum.data) + 1) );
}
