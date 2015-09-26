/**
    ============ base.h ============
    Code for Base Compiling Functions.

    This file should NOT be included
    anywhere besides from main.cpp.

    This Header does NOT contain a
    header guard so include with
    cation.
*/

using namespace std;

//Code parsing Object
class CodeParser{
public:
    //Parses Function Arguments, example input: ("hello world",10)
    //Returns code with semicolon and newline attached.
    int parse_args(string& code, bool write_to_main){
        code = string_kill_whitespace(code);

        if(code.substr(0,1)!="("){
            error_fatal("Expected '(' before '" + code.substr(0,1) + "' when Parsing Function Arguments.");
            pend();
            return EXIT_FAILURE;
        }

        code = string_delete_amount(code,1);
        write("(",write_to_main);

        int balance = 0;
        string code_prev;
        while( (code.substr(0,1)!=")" and balance!=0) and code!="" and (code_prev!=code)){
            code = string_kill_whitespace(code);

            code_prev = code;

            code = string_kill_whitespace(code);
        }

        if(code_prev==code){
            error_fatal("Internal Error when Parsing Function Arguments.");
            error_debug("Remaining code:\n" + compile_code + "\n-----------------------");
            pend();
            return EXIT_FAILURE;
        }

        if(code.substr(0,1)==")"){
            code = string_delete_amount(code,1);
            write(")",write_to_main);
        }

        return EXIT_SUCCESS;
    }

    //Parses Function that begins with a dot ".show()" for example
    int parse_function_from(string& code, bool write_to_main, bool check_semicolon, int class_id){
        code = string_kill_whitespace(code);

        if(code.substr(0,1)!="." and code.substr(0,1)!=","){
            error_fatal("Expected '.' or ',' before '" + code.substr(0,1) + "'.");
            pend();
            return EXIT_FAILURE;
        }

        code = string_delete_amount(code,1);

        if(!function_handler.exists(string_get_until_or(code," ("),S_NULL,S_NULL,class_id,SCOPETYPE_TEMPLATE)){
            error_fatal("Unknown Function '" + string_get_until_or(code," (") + "' of Template '" + class_handler.classes[class_id].name + "'");
            pend();
            return EXIT_FAILURE;
        }

        string function_name = string_get_until_or(code," (");
        code = string_delete_until_or(code," (");

        code = string_kill_whitespace(code);

        if(code.substr(0,1)!="("){
            error_fatal("Expected '(' before '" + code.substr(0,1) + "' when calling function '" + function_name + "'.");
            pend();
            return EXIT_FAILURE;
        }

        write("." + resource(function_name),write_to_main);

        if(this->parse_args(code,write_to_main)==-1){
            return EXIT_FAILURE;
        }

        return EXIT_SUCCESS;
    }

    //Takes string and removes semicolon and newline if needed
    void chop(string& code){
        code = string_kill_whitespace(code);

        if(code.substr(0,1)=="\n"){
            code = string_delete_amount(code,1);
        }

        code = string_kill_whitespace(code);

        if(code.substr(0,1)==";"){
            code = string_delete_amount(code,1);
        }

        code = string_kill_whitespace(code);
    }

    //Writes and removes the value in variable expressions
    int harvest_from_variable_value(string& code,string type, bool write_to_main){
        /*
            code - code
            type - variable type
            write_to_main - write to main

            Example:
            "Hello " + "World"\n
        */

        string code_prev;
        int balance = 0;

        while( ((code.substr(0,1)!="\n" and code.substr(0,1)!=";") or balance!=0) and (code_prev!=code) ){
            code_prev = code;
            code = string_kill_whitespace(code);

            if(this->arg_type(code)==ARGTYPE_STRING){
                ///TODO String Handling code in code_parser.harvest_from_variable_value()

                write("BOOMSLANGCORE_create_string(\"" + harvest_string(code) + "\")",write_to_main);
            }
            else if(this->arg_type(code)==ARGTYPE_NUMBER){
                ///TODO FINISH Number Handling code in code_parser.harvest_from_variable_value()

                write("BOOMSLANGCORE_create_number(" + harvest_decimal(code) + ")",write_to_main);
            }
            else if(this->arg_type(code)==ARGTYPE_VARIABLE){
                ///TODO Variable Handling code in code_parser.harvest_from_variable_value()
            }
            else if(this->arg_type(code)==ARGTYPE_FUNCTION){
                ///TODO Function Handling code in code_parser.harvest_from_variable_value()
            }

            code = string_kill_whitespace(code);
        }

        if(code_prev==code){
            error_fatal("Internal Variable Value Error");
            pend();
            return EXIT_FAILURE;
        }

        return EXIT_SUCCESS;
    }

    //Function to Determine what data type is at the beginning of the string
    int arg_type(string code){
        if(code.substr(0,1)=="\""){
            return ARGTYPE_STRING;
        } else if(code.substr(0,1)=="-"
        or code.substr(0,1)=="0"
        or code.substr(0,1)=="1"
        or code.substr(0,1)=="2"
        or code.substr(0,1)=="3"
        or code.substr(0,1)=="4"
        or code.substr(0,1)=="5"
        or code.substr(0,1)=="6"
        or code.substr(0,1)=="7"
        or code.substr(0,1)=="8"
        or code.substr(0,1)=="9"){
            return ARGTYPE_NUMBER;
        }
        else if(function_handler.exists(string_get_until(code," ("),S_NULL,S_NULL,I_NULL,SCOPETYPE_GLOBAL)){
            return ARGTYPE_FUNCTION;
        } else {
            return ARGTYPE_VARIABLE;
        }
    }
}code_parser;
