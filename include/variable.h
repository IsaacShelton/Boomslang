/**
    ============ variable.h ============
    Code for Variable Compiling

    This file should NOT be included
    anywhere besides from init.h when
    variable is found.

    This Header does NOT contain a
    header guard so include with
    cation.
*/

using namespace std;

error_debug("Found " + string_get_until_or(compile_code," =+-/*.") + " to be a variable.");

string variable_name = string_get_until_or(compile_code," =+-/*.");
string variable_buffer;

compile_code = string_delete_until_or(compile_code," =+-/*.");
compile_code = string_kill_all_whitespace(compile_code);

if(compile_code.substr(0,1)=="."){
    error_debug("Found " + variable_name + " to contain a method.");

    if(template_name=="" and method_name==""){//Main scope
        if( !variable_handler.exists(variable_name,S_NULL,I_NULL,SCOPETYPE_MAIN,indentation) ){
            error_fatal("Undeclared Variable '" + variable_name + "'");
            pend();
            return EXIT_FAILURE;
        }
    } else if (template_name!="" and method_name==""){//Template non-methods
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

    if(method_name==""){
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

    *write_to += resource(variable_name);

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
                error_fatal("You Can't Call Functions of none");
                pend();
                return EXIT_FAILURE;
            }
        }
    }

    compile_code = string_kill_whitespace(compile_code);

    code_chop(compile_code);
    *write_to += ";\n";
}

if(compile_code.substr(0,1)=="="){
    //Equals
    compile_code = string_delete_amount(compile_code,1);

    compile_code = string_kill_whitespace(compile_code);

    //Does the variable not exist?
    if(template_name=="" and method_name==""){//Main scope
        if( !variable_handler.exists(variable_name,S_NULL,I_NULL,SCOPETYPE_MAIN,indentation) ){
            #include "variable not exist.h"
        }
    } else if (template_name!="" and method_name!=""){//Template method
        if(!variable_handler.exists(variable_name,S_NULL,class_handler.find(template_name),SCOPETYPE_TEMPLATE)){
            #include "variable not exist.h"
        }
    } else if (template_name!="" and method_name==""){//Template non-methods
        if(!variable_handler.exists(variable_name,S_NULL,function_handler.find(method_name,S_NULL,S_NULL,class_handler.find(template_name),SCOPETYPE_TEMPLATE),SCOPETYPE_FUNCTION)){
            #include "variable not exist.h"
        }
    }else if (template_name=="" and method_name!=""){//Method
        if(!variable_handler.exists(variable_name,S_NULL,function_handler.find(method_name,S_NULL,S_NULL,I_NULL,SCOPETYPE_GLOBAL),SCOPETYPE_FUNCTION)){
            #include "variable not exist.h"
        }
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

        *write_to += resource(variable_name) + "=";

        //Handle Value
        if(code_harvest_value(compile_code,variable_type,"",method_name,template_name)==EXIT_FAILURE){
            return EXIT_FAILURE;
        }

        code_chop(compile_code);

        *write_to += ";\n";
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

continue;
