
#include <iostream>
#include "../include/globals.h"
#include "../include/base.h"
#include "../include/management.h"
#include "../include/resource.h"
#include "../include/variable.h"

using namespace std;

int compile_variable(string method_name, string template_name, string& init_buffer, string& clean_up, string& write_to){
    error_debug("Found " + string_get_until_or(compile_code," =+-/*.") + " to be a variable.");

    string variable_name = string_get_until_or(compile_code," =+-/*.");
    string variable_buffer;

    compile_code = string_delete_until_or(compile_code," =+-/*.");
    compile_code = string_kill_whitespace(compile_code);

    if(compile_code.substr(0,1)=="."){
        error_debug("Found " + variable_name + " to contain a method.");

        if(class_handler.exists(variable_name)){}
        else if(template_name=="" and method_name==""){//Main scope
            if( !variable_handler.exists(variable_name,S_NULL,I_NULL,SCOPETYPE_MAIN,indentation) ){
                error_fatal("Undeclared Variable '" + variable_name + "'");
                pend();
                return EXIT_FAILURE;
            }
        } else if (template_name!="" and method_name==""){//Template non-methods
            //write_to = &init_buffer;
            if(!variable_handler.exists(variable_name,S_NULL,class_handler.find(template_name),SCOPETYPE_TEMPLATE)){
                error_fatal("Undeclared Variable '" + variable_name + "'");
                pend();
                return EXIT_FAILURE;
            }
        } else if (template_name!="" and method_name!=""){//Template method
            if(!variable_handler.exists(variable_name,S_NULL,function_handler.find(method_name,S_NULL,S_NULL,class_handler.find(template_name),SCOPETYPE_TEMPLATE),SCOPETYPE_FUNCTION) and !variable_handler.exists(variable_name,S_NULL,class_handler.find(template_name),SCOPETYPE_TEMPLATE)){
                error_fatal("Undeclared Variable '" + variable_name + "'");
                pend();
                return EXIT_FAILURE;
            }
        } else if (template_name=="" and method_name!=""){//Method
            if(!variable_handler.exists(variable_name,S_NULL,function_handler.find(method_name,S_NULL,S_NULL,I_NULL,SCOPETYPE_GLOBAL),SCOPETYPE_FUNCTION)){
                error_fatal("Undeclared Variable '" + variable_name + "'");
                pend();
                return EXIT_FAILURE;
            }
        }

        string return_type;

        if(class_handler.exists(variable_name)){}
        else if(method_name==""){
            if(template_name==""){
                //In main scope
                return_type = variable_handler.variables[variable_handler.find(variable_name,S_NULL,I_NULL,SCOPETYPE_MAIN)].type;
            } else {
                //In a template scope
                return_type = variable_handler.variables[variable_handler.find(variable_name,S_NULL,class_handler.find(template_name),SCOPETYPE_TEMPLATE)].type;
            }
        } else {
            if(template_name==""){
                //Non-template methods
                return_type = variable_handler.variables[variable_handler.find(variable_name,S_NULL,function_handler.find(method_name,S_NULL,S_NULL,I_NULL,SCOPETYPE_GLOBAL),SCOPETYPE_FUNCTION)].type;
            } else {
                //The variable is part of the template
                if(variable_handler.exists(variable_name,S_NULL,class_handler.find(template_name),SCOPETYPE_TEMPLATE)){
                    return_type = variable_handler.variables[variable_handler.find(variable_name,S_NULL,class_handler.find(template_name),SCOPETYPE_TEMPLATE)].type;
                } else {
                    //The variable is in a method of the template
                    return_type = variable_handler.variables[variable_handler.find(variable_name,S_NULL,function_handler.find(method_name,S_NULL,S_NULL,class_handler.find(template_name),SCOPETYPE_TEMPLATE),SCOPETYPE_FUNCTION)].type;
                }
            }
        }

        string prev_return_type = return_type;
        bool first = true;

        write_to += resource(variable_name);

        while(compile_code.substr(0,1)=="."){
            if(class_handler.exists(variable_name) and first){
                if(!function_handler.exists(variable_name + string_get_until_or(compile_code," ("),S_NULL,S_NULL,I_NULL,SCOPETYPE_GLOBAL)){
                    error_fatal("Undeclared method '" + variable_name + string_get_until_or(compile_code," (") + "'");
                    pend();
                    return EXIT_FAILURE;
                }

                string function_name = variable_name + string_get_until_or(compile_code,"(");
                string post_function_name = string_get_until_or(compile_code,"(");
                compile_code = string_delete_until_or(compile_code,"(");

                if( function_handler.exists(function_name,S_NULL,S_NULL,I_NULL,SCOPETYPE_GLOBAL) ){
                    write_to += "::" + resource(string_delete_amount(post_function_name,1)) + "(";

                    string function_code_prev;
                    string argument_type;

                    compile_code = string_delete_amount(compile_code,1);
                    compile_code = string_kill_all_whitespace(compile_code);
                    bool first_funcloop = true;

                    while(compile_code.substr(0,1)!=")" and function_code_prev!=compile_code){
                        compile_code = string_kill_whitespace(compile_code);
                        function_code_prev = compile_code;

                        if(compile_code.substr(0,1)=="," and !first){
                            write_to += ",";
                            compile_code = string_delete_amount(compile_code,1);
                        }

                        first_funcloop = false;

                        //Get Value Type
                        if(code_harvest_value_type(compile_code,argument_type,method_name,template_name)==EXIT_FAILURE){
                            error_fatal("Couldn't Determine Type for Argument in Method '" + function_name + "'");
                            pend();
                            return EXIT_FAILURE;
                        }

                        //Handle Value
                        if(code_harvest_value(compile_code,argument_type,",)",method_name,template_name,write_to)==EXIT_FAILURE) return EXIT_FAILURE;

                        compile_code = string_kill_whitespace(compile_code);
                    }

                    if(function_code_prev==compile_code){
                        error_fatal("Internal Function Error");
                        pend();
                        return EXIT_FAILURE;
                    }

                    compile_code = string_delete_amount(compile_code,1);
                    code_chop(compile_code);

                    write_to += ")";
                } else {
                    error_fatal("The Method '" + function_name + "' does not exist.");
                    pend();
                    return EXIT_FAILURE;
                }

                return_type = function_handler.functions[function_handler.find(function_name,S_NULL,S_NULL,I_NULL,SCOPETYPE_GLOBAL)].type;
                prev_return_type = return_type;
            }
            else if(function_handler.exists(string_get_until_or(string_delete_amount(compile_code,1)," ("),S_NULL,S_NULL,class_handler.find(prev_return_type),SCOPETYPE_TEMPLATE) and prev_return_type!="none"){
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
            first = false;
        }

        compile_code = string_kill_whitespace(compile_code);

        code_chop(compile_code);
        write_to += ";\n";
    }

    if(compile_code.substr(0,1)=="="){
        //Equals
        compile_code = string_delete_amount(compile_code,1);

        compile_code = string_kill_whitespace(compile_code);

        //Does the variable not exist?
        if(template_name=="" and method_name=="" and !variable_handler.exists(variable_name,S_NULL,I_NULL,SCOPETYPE_MAIN,indentation)){//Main scope
            if(compile_nonexisting_variable(variable_name,method_name,template_name,init_buffer,clean_up,write_to)==EXIT_FAILURE) return EXIT_FAILURE;
        } else if (template_name!="" and method_name!="" and !variable_handler.exists(variable_name,S_NULL,class_handler.find(template_name),SCOPETYPE_TEMPLATE)){//Template method
            if(compile_nonexisting_variable(variable_name,method_name,template_name,init_buffer,clean_up,write_to)==EXIT_FAILURE) return EXIT_FAILURE;
        } else if (template_name!="" and method_name=="" and !variable_handler.exists(variable_name,S_NULL,function_handler.find(method_name,S_NULL,S_NULL,class_handler.find(template_name),SCOPETYPE_TEMPLATE),SCOPETYPE_FUNCTION)){//Template non-methods
            if(compile_nonexisting_variable(variable_name,method_name,template_name,init_buffer,clean_up,write_to)==EXIT_FAILURE) return EXIT_FAILURE;
        }else if (template_name=="" and method_name!="" and !variable_handler.exists(variable_name,S_NULL,function_handler.find(method_name,S_NULL,S_NULL,I_NULL,SCOPETYPE_GLOBAL),SCOPETYPE_FUNCTION)){//Method
            if(compile_nonexisting_variable(variable_name,method_name,template_name,init_buffer,clean_up,write_to)==EXIT_FAILURE) return EXIT_FAILURE;
        } else {//Variable is Declared
            string variable_type;

            if(method_name==""){
                if(template_name==""){
                    //In main scope
                    variable_type = variable_handler.variables[variable_handler.find(variable_name,S_NULL,I_NULL,SCOPETYPE_MAIN)].type;
                } else {
                    //In a template scope
                    variable_type = variable_handler.variables[variable_handler.find(variable_name,S_NULL,class_handler.find(template_name),SCOPETYPE_TEMPLATE)].type;
                }
            } else {
                if(template_name==""){
                    //Non-template methods
                    variable_type = variable_handler.variables[variable_handler.find(variable_name,S_NULL,function_handler.find(method_name,S_NULL,S_NULL,I_NULL,SCOPETYPE_GLOBAL),SCOPETYPE_FUNCTION)].type;
                } else {
                    //The variable is part of the template
                    if(variable_handler.exists(variable_name,S_NULL,class_handler.find(template_name),SCOPETYPE_TEMPLATE)){
                        variable_type = variable_handler.variables[variable_handler.find(variable_name,S_NULL,class_handler.find(template_name),SCOPETYPE_TEMPLATE)].type;
                    } else {
                        //The variable is in a method of the template
                        variable_type = variable_handler.variables[variable_handler.find(variable_name,S_NULL,function_handler.find(method_name,S_NULL,S_NULL,class_handler.find(template_name),SCOPETYPE_TEMPLATE),SCOPETYPE_FUNCTION)].type;
                    }
                }
            }

            compile_code = string_kill_whitespace(compile_code);

            write_to += resource(variable_name) + "=";

            //Handle Value
            if(code_harvest_value(compile_code,variable_type,"",method_name,template_name,write_to)==EXIT_FAILURE){
                return EXIT_FAILURE;
            }

            code_chop(compile_code);

            write_to += ";\n";
        }
    }

    if(compile_code.substr(0,2)=="+="){
        //Plus
    }

    if(compile_code.substr(0,2)=="-="){
        //Minus
    }

    if(compile_code.substr(0,2)=="*="){
        //Times
    }

    if(compile_code.substr(0,2)=="/="){
        //Divide
    }
}

int compile_nonexisting_variable(string variable_name,string method_name, string template_name, string& init_buffer, string& clean_up, string& write_to){
    string variable_type = S_NULL;
    compile_code = string_kill_all_whitespace(compile_code);

    //Get Value Type
    if(code_harvest_value_type(compile_code,variable_type,method_name,template_name)==EXIT_FAILURE){
        error_fatal("Couldn't Determine Type for Variable '" + variable_name + "'");
        pend();
        return EXIT_FAILURE;
    }

    string value;

    {
        //write_to = &value;

        //Handle Value
        if(code_harvest_value(compile_code,variable_type,"",method_name,template_name,value)==EXIT_FAILURE){
            return EXIT_FAILURE;
        }
    }

    write_to += resource(variable_type) + "& " + resource(variable_name) + " = *(new " + resource(variable_type) + "(" + value + "));\n";
    clean_up += "delete &" + resource(variable_name) + ";\n";

    code_chop(compile_code);

    if(method_name==""){
        if(template_name==""){
            variable_handler.add(variable_name,variable_type,I_NULL,SCOPETYPE_MAIN,indentation);
        } else {
            variable_handler.add(variable_name,variable_type,class_handler.find(template_name),SCOPETYPE_TEMPLATE);
        }
    } else {
        if(template_name==""){
            variable_handler.add(variable_name,variable_type,function_handler.find(method_name,S_NULL,S_NULL,class_handler.find(template_name),SCOPETYPE_TEMPLATE),SCOPETYPE_FUNCTION);
        } else {
            variable_handler.add(variable_name,variable_type,function_handler.find(method_name,S_NULL,S_NULL,I_NULL,SCOPETYPE_GLOBAL),SCOPETYPE_FUNCTION);
        }
    }
}
