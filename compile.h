/**
    ============ compile.h ============
    Code for Compiling Boomslang

    This file should NOT be included
    anywhere besides from init.h at
    compiling.

    This Header does NOT contain a
    header guard so include with
    cation.
*/

using namespace std;

string compile_prev = "";

///Main Compile Loop
while(compile_code!="" and compile_code!=compile_prev){
    compile_code = string_kill_all_whitespace(compile_code);
    compile_prev = compile_code;

    //Is is a line-comment?
    if(compile_code.substr(0,2)=="//"){
        compile_code = string_delete_amount(compile_code,2);
        compile_code = string_delete_until(compile_code,"\n");
    }

    //Is is a multi-line-comment?
    if(compile_code.substr(0,2)=="/#"){
        compile_code = string_delete_amount(compile_code,2);
        compile_code = string_delete_until(compile_code,"#");
        compile_code = string_delete_amount(compile_code,1);
    }

    //Is is raw C++?
    if(compile_code.substr(0,2)=="/{"){
        int balance = 0;
        compile_code = string_delete_amount(compile_code,2);

        while(!( balance==0 and compile_code.substr(0,1)=="}" )){
            if(compile_code.substr(0,1)=="{"){
                balance+=1;
                write(compile_code.substr(0,1),true);
                compile_code = string_delete_amount(compile_code,1);
            }
            else
            if(compile_code.substr(0,1)=="}"){
                balance-=1;
                write(compile_code.substr(0,1),true);
                compile_code = string_delete_amount(compile_code,1);
            }else{
                write(compile_code.substr(0,1),true);
                compile_code = string_delete_amount(compile_code,1);
            }
        }

        compile_code = string_delete_amount(compile_code,1);
    }

    //Is it a action?
    if( ve_actions.exists(string_get_until(compile_code," ")) ){
        error_debug("Found " + string_get_until(compile_code," ") + " to be a action.");

        string action = string_get_until(compile_code," ");
        compile_code = string_delete_until(compile_code," ");
        compile_code = string_kill_whitespace(compile_code);

        #include "action.h"

        compile_code = string_kill_whitespace(compile_code);
        continue;
    }

    //Is it a function?
    if( is_identifier(string_get_until_or(compile_code,"(")) ){
        error_debug("Found " + string_get_until_or(compile_code,"(") + " to be a function.");

        string function_name = string_get_until_or(compile_code,"(");
        compile_code = string_delete_until_or(compile_code,"(");

        if( function_handler.exists(function_name,S_NULL,S_NULL,I_NULL,SCOPETYPE_GLOBAL) ){

        } else {
            error_fatal("The Function '" + function_name + "' does not exists.");
            pend();
            return EXIT_FAILURE;
        }

        compile_code = string_kill_whitespace(compile_code);
        continue;
    }

    //Is it a variable?
    if( is_identifier(string_get_until_or(compile_code," =+-/*.")) ){
        #include "variable.h"
    }

    //Is it a value?
    if( rawvalue_exists(compile_code) ){
        error_debug("Found raw value exists");

        if(compile_code.substr(0,1)=="("){//Expression
            string raw_expression_type = S_NULL;
            string raw_expression;

            if (harvest_raw_expression(compile_code,raw_expression,raw_expression_type)==EXIT_FAILURE)
                return EXIT_FAILURE;

            ve_main_code += raw_expression;
            compile_code = string_kill_whitespace(compile_code);
            string prev_return_type = raw_expression_type;
            string return_type = "";

            if(compile_code.substr(0,1)!="."){
                error_fatal("Expected '.' before '" + compile_code.substr(0,1) + "' after expression.");
                pend();
                return EXIT_FAILURE;
            }

            while(compile_code.substr(0,1)=="." or compile_code.substr(0,1)==","){

                if(compile_code.substr(0,1)==","){
                    write(";",true);
                    return_type = function_handler.functions[function_handler.find(string_get_until_or(string_delete_amount(compile_code,1)," ("),S_NULL,S_NULL,class_handler.find(raw_expression_type),SCOPETYPE_TEMPLATE)].type;
                    ve_main_code += raw_expression;
                    if(code_parser.parse_function_from(compile_code,true,true,class_handler.find(raw_expression_type))==-1){
                        return EXIT_FAILURE;
                    }
                    prev_return_type = return_type;
                }

                if(compile_code.substr(0,1)=="."){
                    if(function_handler.exists(string_get_until_or(string_delete_amount(compile_code,1)," ("),S_NULL,S_NULL,class_handler.find(prev_return_type),SCOPETYPE_TEMPLATE) and prev_return_type!="null"){
                        return_type = function_handler.functions[function_handler.find(string_get_until_or(string_delete_amount(compile_code,1)," ("),S_NULL,S_NULL,class_handler.find(prev_return_type),SCOPETYPE_TEMPLATE)].type;
                        if(code_parser.parse_function_from(compile_code,true,true,class_handler.find(prev_return_type))==-1){
                            return EXIT_FAILURE;
                        }
                        prev_return_type = return_type;
                    } else {
                        if(prev_return_type!="null"){
                            error_fatal("Undeclared Function '" + string_get_until_or(string_delete_amount(compile_code,1)," (") + "' of template '" + prev_return_type + "'.");
                            pend();
                            return EXIT_FAILURE;
                        } else {
                            error_fatal("You Can't Call Functions of null");
                            pend();
                            return EXIT_FAILURE;
                        }
                    }
                }
            }

            compile_code = string_kill_whitespace(compile_code);

            code_parser.chop(compile_code);

            compile_code = string_kill_whitespace(compile_code);
            write(";\n",true);
        }
        else if(compile_code.substr(0,1)=="\""){//String
            //String

            string rawstring = harvest_string(compile_code);
            ve_main_code += "BOOMSLANGCORE_create_string(\"" + rawstring + "\")";
            compile_code = string_kill_whitespace(compile_code);
            string prev_return_type = "String";
            string return_type = "";

            if(compile_code.substr(0,1)!="."){
                error_fatal("Expected '.' before '" + compile_code.substr(0,1) + "' after \"" + rawstring + "\".");
                pend();
                return EXIT_FAILURE;
            }

            while(compile_code.substr(0,1)=="." or compile_code.substr(0,1)==","){

                if(compile_code.substr(0,1)==","){
                    write(";",true);
                    return_type = function_handler.functions[function_handler.find(string_get_until_or(string_delete_amount(compile_code,1)," ("),S_NULL,S_NULL,class_handler.find("String"),SCOPETYPE_TEMPLATE)].type;
                    ve_main_code += "BOOMSLANGCORE_create_string(\"" + rawstring + "\")";
                    if(code_parser.parse_function_from(compile_code,true,true,class_handler.find("String"))==-1){
                        return EXIT_FAILURE;
                    }
                    prev_return_type = return_type;
                }

                if(compile_code.substr(0,1)=="."){
                    if(function_handler.exists(string_get_until_or(string_delete_amount(compile_code,1)," ("),S_NULL,S_NULL,class_handler.find(prev_return_type),SCOPETYPE_TEMPLATE) and prev_return_type!="null"){
                        return_type = function_handler.functions[function_handler.find(string_get_until_or(string_delete_amount(compile_code,1)," ("),S_NULL,S_NULL,class_handler.find(prev_return_type),SCOPETYPE_TEMPLATE)].type;
                        if(code_parser.parse_function_from(compile_code,true,true,class_handler.find(prev_return_type))==-1){
                            return EXIT_FAILURE;
                        }
                        prev_return_type = return_type;
                    } else {
                        if(prev_return_type!="null"){
                            error_fatal("Undeclared Function '" + string_get_until_or(string_delete_amount(compile_code,1)," (") + "' of template '" + prev_return_type + "'.");
                            pend();
                            return EXIT_FAILURE;
                        } else {
                            error_fatal("You Can't Call Functions of null");
                            pend();
                            return EXIT_FAILURE;
                        }
                    }
                }
            }

            compile_code = string_kill_whitespace(compile_code);

            code_parser.chop(compile_code);

            compile_code = string_kill_whitespace(compile_code);
            write(";\n",true);
        } else if(compile_code.substr(0,1)=="0" or compile_code.substr(0,1)=="1" or compile_code.substr(0,1)=="2" or compile_code.substr(0,1)=="3" or compile_code.substr(0,1)=="4" or compile_code.substr(0,1)=="5" or compile_code.substr(0,1)=="6" or compile_code.substr(0,1)=="7" or compile_code.substr(0,1)=="8"
        or compile_code.substr(0,1)=="9"){//Number
            //Decimal

            string rawdecimal = harvest_decimal(compile_code);
            ve_main_code += "BOOMSLANGCORE_create_number(" + rawdecimal + ")";
            compile_code = string_kill_whitespace(compile_code);
            string return_type = "Decimal";
            string prev_return_type = "";

            while(compile_code.substr(0,1)=="." or compile_code.substr(0,1)==","){

                if(compile_code.substr(0,1)==","){
                    write(";\n",true);
                    return_type = function_handler.functions[function_handler.find(string_get_until_or(string_delete_amount(compile_code,1)," ("),S_NULL,S_NULL,class_handler.find("Decimal"),SCOPETYPE_TEMPLATE)].type;
                    ve_main_code += "BOOMSLANGCORE_create_number(" + rawdecimal + ")";
                    if(code_parser.parse_function_from(compile_code,true,true,class_handler.find("Decimal"))==-1){
                        return EXIT_FAILURE;
                    }
                    prev_return_type = return_type;
                }

                if(compile_code.substr(0,1)=="."){
                    if(function_handler.exists(string_get_until_or(string_delete_amount(compile_code,1)," ("),S_NULL,S_NULL,class_handler.find(prev_return_type),SCOPETYPE_TEMPLATE) and prev_return_type!="null"){
                        return_type = function_handler.functions[function_handler.find(string_get_until_or(string_delete_amount(compile_code,1)," ("),S_NULL,S_NULL,class_handler.find(prev_return_type),SCOPETYPE_TEMPLATE)].type;
                        if(code_parser.parse_function_from(compile_code,true,true,class_handler.find(prev_return_type))==-1){
                            return EXIT_FAILURE;
                        }
                        prev_return_type = return_type;
                    } else {
                        if(prev_return_type!="null"){
                            error_fatal("Undeclared Function '" + string_get_until_or(string_delete_amount(compile_code,1)," (") + "' of template '" + prev_return_type + "'.");
                            pend();
                            return EXIT_FAILURE;
                        } else {
                            error_fatal("You Can't Call Functions of null");
                            pend();
                            return EXIT_FAILURE;
                        }
                    }
                }
            }

            compile_code = string_kill_whitespace(compile_code);

            code_parser.chop(compile_code);

            compile_code = string_kill_all_whitespace(compile_code);
            write(";\n",true);
        }

            compile_code = string_kill_whitespace(compile_code);
    }

    compile_code = string_kill_all_whitespace(compile_code);
}

//Internal Error?
if (compile_code==compile_prev and compile_code!=""){
    error_fatal("Internal Error");
    error_debug("Remaining code:\n" + compile_code + "\n-----------------------");
    pend();
    return EXIT_FAILURE;
}

file_write << "int main(){\n" + ve_main_code + "\nreturn 0;\n}";
