
#ifndef RANDOM_H_INCLUDED
#define RANDOM_H_INCLUDED

void boomslang_randomize();
void boomslang_random_set_seed(boomslang_UnsignedInteger seed);
boomslang_UnsignedInteger boomslang_random_seed();
boomslang_Double boomslang_random_under(boomslang_Integer number);
boomslang_Double boomslang_random_between(boomslang_Integer minimum, boomslang_Integer maximum);

#endif // RANDOM_H_INCLUDED
