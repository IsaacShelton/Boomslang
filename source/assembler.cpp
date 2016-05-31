
#include <fstream>
#include <iostream>
#include "../include/die.h"
#include "../include/run.h"
#include "../include/file.h"
#include "../include/scope.h"
#include "../include/errors.h"
#include "../include/locate.h"
#include "../include/options.h"
#include "../include/enforcer.h"
#include "../include/assembler.h"
#include "../include/management.h"

using namespace std;

void process_token(TokenContext context, bool& terminate_needed, string& output, ofstream& write, ofstream& header, ofstream& define, unsigned int& indentation, Environment& environment){
    // Terminate
    if(context.tokens[context.index].id == TOKENINDEX_TERMINATE and terminate_needed){
        output += ";\n";
        terminate_needed = false;
    }
    else { // Other tokens

        if(context.tokens[context.index].id == TOKENINDEX_STRING_LITERAL){
            output += "boomslang_String(\"" + context.tokens[context.index].data + "\")";
        }
        else if(context.tokens[context.index].id == TOKENINDEX_NUMERIC_LITERAL){
            output += "boomslang_Number(" + context.tokens[context.index].data + ")";
        }
        else if(context.tokens[context.index].id == TOKENINDEX_WORD){
            string name = context.tokens[context.index].data;

            output += resource(name);

            context.index++;

            if(context.tokens[context.index].id == TOKENINDEX_WORD){
                output += " ";
            }

            context.index--;

            terminate_needed = true;
        }
        else if(context.tokens[context.index].id == TOKENINDEX_METHOD_CALL){
            output += "." + resource(context.tokens[context.index].data);
        }
        else if(context.tokens[context.index].id == TOKENINDEX_OPEN){
            output += "(";
        }
        else if(context.tokens[context.index].id == TOKENINDEX_CLOSE){
            output += ")";
        }
        else if(context.tokens[context.index].id == TOKENINDEX_INDENT){
            indentation++;
        }
        else if(context.tokens[context.index].id == TOKENINDEX_DEDENT){
            indentation--;
        }
        else if(context.tokens[context.index].id == TOKENINDEX_KEYWORD){
            if(context.tokens[context.index].data == "on"){
                string return_value;
                string method_name;
                string method_code;
                string method_arguments;
                bool of_template = false;

                unsigned int before_indentation = indentation; // The indentation before processing tokens in method
                unsigned int token_indent = indentation + 1;    // The indentation during processing

                context.index++;
                method_name = context.tokens[context.index].data;
                context.index += 2;

                if(!environment_method_exists(environment.scope, Method{method_name, &environment.global, IGNORE_ARGS, IGNORE})){
                    die("Declared Method has no Implementation");
                }

                return_value = environment_method_get(environment.scope, Method{method_name, &environment.global, IGNORE_ARGS, IGNORE}).return_type;

                unsigned int balance = 0;
                bool value = false;
                bool first_word = true;

                while(balance != 0 or (context.tokens[context.index].id != TOKENINDEX_CLOSE)){
                    if(context.tokens[context.index].id == TOKENINDEX_OPEN){
                        balance++;
                    }
                    else if(context.tokens[context.index].id == TOKENINDEX_CLOSE){
                        balance--;
                    }
                    else if(context.tokens[context.index].id == TOKENINDEX_WORD){
                        if(!first_word){
                            method_arguments += " ";
                        }

                        method_arguments += resource(context.tokens[context.index].data);
                        first_word = false;
                    }
                    else if(context.tokens[context.index].id == TOKENINDEX_ASSIGN){
                        if(value == true){
                            die(UNEXPECTED_OPERATOR_INEXP);
                        }

                        value = true;
                        method_arguments += "=";
                    }
                    else if(context.tokens[context.index].id == TOKENINDEX_NEXT){
                        method_arguments += ", ";
                        value = false;
                        first_word = true;
                    }
                    else if(context.tokens[context.index].id == TOKENINDEX_STRING_LITERAL){
                        if(value == false){
                            die(UNEXPECTED_OPERATOR);
                        }

                        method_arguments += "boomslang_String(\"" + context.tokens[context.index].data + "\")";
                    }
                    else if(context.tokens[context.index].id == TOKENINDEX_NUMERIC_LITERAL){
                        if(value == false){
                            die(UNEXPECTED_OPERATOR);
                        }

                        method_arguments += "boomslang_Number(" + context.tokens[context.index].data + ")";
                    }
                    else {
                        die(INVALID_TOKEN);
                    }

                    context.index++;
                }

                context.index+=2;

                if(context.tokens[context.index].id == TOKENINDEX_INDENT){
                    context.index++;
                    while(before_indentation != token_indent){
                        process_token(context, terminate_needed, method_code, write, header, define, token_indent, environment);
                        context.index++;
                    }
                }

                if( string_count(method_name, ".") == 1){
                    of_template = true;
                }

                if(!of_template){
                    if(return_value == "void"){
                        write  << "void " + resource(method_name) + "(" + method_arguments + "){\n" + method_code + "}\n";
                        header << "void " + resource(method_name) + "(" + method_arguments + ");\n";
                    }
                    else {
                        write  << resource(return_value) + " " + resource(method_name) + "(" + method_arguments + "){\n" + method_code + "}\n";
                        header << resource(return_value) + " " + resource(method_name) + "(" + method_arguments + ");\n";
                    }
                }
                else {
                    if(return_value == "void"){
                        write  << "void " + resource(string_get_until(method_name, ".")) + "::" + resource(string_delete_amount(string_delete_until(method_name,"."),1)) + "(" + method_arguments + "){\n" + method_code + "}\n";
                        template_add_method(string_get_until(method_name, "."), "void " + resource(string_delete_amount(string_delete_until(method_name,"."),1)) + "(" + method_arguments + ");");
                    }
                    else {
                        write  << resource(return_value) + " " + resource(string_get_until(method_name, ".")) + "::" + resource(string_delete_amount(string_delete_until(method_name,"."),1)) + "(" + method_arguments + "){\n" + method_code + "}\n";
                        template_add_method(string_get_until(method_name, "."), resource(return_value) + " " + resource(string_delete_amount(string_delete_until(method_name,"."),1)) + "(" + method_arguments + ");");
                    }
                }
            }
            else if(context.tokens[context.index].data == "template"){
                string template_name;
                string template_code;

                unsigned int before_indentation = indentation; // The indentation before processing tokens in method
                unsigned int token_indent = indentation + 1;    // The indentation during processing

                context.index++;
                template_name = context.tokens[context.index].data;

                if(!environment_template_exists(environment.scope, Template{template_name})){
                    die("Declared Template has no Implementation");
                }

                context.index+=2;

                if(context.tokens[context.index].id == TOKENINDEX_INDENT){
                    context.index++;
                    while(before_indentation != token_indent){
                        process_token(context, terminate_needed, template_code, write, header, define, token_indent, environment);
                        context.index++;
                    }
                }

                header << "class " + resource(template_name) + "{\npublic:\n" + template_code + "#ifdef BOOMSLANGDEFINE_" + template_name + "\nBOOMSLANGDEFINE_" + template_name + ";\n#endif // BOOMSLANGDEFINE_" + template_name + "\n};\n";
            }
            else if(context.tokens[context.index].data == "return"){
                output += "return ";
            }
        }
        else if(context.tokens[context.index].id == TOKENINDEX_ADDRESS){
            output += "&";
        }
        else if(context.tokens[context.index].id == TOKENINDEX_MEMBER){
            output += ".";
        }
        else if(context.tokens[context.index].id == TOKENINDEX_NEXT){
            output += ",";
        }
        else if(context.tokens[context.index].id == TOKENINDEX_ADDRESSMEMBER){
            output += "->";
        }
        else if(context.tokens[context.index].id == TOKENINDEX_NOT){
            output += "!";
        }
        else if(context.tokens[context.index].id == TOKENINDEX_ASSIGN){
            output += " = ";
        }
        else if(context.tokens[context.index].id == TOKENINDEX_ADD){
            output += " + ";
        }
        else if(context.tokens[context.index].id == TOKENINDEX_SUBTRACT){
            output += " - ";
        }
        else if(context.tokens[context.index].id == TOKENINDEX_MULTIPLY){
            output += " * ";
        }
        else if(context.tokens[context.index].id == TOKENINDEX_DIVIDE){
            output += " / ";
        }

        terminate_needed = true;
    }
}

