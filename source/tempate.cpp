
#include <string>
#include <iostream>
#include "../include/globals.h"
#include "../include/resource.h"
#include "../include/base.h"
#include "../include/action.h"
#include "../include/function.h"
#include "../include/variable.h"

using namespace std;

int compile_template(int arg_count,char** args, unsigned int indentation,bool unique_template,string method_name,string template_name,string& write_template_buffer){
    string write_buffer = "";
    string init_buffer = "";
    string compile_prev;
    unsigned int before_indentation = indentation;
    indentation += 1;
    write_to = &write_template_buffer;

    ///Method Code Compile Loop
    while(compile_code!=compile_prev and indentation>0){
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
                write_buffer += "{\n";
            }
        }
        else if(indentation > new_indentation){
            while((indentation > new_indentation) and (indentation > before_indentation)){
                indentation--;
                write_buffer += "}\n";
            }
        }

        if(compile_code=="") continue;
        if(new_indentation<=before_indentation){
            for(int i = 0; i < new_indentation;i++){
                compile_code = "\t" + compile_code;
            }
            continue;
        }

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
                    write_template_buffer += compile_code.substr(0,1);
                    compile_code = string_delete_amount(compile_code,1);
                }
                else
                if(compile_code.substr(0,1)=="}"){
                    balance-=1;
                    write_template_buffer += compile_code.substr(0,1);
                    compile_code = string_delete_amount(compile_code,1);
                } else {
                    write_template_buffer += compile_code.substr(0,1);
                    compile_code = string_delete_amount(compile_code,1);
                }
            }

            compile_code = string_delete_amount(compile_code,1);
        }

        ///Check for before block clauses

        //else
        if(string_get_until_or(compile_code," \n")=="else"){
            error_debug("Found else statement");
            compile_code = string_delete_until_or(compile_code," \n");
            compile_code = string_kill_whitespace(compile_code);

            init_buffer += " else ";

            if(string_get_until(compile_code," ")=="if"){
                error_debug("Found if statement");
                compile_code = string_delete_until(compile_code," ");

                string expression;
                string type = S_NULL;

                if(code_harvest_value_type(compile_code,type,method_name,template_name)){
                    return EXIT_FAILURE;
                }

                if(code_harvest_raw_expression(compile_code,expression,type,method_name,template_name)){
                    return EXIT_FAILURE;
                }

                init_buffer += "if" + expression + ")";

                continue;
            }

            continue;
        }

        //if
        if(string_get_until(compile_code," ")=="if"){
            error_debug("Found if statement");
            compile_code = string_delete_until(compile_code," ");

            string expression;
            string type = S_NULL;

            if(code_harvest_value_type(compile_code,type,method_name,template_name)){
                return EXIT_FAILURE;
            }

            if(code_harvest_raw_expression(compile_code,expression,type,method_name,template_name)){
                return EXIT_FAILURE;
            }

            init_buffer += "if" + expression + ")";

            continue;
        }

        //Is it a keyword?
        if(ve_keywords.exists(string_get_until_or(compile_code," \n"))){
            //Method Declaration
            if(string_get_until_or(compile_code," \n")=="on"){
                error_debug("Found Method Declaration");
                //Method Declaration
                compile_code = string_delete_amount(compile_code,2);
                compile_code = string_kill_whitespace(compile_code);

                string buffer;
                string return_type = "none";

                string method_name = string_get_until_or(compile_code," (\n-");
                compile_code = string_delete_until_or(compile_code," (\n-");

                function_handler.add(method_name,"none","",class_handler.find(template_name),SCOPETYPE_TEMPLATE);

                //Expect opening parenthesis
                if(compile_code.substr(0,1)!="(" and compile_code.substr(0,1)!="\n" and compile_code.substr(0,2)!="->"){
                    error_fatal("Expected '(' or '->' or newline before '" + compile_code.substr(0,1) + "' in Method Argument Declaration");
                    pend();
                    return EXIT_FAILURE;
                }

                if(compile_code.substr(0,1)=="("){
                    write_to = &buffer;
                    if(code_parse_declaration_args(compile_code,method_name,template_name)==EXIT_FAILURE) return EXIT_FAILURE;
                } else {
                    buffer = "()";
                }

                compile_code = string_kill_whitespace(compile_code);

                if(compile_code.substr(0,2)=="->"){
                    compile_code = string_delete_amount(compile_code,2);
                    compile_code = string_kill_whitespace(compile_code);
                    return_type = string_get_until_or(compile_code," \n");
                    compile_code = string_delete_until_or(compile_code," \n");
                    compile_code = string_kill_whitespace(compile_code);
                    function_handler.functions[function_handler.find(method_name,S_NULL,S_NULL,class_handler.find(template_name),SCOPETYPE_TEMPLATE)].type = return_type;
                }

                if(compile_function(arg_count,args,indentation,method_name,template_name,return_type,write_buffer)==EXIT_FAILURE) return EXIT_FAILURE;

                if(return_type!="none"){
                    if(method_name=="")
                        file_write << resource(return_type) + " " + resource(method_name) + buffer + "{\n" + write_buffer;
                    else
                        write_template_buffer += resource(return_type) + " " + resource(method_name) + buffer + "{\n" + write_buffer;
                } else {
                    if(method_name=="")
                        file_write << "void " + resource(method_name) + buffer + "{\n" + write_buffer;
                    else
                        write_template_buffer += "void " + resource(method_name) + buffer + "{\n" + write_buffer;
                }
                continue;
            }
            //Static method declaration
            if(string_get_until_or(compile_code," \n")=="when"){
                error_debug("Found Static Method Declaration");
                //Method Declaration
                compile_code = string_delete_amount(compile_code,4);
                compile_code = string_kill_whitespace(compile_code);

                string buffer;
                string return_type = "none";

                string method_name = string_get_until_or(compile_code," (\n-");
                compile_code = string_delete_until_or(compile_code," (\n-");

                string prev_method_name = method_name;
                string prev_template_name = template_name;
                method_name = template_name + "." + method_name;
                template_name = "";
                function_handler.add(method_name,"none","",I_NULL,SCOPETYPE_GLOBAL);

                //Expect opening parenthesis
                if(compile_code.substr(0,1)!="(" and compile_code.substr(0,1)!="\n" and compile_code.substr(0,2)!="->"){
                    error_fatal("Expected '(' or '->' or newline before '" + compile_code.substr(0,1) + "' in Method Argument Declaration");
                    pend();
                    return EXIT_FAILURE;
                }

                if(compile_code.substr(0,1)=="("){
                    write_to = &buffer;
                    if(code_parse_declaration_args(compile_code,method_name,template_name)==EXIT_FAILURE) return EXIT_FAILURE;
                } else {
                    buffer = "()";
                }

                compile_code = string_kill_whitespace(compile_code);

                if(compile_code.substr(0,2)=="->"){
                    compile_code = string_delete_amount(compile_code,2);
                    compile_code = string_kill_whitespace(compile_code);
                    return_type = string_get_until_or(compile_code," \n");
                    compile_code = string_delete_until_or(compile_code," \n");
                    compile_code = string_kill_whitespace(compile_code);
                    function_handler.functions[function_handler.find(method_name,S_NULL,S_NULL,class_handler.find(template_name),SCOPETYPE_TEMPLATE)].type = return_type;
                }

                if(compile_function(arg_count,args,indentation,method_name,template_name,return_type,write_buffer)==EXIT_FAILURE) return EXIT_FAILURE;

                method_name = prev_method_name;
                template_name = prev_template_name;

                if(return_type!="none"){
                    if(method_name=="")
                        file_write << "static " + resource(return_type) + " " + resource(method_name) + buffer + "{\n" + write_buffer;
                    else
                        write_template_buffer += "static " + resource(return_type) + " " + resource(method_name) + buffer + "{\n" + write_buffer;
                } else {
                    if(method_name=="")
                        file_write << "static void " + resource(method_name) + buffer + "{\n" + write_buffer;
                    else
                        write_template_buffer += "static void " + resource(method_name) + buffer + "{\n" + write_buffer;
                }
                continue;
            }
            //Variable Declaration
            if(string_get_until_or(compile_code," ")=="new"){
                compile_code = string_delete_until_or(compile_code," ");

                compile_code = string_kill_whitespace(compile_code);

                string class_name = string_get_until_or(compile_code," \n");
                compile_code = string_delete_until_or(compile_code," \n");

                compile_code = string_kill_all_whitespace(compile_code);

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

                write_template_buffer += resource(class_name) + " " + resource(variable_name) + ";";
                variable_handler.add(variable_name,class_name,function_handler.find(method_name,S_NULL,S_NULL,I_NULL,SCOPETYPE_GLOBAL),SCOPETYPE_FUNCTION);
            }
        }

        //Is it a action?
        if( ve_actions.exists(string_get_until(compile_code," ")) ){
            error_debug("Found " + string_get_until(compile_code," ") + " to be a action.");

            string action_name = string_get_until(compile_code," ");
            compile_code = string_delete_until(compile_code," ");
            compile_code = string_kill_whitespace(compile_code);

            action(action_name);
            continue;
        }

        //Is it a function?
        if( is_identifier(string_get_until_or(compile_code,"(")) ){
            write_to = &init_buffer;
            error_debug("Found " + string_get_until_or(compile_code,"(") + " to be a function.");

            string function_name = string_get_until_or(compile_code,"(");
            compile_code = string_delete_until_or(compile_code,"(");

            if( function_handler.exists(function_name,S_NULL,S_NULL,I_NULL,SCOPETYPE_FUNCTION) ){

                init_buffer += resource(function_name) + "(";

                string function_code_prev;
                string argument_type;

                compile_code = string_delete_amount(compile_code,1);
                compile_code = string_kill_all_whitespace(compile_code);
                bool first = true;

                while(compile_code.substr(0,1)!=")" and function_code_prev!=compile_code){
                    compile_code = string_kill_whitespace(compile_code);
                    function_code_prev = compile_code;

                    if(compile_code.substr(0,1)=="," and !first){
                        init_buffer += ",";
                        compile_code = string_delete_amount(compile_code,1);
                    }

                    first = false;

                    //Get Value Type
                    if(code_harvest_value_type(compile_code,argument_type,method_name,template_name)==EXIT_FAILURE){
                        error_fatal("Couldn't Determine Type for Argument in Function '" + function_name + "'");
                        pend();
                        return EXIT_FAILURE;
                    }

                    //Handle Value
                    if(code_harvest_value(compile_code,argument_type,",)",method_name,template_name)==EXIT_FAILURE){
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
            } else {
                error_fatal("The Function '" + function_name + "' does not exist.");
                pend();
                return EXIT_FAILURE;
            }
            write_to = &write_template_buffer;
            continue;
        }

        //Is it a variable?
        if( is_identifier(string_get_until_or(compile_code," =+-/*.")) ){
            write_to = &write_template_buffer;
            method_name = "";
            /*template_name == template_name*/
            if(compile_variable(method_name,template_name,init_buffer)==EXIT_FAILURE) return EXIT_FAILURE;
            write_to = &write_template_buffer;
            continue;
        }

        //Is it a value?
        if( rawvalue_exists(compile_code) ){
            error_debug("Found raw value exists");
            write_to = &init_buffer;


            if(compile_code.substr(0,1)=="("){//Expression
                string raw_expression_type = S_NULL;
                string raw_expression;

                if (code_harvest_raw_expression(compile_code,raw_expression,raw_expression_type,"",template_name)==EXIT_FAILURE)
                    return EXIT_FAILURE;

                write_to = &init_buffer;

                init_buffer += raw_expression;
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
                        if(code_parse_function_from(compile_code,false,class_handler.find(prev_return_type),"",template_name)==EXIT_FAILURE){
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

                compile_code = string_kill_whitespace(compile_code);
                init_buffer += ";\n";
            }
            else if(compile_code.substr(0,1)=="\""){//String
                //String

                string rawstring = code_harvest_string(compile_code);
                init_buffer += "boomslang_String(\"" + rawstring + "\")";
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
                        if(code_parse_function_from(compile_code,false,class_handler.find(prev_return_type),"",template_name)==EXIT_FAILURE){
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

                compile_code = string_kill_whitespace(compile_code);
                init_buffer += ";\n";
            } else if(compile_code.substr(0,1)=="0" or compile_code.substr(0,1)=="1" or compile_code.substr(0,1)=="2" or compile_code.substr(0,1)=="3" or compile_code.substr(0,1)=="4" or compile_code.substr(0,1)=="5" or compile_code.substr(0,1)=="6" or compile_code.substr(0,1)=="7" or compile_code.substr(0,1)=="8"
            or compile_code.substr(0,1)=="9"){//Number
                //Decimal

                string rawdecimal = code_harvest_decimal(compile_code);
                init_buffer += "boomslang_Number(" + rawdecimal + ")";
                compile_code = string_kill_whitespace(compile_code);
                string return_type = "Decimal";
                string prev_return_type = "";

                while(compile_code.substr(0,1)=="."){
                    if(function_handler.exists(string_get_until_or(string_delete_amount(compile_code,1)," ("),S_NULL,S_NULL,class_handler.find(prev_return_type),SCOPETYPE_TEMPLATE) and prev_return_type!="none"){
                        return_type = function_handler.functions[function_handler.find(string_get_until_or(string_delete_amount(compile_code,1)," ("),S_NULL,S_NULL,class_handler.find(prev_return_type),SCOPETYPE_TEMPLATE)].type;
                        if(code_parse_function_from(compile_code,false,class_handler.find(prev_return_type),"",template_name)==EXIT_FAILURE){
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
                init_buffer += ";\n";
            }

            write_to = &write_template_buffer;
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

    if(unique_template)
        write_template_buffer = "\n" + write_template_buffer + template_name + "(){\n" + init_buffer + "}\n" ;
    else
        write_template_buffer = "\n" + write_template_buffer + resource(template_name) + "(){\n" + init_buffer + "}\n";
}
