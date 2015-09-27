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

compile_code = string_delete_until_or(compile_code," =+-/*.");

compile_code = string_kill_whitespace(compile_code);

if(compile_code.substr(0,1)=="."){
    error_debug("Found " + variable_name + " to contain a method.");
    string return_type = variable_handler.variables[variable_handler.find(variable_name,S_NULL,I_NULL,SCOPETYPE_MAIN)].type;
    string prev_return_type = return_type;
    ve_main_code += resource(variable_name);

    while(compile_code.substr(0,1)=="." or compile_code.substr(0,1)==","){

        if(compile_code.substr(0,1)==","){
            if(variable_handler.exists(variable_name,S_NULL,I_NULL,SCOPETYPE_MAIN)){
                write(";\n",true);
                return_type = function_handler.functions[function_handler.find(string_get_until_or(string_delete_amount(compile_code,1)," ("),S_NULL,S_NULL,class_handler.find( variable_handler.variables[variable_handler.find(variable_name,S_NULL,I_NULL,SCOPETYPE_MAIN)].type ),SCOPETYPE_TEMPLATE)].type;
                ve_main_code += resource(variable_name);
                if(code_parser.parse_function_from(compile_code,true,true,class_handler.find( variable_handler.variables[variable_handler.find(variable_name,S_NULL,I_NULL,SCOPETYPE_MAIN)].type ))==-1){
                    return EXIT_FAILURE;
                }
                prev_return_type = return_type;
            } else {
                error_fatal("Undeclared Variable " + variable_name + ".");
                pend();
                return EXIT_FAILURE;
            }
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

    ve_main_code += ";\n";
}

if(compile_code.substr(0,1)=="="){
    //Equals
    compile_code = string_delete_amount(compile_code,1);

    compile_code = string_kill_whitespace(compile_code);

    if(!variable_handler.exists(variable_name,S_NULL,I_NULL,SCOPETYPE_MAIN)){//Not Declared Yet
        string variable_type = S_NULL;
        compile_code = string_kill_whitespace(compile_code);

        //Get Value Type
        if(code_parser.harvest_from_variable_value_type(compile_code,variable_type)==EXIT_FAILURE){
            return EXIT_FAILURE;
        }

        ve_main_code += resource(variable_type) + " " + resource(variable_name) + "=";

        //Handle Value
        if(code_parser.harvest_from_variable_value(compile_code,variable_type,true)==EXIT_FAILURE){
            return EXIT_FAILURE;
        }

        code_parser.chop(compile_code);

        ve_main_code += ";";
        variable_handler.add(variable_name,variable_type,I_NULL,SCOPETYPE_MAIN);
    }
    else {//Is Declared
        if(variable_handler.exists(variable_name,S_NULL,I_NULL,SCOPETYPE_MAIN)){//Does the variable exist?
            //The variable exists, so far okay
            string variable_type = variable_handler.variables[variable_handler.find(variable_name,S_NULL,I_NULL,SCOPETYPE_MAIN)].type;
            compile_code = string_kill_whitespace(compile_code);

            ve_main_code += resource(variable_name) + "=";

            //Handle Value
            if(code_parser.harvest_from_variable_value(compile_code,variable_type,true)==EXIT_FAILURE){
               return EXIT_FAILURE;
            }

            code_parser.chop(compile_code);

            ve_main_code += ";";
        }
        else {//The variable does not exist
            error_fatal("Undeclared variable '" + variable_name + "'.");
            pend();
            return EXIT_FAILURE;
        }
    }
}

if(compile_code.substr(0,1)=="+"){
    //Plus
}

if(compile_code.substr(0,1)=="-"){
    //Minus
}

if(compile_code.substr(0,1)=="*"){
    //Times
}

if(compile_code.substr(0,1)=="/"){
    //Divide
}

continue;