void compile(Configuration* config, const TokenList& tokens, Environment& environment){
    // Writes resulting c++ source code

    ofstream write( (HOME + CPP_SOURCE).c_str() );
    ofstream header( (HOME + CPP_HEADER).c_str() );
    ofstream define( (HOME + CPP_DEFINE).c_str() );
    bool terminate_needed = false;
    string global;

    // Ensure file(s) are open
    if(!write.is_open()){
        die("Failed to open source file(s)");
    }

    // Write Comment
    write << "/*This file was generated by Boomslang, modify at your own risk*/\n\n#include \"define.h\"\n#include \"../core/boomslangcore.h\"\n#include \"source.h\"\n\nint* argc;\nchar*** argv;\n\n";
    header << "/*This file was generated by Boomslang, modify at your own risk*/\n\n#ifndef SOURCE_H_INCLUDED\n#define SOURCE_H_INCLUDED\n\n#include \"define.h\"\n#include \"../core/boomslangcore.h\"\n\nextern int* argc;\nextern char*** argv;\n\n";
    define << "/*This file was generated by Boomslang, modify at your own risk*/\n\n#ifndef DEFINE_H_INCLUDED\n#define DEFINE_H_INCLUDED\n\n";

    if(template_additions.size() != 0){
        header << "\n";
    }

    unsigned int indentation = 0;

    // Process tokens
    for(unsigned int index = 0; index < tokens.size(); index++){
        process_token(TokenContext{tokens, index}, terminate_needed, global, write, header, define, indentation, environment);
    }

    for(unsigned int i = 0; i < template_additions.size(); i++){
        define << "#define BOOMSLANGDEFINE_" + template_additions[i].name + " " + template_additions[i].additions + "\n";
    }

    // Write Main
    write << "int main(int _agc, char** _agv){\nargc = &_agc;\nargv = &_agv;\nboomslang_main();\nreturn 0;\n}\n";
    header << "int main(int, char**);\n\n#endif\n";
    define << "#endif\n";

    write.close();
    header.close();
}

