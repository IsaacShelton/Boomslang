
#include <iostream>
#include "../include/options.h"
#include "../include/assembler.h"

using namespace std;

void compile(Configuration* config, const TokenList& tokens){
    // Writes output c++ source code

    if(true)
}

void build(Configuration* config){
    // Builds the executable/package

}

void assemble(Configuration* config, const TokenList& tokens){
    // Creates executable/package from code

    // Write source code
    compile(config, tokens);

    // Build the result
    build(config);
}
