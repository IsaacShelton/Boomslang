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
string method_name;

///Main Compile Loop
while(compile_code!="" and compile_code!=compile_prev){
    compile_code = string_kill_newline(compile_code);
    unsigned int new_indentation = 0;

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
        //Method Declaration
        if(string_get_until_or(compile_code," \n")=="on"){
            error_debug("Found Function Declaration");
            //Function Declaration
            compile_code = string_delete_amount(compile_code,2);
            compile_code = string_kill_whitespace(compile_code);
            string buffer;

            string method_name = string_get_until_or(compile_code," (");
            compile_code = string_delete_until_or(compile_code," (");
            function_handler.add(method_name,"none","",I_NULL,SCOPETYPE_GLOBAL);

            //Expect opening parenthesis
            if(compile_code.substr(0,1)!="("){
                error_fatal("Expected '(' before '" + compile_code.substr(0,1) + "' in Method Argument Declaration");
                pend();
                return EXIT_FAILURE;
            }

            write_to = &buffer;
            if(code_parse_declaration_args(compile_code,method_name)==EXIT_FAILURE){
                return EXIT_FAILURE;
            }

            #include "compile function.h"

            if(return_type!="none"){
                file_write << resource(return_type) + " " + resource(method_name) + buffer + "{\n" << write_buffer << "}\n";
            } else {
                file_write << "void " + resource(method_name) + buffer + "{\n" << write_buffer << "}\n";
            }
            continue;
        }
        //Variable Declaration
        if(string_get_until_or(compile_code," ")=="new"){
            compile_code = string_delete_until_or(compile_code," ");

            compile_code = string_kill_whitespace(compile_code);

            string class_name = string_get_until_or(compile_code," ");
            compile_code = string_delete_until_or(compile_code," ");

            compile_code = string_kill_whitespace(compile_code);

            string variable_name = string_get_until_or(compile_code," \n");
            compile_code = string_delete_until_or(compile_code," \n");

            if(!class_handler.exists(class_name)){
                error_fatal("Undeclared Template '" + class_name + "'");
                pend();
                return EXIT_FAILURE;
            }

            compile_code = string_kill_whitespace(compile_code);

            code_chop(compile_code);

            compile_code = string_kill_whitespace(compile_code);
            compile_code = string_kill_newline(compile_code);

            if(is_indent(compile_code)){
                string template_name = "boomslangUniqueTemplate" + to_string(next_unique_template);
                bool unique_template = true;

                #include "compile template.h"

                file_write << "class boomslangUniqueTemplate" + to_string(next_unique_template) + ":public " + resource(class_name) + "{\npublic:\n" + write_template_buffer + "};\n";
                ve_main_code += "boomslangUniqueTemplate" + to_string(next_unique_template) + " " + resource(variable_name) + ";";

                variable_handler.add(variable_name,class_name,I_NULL,SCOPETYPE_MAIN,indentation);
                next_unique_template += 1;
                continue;
            } else {
                ve_main_code += resource(class_name) + " " + resource(variable_name) + ";\n";
                variable_handler.add(variable_name,class_name,I_NULL,SCOPETYPE_MAIN,indentation);
            }
        }
        //Template Declaration
        if(string_get_until_or(compile_code," ")=="template"){
            error_debug("Found Template Declaration");
            compile_code = string_delete_until_or(compile_code," ");
            compile_code = string_kill_whitespace(compile_code);

            string template_name = string_get_until_or(compile_code," \n");
            bool unique_template = false;
            compile_code = string_delete_until_or(compile_code," \n");
            compile_code = string_kill_whitespace(compile_code);
            class_handler.add(template_name);

            #include "compile template.h"

            file_write << "class " + resource(template_name) + "{\npublic:\n" + write_template_buffer + "};\n";
            continue;
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
        error_debug("Found " + string_get_until_or(compile_code,"(") + " to be a method.");

        string function_name = string_get_until_or(compile_code,"(");
        compile_code = string_delete_until_or(compile_code,"(");

        if( function_handler.exists(function_name,S_NULL,S_NULL,I_NULL,SCOPETYPE_GLOBAL) ){
            write_to = &ve_main_code;
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
                if(code_harvest_value_type(compile_code,argument_type)==EXIT_FAILURE){
                    error_fatal("Couldn't Determine Type for Argument in Method '" + function_name + "'");
                    pend();
                    return EXIT_FAILURE;
                }

                //Handle Value
                if(code_harvest_value(compile_code,argument_type,",)")==EXIT_FAILURE){
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
            code_chop(compile_code);

            ve_main_code += ");\n";
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
        string method_name = "";
        #include "variable.h"
    }

    //Is it a value?
    if( rawvalue_exists(compile_code) ){
        error_debug("Found raw value exists");
        write_to = &ve_main_code;

        if(compile_code.substr(0,1)=="("){//Expression
            string raw_expression_type = S_NULL;
            string raw_expression;

            if (code_harvest_raw_expression(compile_code,raw_expression,raw_expression_type)==EXIT_FAILURE)
                return EXIT_FAILURE;

            write_to = &ve_main_code;

            ve_main_code += raw_expression;
            compile_code = string_kill_whitespace(compile_code);
            string prev_return_type = raw_expression_type;
            string return_type = "";

            if(compile_code.substr(0,1)!="."){
                error_fatal("Expected '.' before '" + compile_code.substr(0,1) + "' after expression.");
                pend();
                return EXIT_FAILURE;
            }

            while(compile_code.substr(0,1)=="."){
                if(function_handler.exists(string_get_until_or(string_delete_amount(compile_code,1)," ("),S_NULL,S_NULL,class_handler.find(prev_return_type),SCOPETYPE_TEMPLATE) and prev_return_type!="none"){
                    return_type = function_handler.functions[function_handler.find(string_get_until_or(string_delete_amount(compile_code,1)," ("),S_NULL,S_NULL,class_handler.find(prev_return_type),SCOPETYPE_TEMPLATE)].type;
                    if(code_parse_function_from(compile_code,true,class_handler.find(prev_return_type))==-1){
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
            compile_code = string_kill_whitespace(compile_code);

            code_chop(compile_code);

            compile_code = string_kill_whitespace(compile_code);
            write(";\n",true);
        }
        else if(compile_code.substr(0,1)=="\""){//String
            //String

            string rawstring = code_harvest_string(compile_code);
            ve_main_code += "boomslang_String(\"" + rawstring + "\")";
            compile_code = string_kill_whitespace(compile_code);
            string prev_return_type = "String";
            string return_type = "";

            if(compile_code.substr(0,1)!="."){
                error_fatal("Expected '.' before '" + compile_code.substr(0,1) + "' after \"" + rawstring + "\".");
                pend();
                return EXIT_FAILURE;
            }

            while(compile_code.substr(0,1)=="."){
                if(function_handler.exists(string_get_until_or(string_delete_amount(compile_code,1)," ("),S_NULL,S_NULL,class_handler.find(prev_return_type),SCOPETYPE_TEMPLATE) and prev_return_type!="none"){
                    return_type = function_handler.functions[function_handler.find(string_get_until_or(string_delete_amount(compile_code,1)," ("),S_NULL,S_NULL,class_handler.find(prev_return_type),SCOPETYPE_TEMPLATE)].type;
                    if(code_parse_function_from(compile_code,true,class_handler.find(prev_return_type))==EXIT_FAILURE){
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

            compile_code = string_kill_whitespace(compile_code);

            code_chop(compile_code);

            compile_code = string_kill_whitespace(compile_code);
            write(";\n",true);
        } else if(compile_code.substr(0,1)=="0" or compile_code.substr(0,1)=="1" or compile_code.substr(0,1)=="2" or compile_code.substr(0,1)=="3" or compile_code.substr(0,1)=="4" or compile_code.substr(0,1)=="5" or compile_code.substr(0,1)=="6" or compile_code.substr(0,1)=="7" or compile_code.substr(0,1)=="8"
        or compile_code.substr(0,1)=="9"){//Number
            //Decimal

            string rawdecimal = code_harvest_decimal(compile_code);
            ve_main_code += "boomslang_Number(" + rawdecimal + ")";
            compile_code = string_kill_whitespace(compile_code);
            string return_type = "Decimal";
            string prev_return_type = "";

            while(compile_code.substr(0,1)=="."){
                if(function_handler.exists(string_get_until_or(string_delete_amount(compile_code,1)," ("),S_NULL,S_NULL,class_handler.find(prev_return_type),SCOPETYPE_TEMPLATE) and prev_return_type!="none"){
                    return_type = function_handler.functions[function_handler.find(string_get_until_or(string_delete_amount(compile_code,1)," ("),S_NULL,S_NULL,class_handler.find(prev_return_type),SCOPETYPE_TEMPLATE)].type;
                    if(code_parse_function_from(compile_code,true,class_handler.find(prev_return_type))==-1){
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

            compile_code = string_kill_whitespace(compile_code);

            code_chop(compile_code);

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