void build(Configuration* config){
    // Builds the executable/package

    string compile_flags = "-std=c++11 ";
    string linker_flags;

    #if defined(__WIN32__)
    //Compile without console unless specified

    if (!config->console){
        linker_flags += "-Wl,--subsystem,windows ";
    }

    if (!config->optimize){
        linker_flags += "-O3 ";
    }

    if(file_exists( ("C:\\Users\\" + USERNAME + "\\AppData\\Roaming\\Boomslang\\source\\source.o") )){
        if(remove( ("C:\\Users\\" + USERNAME + "\\AppData\\Roaming\\Boomslang\\source\\source.o").c_str() )!=0){
            die("Failed to delete object file");
        }
    }

    { //Run MinGW
        bool bad = execute_silent("C:\\Users\\" + USERNAME + "\\AppData\\Roaming\\Boomslang\\resources\\MinGW\\bin\\g++","-c \"C:\\Users\\" + USERNAME + "\\AppData\\Roaming\\Boomslang\\source\\source.cpp\" " + compile_flags + " -o \"" + "C:\\Users\\" + USERNAME + "\\AppData\\Roaming\\Boomslang\\source\\source.o" + "\" 2>\"C:\\Users\\" + USERNAME + "\\AppData\\Roaming\\Boomslang\\logs\\native.log\"");

        if(bad) die("Native Compiler Error");
    }

    if(config->package){
        // -=- old -=-
        //execute_silent("C:\\Users\\" + USERNAME + "\\AppData\\Roaming\\Boomslang\\resources\\MinGW\\bin\\ar","rvs \"C:\\Users\\" + USERNAME + "\\AppData\\Roaming\\Boomslang\\source\\final.a\" \"C:\\Users\\" + USERNAME + "\\AppData\\Roaming\\Boomslang\\source\\final.o\" \"C:\\Users\\" + USERNAME + "\\AppData\\Roaming\\Boomslang\\core\\libboomslangcore.a\" " + linker_flags + " 2>\"C:\\Users\\" + USERNAME + "\\AppData\\Roaming\\Boomslang\\logs\\linker_errors.log\"");
        //branch_create(filename_path(file_read_name) + filename_change_ext(filename_name(file_read_name),"branch"),"C:\\Users\\" + USERNAME + "\\AppData\\Roaming\\Boomslang\\source\\final.a","C:\\Users\\" + USERNAME + "\\AppData\\Roaming\\Boomslang\\source\\final.h","C:\\Users\\" + USERNAME + "\\AppData\\Roaming\\Boomslang\\source\\final.boomslang");
    }
    else {
        bool bad = execute_silent("C:\\Users\\" + USERNAME + "\\AppData\\Roaming\\Boomslang\\resources\\MinGW\\bin\\g++","\"C:\\Users\\" + USERNAME + "\\AppData\\Roaming\\Boomslang\\source\\source.o\" " + linker_flags + "\"C:\\Users\\" + USERNAME + "\\AppData\\Roaming\\Boomslang\\core\\libboomslangcore.a\" -o \"" +  config->output_filename + "\" 2>\"C:\\Users\\" + USERNAME + "\\AppData\\Roaming\\Boomslang\\logs\\linker.log\"");

        if(bad) die("Native Compiler Error");
    }

    if(config->run){
        system(("\"" + config->output_filename + "\"").c_str());
    }
    #endif

    cout << "Successfully Built '" + filename_name(config->output_filename) + "'" << endl;
}

void assemble(Configuration* config, const TokenList& tokens, Environment& environment){
    // Creates executable/package from code

    // Write source code
    compile(config, tokens, environment);

    // Build the result
    build(config);
}
