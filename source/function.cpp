
#include <string>
#include <iostream>
#include "../include/globals.h"
#include "../include/resource.h"
#include "../include/base.h"
#include "../include/action.h"
#include "../include/variable.h"

using namespace std;

int compile_function(int arg_count,char** args, unsigned int indentation,string method_name,string template_name,string& return_type,string& write_to){
    unsigned int before_indentation = indentation;
    string compile_prev;
    string clean_up;
    write_to = "";
    indentation += 1;

    ///Method Code Compile Loop
    while(compile_code!=compile_prev and indentation>before_indentation){
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
                write_to += "{\n";
            }
        }
        else if(indentation > new_indentation){
            while((indentation > new_indentation) and (indentation > before_indentation)){
                indentation--;
                if (indentation == before_indentation) write_to += clean_up;
                write_to += "}\n";
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
        if(compile_code.substr(0,1)=="#"){
            compile_code = string_delete_amount(compile_code,1);
            compile_code = string_delete_until(compile_code,"\n");
        }

        //Is is a multi-line-comment?
        if(compile_code.substr(0,2)=="/#"){
            compile_code = string_delete_amount(compile_code,2);
            compile_code = string_delete_until(compile_code,"#");
            compile_code = string_delete_amount(compile_code,1);
        }

        //else
        if(string_get_until_or(compile_code," \n")=="else"){
            error_debug("Found else statement");
            compile_code = string_delete_until_or(compile_code," \n");
            compile_code = string_kill_whitespace(compile_code);

            write_to += " else ";

            if(string_get_until(compile_code," ")=="if"){
                error_debug("Found if statement");
                compile_code = string_delete_until(compile_code," ");

                string expression;
                string type = S_NULL;

                if(code_harvest_value_type(compile_code,type,method_name,template_name,indentation)){
                    return EXIT_FAILURE;
                }

                if(code_harvest_raw_expression(compile_code,expression,type,method_name,template_name,write_to)){
                    return EXIT_FAILURE;
                }

                write_to += "if" + expression + ")";

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

            if(code_harvest_value_type(compile_code,type,method_name,template_name,indentation)){
                return EXIT_FAILURE;
            }

            if(code_harvest_raw_expression(compile_code,expression,type,method_name,template_name,write_to)){
                return EXIT_FAILURE;
            }

            write_to += "if" + expression + ")";

            continue;
        }

        //while
        if(string_get_until(compile_code," ")=="while"){
            error_debug("Found while statement");
            compile_code = string_delete_until(compile_code," ");

            string expression;
            string type = S_NULL;

            if(code_harvest_value_type(compile_code,type,method_name,template_name,indentation)){
                return EXIT_FAILURE;
            }

            if(code_harvest_raw_expression(compile_code,expression,type,method_name,template_name,write_to)){
                return EXIT_FAILURE;
            }

            write_to += "while" + expression + ")";

            continue;
        }

        //try
        if(string_get_until_or(compile_code," \n")=="try"){
            error_debug("Found try statement");
            compile_code = string_delete_until_or(compile_code," \n");
            compile_code = string_kill_whitespace(compile_code);

            write_to += "try ";
            continue;
        }

        //catch
        if(string_get_until_or(compile_code," \n")=="catch"){
            error_debug("Found catch statement");
            compile_code = string_delete_until_or(compile_code," \n");
            compile_code = string_kill_whitespace(compile_code);

            if(compile_code.substr(0,1)=="\n"){
                write_to += "catch(...) ";
                continue;
            }
            else {//Catch specific template
                string catch_template = string_get_until_or(compile_code," \n");
                compile_code = string_delete_until_or(compile_code," \n");
                compile_code = string_kill_whitespace(compile_code);

                if(!class_handler.exists(catch_template)){
                    error_fatal("Undeclared Template '" + catch_template + "'");
                    pend();
                    return EXIT_FAILURE;
                }

                if(compile_code.substr(0,1)=="\n"){
                    write_to += "catch(" + resource(catch_template) + ") ";
                    continue;
                } else if(compile_code.substr(0,2)=="as"){//Catch into variable
                    compile_code = string_delete_amount(compile_code,2);
                    compile_code = string_kill_whitespace(compile_code);

                    string catch_variable = string_get_until_or(compile_code," \n");
                    compile_code = string_delete_until_or(compile_code," \n");
                    compile_code = string_kill_whitespace(compile_code);

                    write_to += "catch(" + resource(catch_template) + " " + resource(catch_variable) + ") ";
                    variable_handler.add(catch_variable,catch_template,I_NULL,SCOPETYPE_MAIN);
                    continue;
                } else {
                    error_fatal("Expected 'as' before '" + compile_code.substr(0,1) + "' after template in catch statement");
                    pend();
                    return EXIT_FAILURE;
                }

                continue;
            }
        }

        //throw
        if(string_get_until_or(compile_code," \n")=="throw"){
            error_debug("Found throw statement");
            compile_code = string_delete_until_or(compile_code," \n");
            compile_code = string_kill_whitespace(compile_code);
            string type_of;

            write_to += "throw ";

            if(code_harvest_value_type(compile_code,type_of,"","",indentation)==EXIT_FAILURE){
                error_fatal("Couldn't Determine type for throw statement");
                pend();
                return EXIT_FAILURE;
            }

            if(code_harvest_value(compile_code,type_of,"","","",indentation,write_to)==EXIT_FAILURE){
                return EXIT_FAILURE;
            }

            write_to += ";\n";
        }

        //Is it a keyword?
        if(ve_keywords.exists(string_get_until_or(compile_code," \n"))){
            //Variable Declaration
            if(string_get_until_or(compile_code," ")=="new"){
                compile_code = string_delete_until_or(compile_code," ");

                compile_code = string_kill_whitespace(compile_code);

                string class_name = string_get_until_or(compile_code," \n(.");
                compile_code = string_delete_until_or(compile_code," \n(.");

                compile_code = string_kill_all_whitespace(compile_code);

                if(compile_code.substr(0,1)=="." or compile_code.substr(0,1)=="("){
                    if(!class_handler.exists(class_name)){
                        error_fatal("Undeclared Template '" + class_name + "'");
                        pend();
                        return EXIT_FAILURE;
                    }

                    write_to += resource(class_name);
                    compile_code = string_kill_whitespace(compile_code);

                    if(compile_code.substr(0,1)!="("){
                        write_to += "()";
                    } else {
                        code_parse_args(compile_code,method_name,template_name,write_to);
                    }

                    compile_code = string_kill_whitespace(compile_code);

                    if(compile_code.substr(0,1)=="."){
                        string return_type = class_name;
                        string prev_return_type = class_name;

                        while(compile_code.substr(0,1)=="."){
                            if(function_handler.exists(string_get_until_or(string_delete_amount(compile_code,1)," ("),S_NULL,S_NULL,class_handler.find(prev_return_type),SCOPETYPE_TEMPLATE) and prev_return_type!="none"){
                                return_type = function_handler.functions[function_handler.find(string_get_until_or(string_delete_amount(compile_code,1)," ("),S_NULL,S_NULL,class_handler.find(prev_return_type),SCOPETYPE_TEMPLATE)].type;

                                if(code_parse_function_from(compile_code,true,class_handler.find(prev_return_type),method_name,template_name,write_to)==EXIT_FAILURE){
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

                        write_to += ";\n";
                    }
                } else {
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

                    write_to += resource(class_name) + " " + resource(variable_name) + ";";
                    variable_handler.add(variable_name,class_name,function_handler.find(method_name,S_NULL,S_NULL,I_NULL,SCOPETYPE_GLOBAL),SCOPETYPE_FUNCTION);
                }
            }
        }

        //Return statement?
        if(compile_code.substr(0,7)=="return "){
            compile_code = string_delete_amount(compile_code,7);

            string new_return_type;

            if(code_harvest_value_type(compile_code,new_return_type,method_name,template_name,indentation)==EXIT_FAILURE){
                error_fatal("Couldn't Determine return type for method '" + method_name + "'");
                pend();
                return EXIT_FAILURE;
            }

            if(template_name==""){
                if( function_handler.functions[function_handler.find(method_name,S_NULL,S_NULL,I_NULL,SCOPETYPE_GLOBAL)].type=="none" or function_handler.functions[function_handler.find(method_name,S_NULL,S_NULL,I_NULL,SCOPETYPE_GLOBAL)].type==new_return_type ){
                    function_handler.functions[function_handler.find(method_name,S_NULL,S_NULL,I_NULL,SCOPETYPE_GLOBAL)].type = new_return_type;
                    return_type = new_return_type;

                    write_to += "return ";

                    if(code_harvest_value(compile_code,new_return_type,"",method_name,template_name,indentation,write_to)==EXIT_FAILURE){
                        return EXIT_FAILURE;
                    }
                    write_to += ";\n";
                    code_chop(compile_code);
                    continue;
                } else {
                    error_fatal("Conflicting return types '" + new_return_type + "' and '" + function_handler.functions[function_handler.find(method_name,S_NULL,S_NULL,I_NULL,SCOPETYPE_GLOBAL)].type + "'");
                    pend();
                    return EXIT_FAILURE;
                }
            } else {
                if( function_handler.functions[function_handler.find(method_name,S_NULL,S_NULL,class_handler.find(template_name),SCOPETYPE_TEMPLATE)].type=="none" or function_handler.functions[function_handler.find(method_name,S_NULL,S_NULL,class_handler.find(template_name),SCOPETYPE_TEMPLATE)].type==new_return_type ){
                    function_handler.functions[function_handler.find(method_name,S_NULL,S_NULL,class_handler.find(template_name),SCOPETYPE_TEMPLATE)].type = new_return_type;
                    return_type = new_return_type;

                    write_to += "return ";

                    if(code_harvest_value(compile_code,new_return_type,"",method_name,template_name,indentation,write_to)==EXIT_FAILURE){
                        return EXIT_FAILURE;
                    }
                    write_to += ";\n";
                    code_chop(compile_code);
                    continue;
                } else {
                    error_fatal("Conflicting return types '" + new_return_type + "' and '" + function_handler.functions[function_handler.find(method_name,S_NULL,S_NULL,class_handler.find(template_name),SCOPETYPE_TEMPLATE)].type + "'");
                    pend();
                    return EXIT_FAILURE;
                }
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
            error_debug("Found " + string_get_until_or(compile_code,"(") + " to be a function.");

            string function_name = string_get_until_or(compile_code,"(");
            compile_code = string_delete_until_or(compile_code,"(");

            if( function_handler.exists(function_name,S_NULL,S_NULL,I_NULL,SCOPETYPE_GLOBAL) ){

                write_to += resource(function_name) + "(";

                string function_code_prev;
                string argument_type;

                compile_code = string_delete_amount(compile_code,1);
                compile_code = string_kill_all_whitespace(compile_code);
                bool first = true;

                while(compile_code.substr(0,1)!=")" and function_code_prev!=compile_code){
                    compile_code = string_kill_whitespace(compile_code);
                    function_code_prev = compile_code;

                    if(compile_code.substr(0,1)=="," and !first){
                        write_to += ",";
                        compile_code = string_delete_amount(compile_code,1);
                    }

                    first = false;

                    //Get Value Type
                    if(code_harvest_value_type(compile_code,argument_type,method_name,template_name,indentation)==EXIT_FAILURE){
                        error_fatal("Couldn't Determine Type for Argument in Function '" + function_name + "'");
                        pend();
                        return EXIT_FAILURE;
                    }

                    //Handle Value
                    if(code_harvest_value(compile_code,argument_type,",)",method_name,template_name,indentation,write_to)==EXIT_FAILURE){
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

                write_to += ");\n";
            } else {
                error_fatal("The Function '" + function_name + "' does not exist.");
                pend();
                return EXIT_FAILURE;
            }
            continue;
        }

        //Is it a variable?
        if( is_identifier(string_get_until_or(compile_code," =+-/*.")) ){
            string init_buffer;
            if(compile_variable(method_name,template_name,init_buffer,clean_up,indentation,write_to)==EXIT_FAILURE) return EXIT_FAILURE;
            continue;
        }

        //Is it a value?
        if( rawvalue_exists(compile_code) ){
            error_debug("Found raw value exists");

            if(compile_code.substr(0,1)=="("){//Expression
                error_debug("Found expression");
                string raw_expression_type = S_NULL;
                string raw_expression;

                if (code_harvest_raw_expression(compile_code,raw_expression,raw_expression_type,method_name,template_name,write_to)==EXIT_FAILURE){
                    return EXIT_FAILURE;
                }

                write_to += raw_expression;
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
                        if(code_parse_function_from(compile_code,false,class_handler.find(prev_return_type),method_name,template_name,write_to)==EXIT_FAILURE){
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
                write_to += ";\n";
            }
            else if(compile_code.substr(0,1)=="\""){//String
                //String

                string rawstring = code_harvest_string(compile_code);
                write_to += "boomslang_String(\"" + rawstring + "\")";
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
                        if(code_parse_function_from(compile_code,false,class_handler.find(prev_return_type),method_name,template_name,write_to)==EXIT_FAILURE){
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
                write_to += ";\n";
            } else if(compile_code.substr(0,1)=="0" or compile_code.substr(0,1)=="1" or compile_code.substr(0,1)=="2" or compile_code.substr(0,1)=="3" or compile_code.substr(0,1)=="4" or compile_code.substr(0,1)=="5" or compile_code.substr(0,1)=="6" or compile_code.substr(0,1)=="7" or compile_code.substr(0,1)=="8"
            or compile_code.substr(0,1)=="9"){//Number
                //Decimal

                string rawdecimal = code_harvest_decimal(compile_code);
                write_to += "boomslang_Number(" + rawdecimal + ")";
                compile_code = string_kill_whitespace(compile_code);
                string return_type = "Decimal";
                string prev_return_type = "";

                while(compile_code.substr(0,1)=="."){
                    if(function_handler.exists(string_get_until_or(string_delete_amount(compile_code,1)," ("),S_NULL,S_NULL,class_handler.find(prev_return_type),SCOPETYPE_TEMPLATE) and prev_return_type!="none"){
                        return_type = function_handler.functions[function_handler.find(string_get_until_or(string_delete_amount(compile_code,1)," ("),S_NULL,S_NULL,class_handler.find(prev_return_type),SCOPETYPE_TEMPLATE)].type;
                        if(code_parse_function_from(compile_code,false,class_handler.find(prev_return_type),method_name,template_name,write_to)==EXIT_FAILURE){
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
                write_to += ";\n";
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
    if(indentation>before_indentation){
        error_fatal("Unbalanced Blocks");
        pend();
        return EXIT_FAILURE;
    }
}
