
#include <string>
#include <iostream>
#include <stdlib.h>
#include "../include/file.h"
#include "../include/lexer.h"
#include "../include/token.h"
#include "../include/options.h"
#include "../include/enforcer.h"
#include "../include/assembler.h"

using namespace std;

int main(int argc, char** argv){
    // Configure from specified options
    Configuration config = configure(&argc, &argv);

    // Tokenize the Code
    TokenList tokens = tokenize(contents(argv[1]));

    // Ensure the code is valid
    enforce(tokens);

    // Assemble results
    assemble(&config, tokens);

    return 0;
}
