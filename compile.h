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
    compile_code = string_kill_newline(compile_code);
    int new_indentation = 0;

    //Get Indents
    while(is_indent(compile_code)){
        //Record Indent
        new_indentation++;

        //Remove indent
        if(compile_code.substr(0,4)=="    "){
            compile_code = string_delete_amount(compile_code,4);
        }
        else if(compile_code.substr(0,1)=="\t"){
            compile_code = string_delete_amount(compile_code,1);
        }
        else {
            error_fatal("Encountered an unexpected error while parsing block.");
            pend();
            return EXIT_FAILURE;
        }
    }

    //Check for blocks
    if(new_indentation > indentation){
        while(new_indentation > indentation){
            indentation++;
            write("{\n",true);
        }
    }
    else if(indentation > new_indentation){
        while(indentation > new_indentation){
            indentation--;
            write("}\n",true);
        }
    }

    if(compile_code=="") continue;

    //Prepare for command
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
            } else {
                write(compile_code.substr(0,1),true);
                compile_code = string_delete_amount(compile_code,1);
            }
        }

        compile_code = string_delete_amount(compile_code,1);
    }

    //Is it a keyword?
    if(ve_keywords.exists(string_get_until_or(compile_code," \n"))){
        if(string_get_until_or(compile_code," \n")=="on"){
            //Function Declaration
            compile_code = string_delete_amount(compile_code,2);
            compile_code = string_kill_whitespace(compile_code);

            if(compile_code.substr(0,1)!="("){
                eroor_fatal("Expected '(' before '" + compile_code.substr(0,1) + "' in Method Argument Declaration");
                pend();
                return EXIT_FAILURE;
            }

            #include "compile_function.h"
        }
    }

    //Is it a action?
    if( ve_actions.exists(string_get_until(compile_code," ")) ){
        error_debug("Found " + string_get_until(compile_code," ") + " to be a action.");

        string action = string_get_until(compile_code," ");
        compile_code = string_delete_until(compile_code," ");
        compile_code = string_kill_whitespace(compile_code);

        #include "action.h"
        continue;
    }

    //Is it a method?
    if( is_identifier(string_get_until_or(compile_code,"(")) ){
        error_debug("Found " + string_get_until_or(compile_code,"(") + " to be a function.");

        string function_name = string_get_until_or(compile_code,"(");
        compile_code = string_delete_until_or(compile_code,"(");

        if( function_handler.exists(function_name,S_NULL,S_NULL,I_NULL,SCOPETYPE_GLOBAL) ){
            if(!function_handler.exists(function_name,S_NULL,S_NULL,I_NULL,SCOPETYPE_GLOBAL)){
                error_fatal("Undeclared method '" + function_name + "'");
                return EXIT_FAILURE;
            }

            write(resource(function_name) + "(",true);

            string function_code_prev;
            string argument_type;

            compile_code = string_delete_amount(compile_code,1);
            compile_code = string_kill_all_whitespace(compile_code);
            bool first = true;

            while(compile_code.substr(0,1)!=")" and function_code_prev!=compile_code){
                compile_code = string_kill_whitespace(compile_code);
                function_code_prev = compile_code;

                if(compile_code.substr(0,1)=="," and !first){
                    write(",",true);
                    compile_code = string_delete_amount(compile_code,1);
                }

                first = false;

                //Get Value Type
                if(code_parser.harvest_from_variable_value_type(compile_code,argument_type)==EXIT_FAILURE){
                    error_fatal("Couldn't Determine Type for Argument in Method '" + function_name + "'");
                    pend();
                    return EXIT_FAILURE;
                }

                //Handle Value
                if(code_parser.harvest_from_variable_value(compile_code,argument_type,",)")==EXIT_FAILURE){
                    return EXIT_FAILURE;
                }

                compile_code = string_kill_whitespace(compile_code);
            }

            if(function_code_prev==compile_code){
                error_fatal("Internal Function Error");
                pend();
                return EXIT_FAILURE;
            }

            compile_code = string_delete_amount(compile_code,1);
            code_parser.chop(compile_code);
        } else {
            error_fatal("The Method '" + function_name + "' does not exist.");
            pend();
            return EXIT_FAILURE;
        }
        continue;
    }

    //Is it a variable?
    if( is_identifier(string_get_until_or(compile_code," =+-/*.")) ){
        write_to = &ve_main_code;
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
                    if(code_parser.parse_function_from(compile_code,true,class_handler.find(raw_expression_type))==-1){
                        return EXIT_FAILURE;
                    }
                    prev_return_type = return_type;
                }

                if(compile_code.substr(0,1)=="."){
                    if(function_handler.exists(string_get_until_or(string_delete_amount(compile_code,1)," ("),S_NULL,S_NULL,class_handler.find(prev_return_type),SCOPETYPE_TEMPLATE) and prev_return_type!="none"){
                        return_type = function_handler.functions[function_handler.find(string_get_until_or(string_delete_amount(compile_code,1)," ("),S_NULL,S_NULL,class_handler.find(prev_return_type),SCOPETYPE_TEMPLATE)].type;
                        if(code_parser.parse_function_from(compile_code,true,class_handler.find(prev_return_type))==-1){
                            return EXIT_FAILURE;
                        }
                        prev_return_type = return_type;
                    } else {
                        if(prev_return_type!="none"){
                            error_fatal("Undeclared Method '" + string_get_until_or(string_delete_amount(compile_code,1)," (") + "' of template '" + prev_return_type + "'.");
                            pend();
                            return EXIT_FAILURE;
                        } else {
                            error_fatal("You Can't Call Methods of none");
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
            ve_main_code += "BOOMSLANG_String(\"" + rawstring + "\")";
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
                    ve_main_code += "BOOMSLANG_String(\"" + rawstring + "\")";
                    if(code_parser.parse_function_from(compile_code,true,class_handler.find("String"))==-1){
                        return EXIT_FAILURE;
                    }
                    prev_return_type = return_type;
                }

                if(compile_code.substr(0,1)=="."){
                    if(function_handler.exists(string_get_until_or(string_delete_amount(compile_code,1)," ("),S_NULL,S_NULL,class_handler.find(prev_return_type),SCOPETYPE_TEMPLATE) and prev_return_type!="none"){
                        return_type = function_handler.functions[function_handler.find(string_get_until_or(string_delete_amount(compile_code,1)," ("),S_NULL,S_NULL,class_handler.find(prev_return_type),SCOPETYPE_TEMPLATE)].type;
                        if(code_parser.parse_function_from(compile_code,true,class_handler.find(prev_return_type))==-1){
                            return EXIT_FAILURE;
                        }
                        prev_return_type = return_type;
                    } else {
                        if(prev_return_type!="none"){
                            error_fatal("Undeclared Function '" + string_get_until_or(string_delete_amount(compile_code,1)," (") + "' of template '" + prev_return_type + "'.");
                            pend();
                            return EXIT_FAILURE;
                        } else {
                            error_fatal("none does not have Methods");
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
            ve_main_code += "BOOMSLANG_Number(" + rawdecimal + ")";
            compile_code = string_kill_whitespace(compile_code);
            string return_type = "Decimal";
            string prev_return_type = "";

            while(compile_code.substr(0,1)=="." or compile_code.substr(0,1)==","){

                if(compile_code.substr(0,1)==","){
                    write(";\n",true);
                    return_type = function_handler.functions[function_handler.find(string_get_until_or(string_delete_amount(compile_code,1)," ("),S_NULL,S_NULL,class_handler.find("Decimal"),SCOPETYPE_TEMPLATE)].type;
                    ve_main_code += "BOOMSLANG_Number(" + rawdecimal + ")";
                    if(code_parser.parse_function_from(compile_code,true,class_handler.find("Decimal"))==-1){
                        return EXIT_FAILURE;
                    }
                    prev_return_type = return_type;
                }

                if(compile_code.substr(0,1)=="."){
                    if(function_handler.exists(string_get_until_or(string_delete_amount(compile_code,1)," ("),S_NULL,S_NULL,class_handler.find(prev_return_type),SCOPETYPE_TEMPLATE) and prev_return_type!="none"){
                        return_type = function_handler.functions[function_handler.find(string_get_until_or(string_delete_amount(compile_code,1)," ("),S_NULL,S_NULL,class_handler.find(prev_return_type),SCOPETYPE_TEMPLATE)].type;
                        if(code_parser.parse_function_from(compile_code,true,class_handler.find(prev_return_type))==-1){
                            return EXIT_FAILURE;
                        }
                        prev_return_type = return_type;
                    } else {
                        if(prev_return_type!="none"){
                            error_fatal("Undeclared Function '" + string_get_until_or(string_delete_amount(compile_code,1)," (") + "' of template '" + prev_return_type + "'.");
                            pend();
                            return EXIT_FAILURE;
                        } else {
                            error_fatal("You Can't Call Functions of none");
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
    }
}

//Internal Error?
if (compile_code==compile_prev and compile_code!=""){
    error_fatal("Internal Error");
    error_debug("Remaining code:\n" + compile_code + "\n-----------------------");
    pend();
    return EXIT_FAILURE;
}

//Block Error? (This error should never occur)
if(indentation!=0){
    error_fatal("Unbalanced Blocks");
    pend();
    return EXIT_FAILURE;
}

file_write << "int main(int argument_count, char** argument){\n" + ve_main_code + "\nreturn 0;\n}";
