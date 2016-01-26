
#include <string>
#include <iostream>
#include "../include/globals.h"
#include "../include/scripts.h"
#include "../include/file.h"
#include "../include/base.h"
#include "../include/resource.h"
#include "../include/function.h"
#include "../include/template.h"
#include "../include/action.h"
#include "../include/variable.h"

using namespace std;

int compile(int arg_count, char** arg, string& write_to){
    string compile_prev = "";
    string method_name;
    string clean_up;

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

        ///Check for before block clauses

        //else
        if(string_get_until_or(compile_code," \n")=="else"){
            error_debug("Found else statement");
            compile_code = string_delete_until_or(compile_code," \n");
            compile_code = string_kill_whitespace(compile_code);

            ve_main_code += " else ";

            if(string_get_until(compile_code," ")=="if"){
                error_debug("Found if statement");
                compile_code = string_delete_until(compile_code," ");

                string expression;
                string type = S_NULL;

                if(code_harvest_value_type(compile_code,type,"","")==EXIT_FAILURE) return EXIT_FAILURE;

                if(code_harvest_raw_expression(compile_code,expression,type,"","",ve_main_code)==EXIT_FAILURE) return EXIT_FAILURE;

                ve_main_code += "if" + expression + ")";

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

            if(code_harvest_value_type(compile_code,type,"","")==EXIT_FAILURE) return EXIT_FAILURE;

            if(code_harvest_raw_expression(compile_code,expression,type,"","",ve_main_code)==EXIT_FAILURE) return EXIT_FAILURE;

            ve_main_code += "if" + expression + ")";

            continue;
        }

        //try
        if(string_get_until_or(compile_code," \n")=="try"){
            error_debug("Found try statement");
            compile_code = string_delete_until_or(compile_code," \n");
            compile_code = string_kill_whitespace(compile_code);

            ve_main_code += "try ";
            continue;
        }

        //catch
        if(string_get_until_or(compile_code," \n")=="catch"){
            error_debug("Found catch statement");
            compile_code = string_delete_until_or(compile_code," \n");
            compile_code = string_kill_whitespace(compile_code);

            if(compile_code.substr(0,1)=="\n"){
                ve_main_code += "catch(...) ";
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
                    ve_main_code += "catch(" + resource(catch_template) + ") ";
                    continue;
                } else if(compile_code.substr(0,2)=="as"){//Catch into variable
                    compile_code = string_delete_amount(compile_code,2);
                    compile_code = string_kill_whitespace(compile_code);

                    string catch_variable = string_get_until_or(compile_code," \n");
                    compile_code = string_delete_until_or(compile_code," \n");
                    compile_code = string_kill_whitespace(compile_code);

                    ve_main_code += "catch(" + resource(catch_template) + " " + resource(catch_variable) + ") ";
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

            if(code_harvest_value_type(compile_code,type_of,"","")==EXIT_FAILURE){
                error_fatal("Couldn't Determine type for throw statement");
                pend();
                return EXIT_FAILURE;
            }

            if(code_harvest_value(compile_code,type_of,"","","",ve_main_code)==EXIT_FAILURE){
                return EXIT_FAILURE;
            }

            write_to += ";\n";
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
                string write_buffer;
                string return_type = "none";
                string method_name = string_get_until_or(compile_code," (\n-");
                string template_name = "";

                compile_code = string_delete_until_or(compile_code," (\n-");
                function_handler.add(method_name,"none","",I_NULL,SCOPETYPE_GLOBAL);

                compile_code = string_kill_whitespace(compile_code);

                //Expect opening parenthesis
                if(compile_code.substr(0,1)!="(" and compile_code.substr(0,1)!="\n" and compile_code.substr(0,2)!="->"){
                    error_fatal("Expected '(' or '->' or newline before '" + compile_code.substr(0,1) + "' in Method Argument Declaration");
                    pend();
                    return EXIT_FAILURE;
                }

                if(compile_code.substr(0,1)=="("){
                    //write_to = &buffer;
                    if(code_parse_declaration_args(compile_code,method_name,"",buffer)==EXIT_FAILURE) return EXIT_FAILURE;
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
                    function_handler.functions[function_handler.find(method_name,S_NULL,S_NULL,I_NULL,SCOPETYPE_GLOBAL)].type = return_type;
                }

                if(compile_function(arg_count,arg,indentation,method_name,template_name,return_type,write_buffer)==EXIT_FAILURE) return EXIT_FAILURE;

                write_buffer = write_buffer.substr(0,write_buffer.length()-1);
                file_write << "#define boomslangMethod" + to_string(next_method_id) + " {\\\n" << string_replace_all(write_buffer,"\n","\\\n") << endl;

                if(return_type!="none"){
                    file_write << resource(return_type) + " " + resource(method_name) + buffer + " " << "boomslangMethod" + to_string(next_method_id) + ";\n";
                } else {
                    file_write << "void " + resource(method_name) + buffer + " " << "boomslangMethod" + to_string(next_method_id) + ";\n";
                }
                next_method_id++;
                continue;
            }
            //Variable Declaration
            if(string_get_until_or(compile_code," ")=="new"){
                error_debug("Found new");
                compile_code = string_delete_until_or(compile_code," ");

                compile_code = string_kill_whitespace(compile_code);

                string class_name = string_get_until_or(compile_code," \n");
                compile_code = string_delete_until_or(compile_code," \n");

                compile_code = string_kill_all_whitespace(compile_code);

                string variable_name = string_get_until_or(compile_code," \n");
                compile_code = string_delete_until_or(compile_code," \n");

                string write_template_buffer;

                if(!class_handler.exists(class_name)){
                    error_fatal("Undeclared Template '" + class_name + "'");
                    pend();
                    return EXIT_FAILURE;
                }

                if(variable_handler.exists(variable_name,S_NULL,I_NULL,SCOPETYPE_MAIN)){
                    error_fatal("The variable '" + variable_name + "' was already declared");
                    pend();
                    return EXIT_FAILURE;
                }

                compile_code = string_kill_whitespace(compile_code);

                if(compile_code.substr(0,1)!="\n"){
                    error_fatal("Expected newline before '" + compile_code.substr(0,1) + "'");
                    pend();
                    return EXIT_FAILURE;
                }

                compile_code = string_kill_newline(compile_code);

                if(is_indent(compile_code)){
                    string template_name = "boomslangUniqueTemplate" + to_string(next_unique_template);
                    class_handler.add(template_name);

                    if(compile_template(arg_count,arg,indentation,true,"",template_name,write_template_buffer)==EXIT_FAILURE) return EXIT_FAILURE;

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
                string write_template_buffer;
                string parent_list = "";
                bool unique_template = false;

                if(class_handler.exists(template_name)){
                    error_fatal("The template '" + template_name + "' was already declared");
                    pend();
                    return EXIT_FAILURE;
                }

                compile_code = string_delete_until_or(compile_code," \n");
                compile_code = string_kill_whitespace(compile_code);

                while(compile_code.substr(0,1)!="\n"){
                    if(parent_list==""){
                        parent_list = ":";
                    }
                    parent_list += "public " + resource(string_get_until_or(compile_code," \n"));
                    compile_code = string_delete_until_or(compile_code," \n");
                    compile_code = string_kill_whitespace(compile_code);
                }

                class_handler.add(template_name);
                variable_handler.add("self",template_name,class_handler.find(template_name),SCOPETYPE_TEMPLATE);

                if(compile_template(arg_count,arg,indentation,false,"",template_name,write_template_buffer)==EXIT_FAILURE) return EXIT_FAILURE;

                file_write << "class " + resource(template_name) + parent_list + "{\npublic:\n" + write_template_buffer + "};\n";
                continue;
            }
        }

        //Is it a action?
        if( ve_actions.exists(string_get_until(compile_code," ")) ){
            error_debug("Found " + string_get_until(compile_code," ") + " to be a action.");

            string action_name = string_get_until(compile_code," ");
            compile_code = string_delete_until(compile_code," ");
            compile_code = string_kill_whitespace(compile_code);

            if(action(action_name)==EXIT_FAILURE) return EXIT_FAILURE;
            continue;
        }

        //Is it a method?
        if( is_identifier(string_get_until_or(compile_code,"(")) ){
            error_debug("Found " + string_get_until_or(compile_code,"(") + " to be a method.");

            string function_name = string_get_until_or(compile_code,"(");
            compile_code = string_delete_until_or(compile_code,"(");

            if( function_handler.exists(function_name,S_NULL,S_NULL,I_NULL,SCOPETYPE_GLOBAL) ){
                ve_main_code += resource(function_name) + "(";

                string function_code_prev;
                string argument_type;

                compile_code = string_delete_amount(compile_code,1);
                compile_code = string_kill_all_whitespace(compile_code);
                bool first = true;

                while(compile_code.substr(0,1)!=")" and function_code_prev!=compile_code){
                    compile_code = string_kill_whitespace(compile_code);
                    function_code_prev = compile_code;

                    if(compile_code.substr(0,1)=="," and !first){
                        ve_main_code += ",";
                        compile_code = string_delete_amount(compile_code,1);
                    }

                    first = false;

                    //Get Value Type
                    if(code_harvest_value_type(compile_code,argument_type,"","")==EXIT_FAILURE){
                        error_fatal("Couldn't Determine Type for Argument in Method '" + function_name + "'");
                        pend();
                        return EXIT_FAILURE;
                    }

                    //Handle Value
                    if(code_harvest_value(compile_code,argument_type,",)","","",ve_main_code)==EXIT_FAILURE) return EXIT_FAILURE;

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
            string method_name = "";
            string template_name = "";
            string init_buffer;
            if(compile_variable(method_name,template_name,init_buffer,clean_up,write_to)==EXIT_FAILURE) return EXIT_FAILURE;
            continue;
        }

        //Is it a value?
        if( rawvalue_exists(compile_code) ){
            error_debug("Found raw value exists");

            if(compile_code.substr(0,1)=="("){//Expression
                string raw_expression_type = S_NULL;
                string raw_expression;

                if (code_harvest_raw_expression(compile_code,raw_expression,raw_expression_type,"","",ve_main_code)==EXIT_FAILURE) return EXIT_FAILURE;

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
                        if(code_parse_function_from(compile_code,false,class_handler.find(prev_return_type),"","",ve_main_code)==EXIT_FAILURE) return EXIT_FAILURE;

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
                        if(code_parse_function_from(compile_code,false,class_handler.find(prev_return_type),"","",ve_main_code)==EXIT_FAILURE) return EXIT_FAILURE;

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
                        if(code_parse_function_from(compile_code,false,class_handler.find(prev_return_type),"","",ve_main_code)==EXIT_FAILURE) return EXIT_FAILURE;

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

    file_write << "int main(int argument_count, char** argument){\nargc = &argument_count;\nargv = &argument;\n" + ve_main_code + "\n" + clean_up + "\nreturn 0;\n}";
}
