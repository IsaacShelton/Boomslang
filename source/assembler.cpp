
#include <fstream>
#include <iostream>
#include "../include/die.h"
#include "../include/locate.h"
#include "../include/options.h"
#include "../include/assembler.h"

using namespace std;

void compile(Configuration* config, const TokenList& tokens){
    // Writes output c++ source code

    ofstream write( (HOME + CPP_SOURCE).c_str() );
    bool terminate_needed = false;

    if(!write.is_open()){
        die("Failed to open source file(s)");
    }

    // Write Header
    write << "\n#include \"../boomslangcore/boomslangcore.h\"\n\n";

    // Write Main
    write << "int main(){\n";

    for(unsigned int index = 0; index < tokens.size(); index++){
        // Write token

        // Terminate
        if(tokens[index].id == TOKENINDEX_TERMINATE and terminate_needed){
            write << ";\n";
            terminate_needed = false;
        }
        else { // Other tokens

            if(tokens[index].id == TOKENINDEX_STRING_LITERAL){
                write << "boomslang_String(\"" + tokens[index].data + "\")";
            }
            else if(tokens[index].id == TOKENINDEX_NUMERIC_LITERAL){
                write << "boomslang_Number(" + tokens[index].data + ")";
            }
            else if(tokens[index].id == TOKENINDEX_METHOD_CALL){
                write << "." + tokens[index].data;
            }
            else if(tokens[index].id == TOKENINDEX_OPEN){
                write << "(";
            }
            else if(tokens[index].id == TOKENINDEX_CLOSE){
                write << ")";
            }

            terminate_needed = true;
        }
    }

    // Close Main
    write << "}\n";

    write.close();
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
