
/*
    (c) 2016 Isaac Shelton

    This file is part of Boomslang.

    Boomslang is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Boomslang is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Boomslang. If not, see <http://www.gnu.org/licenses/>.
*/

#include <string>
#include <iostream>
#include <stdlib.h>
#include "../include/file.h"
#include "../include/clean.h"
#include "../include/token.h"
#include "../include/lexer.h"
#include "../include/options.h"
#include "../include/enforcer.h"
#include "../include/assembler.h"

int main(int argc, char** argv){
    // Configure from specified options
    Configuration config = configure(&argc, &argv);

    // Tokenize the Code
    TokenList tokens = tokenize(contents(argv[1]));

    // Ensure the code is valid
    Environment environment = enforce(&config, tokens);

    // Assemble results
    assemble(&config, tokens, environment);

    // Clean up
    clean(&config, environment);
    return 0;
}
