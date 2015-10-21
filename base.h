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
//Parses Function Arguments, example input: ("hello world",10)
//Returns code with semicolon and newline attached.
int CodeParser::parse_args(string& code){

    if(code.substr(0,1)!="("){
        error_fatal("Expected '(' before '" + code.substr(0,1) + "' when parsing method arguments");
        pend();
        return EXIT_FAILURE;
    }

    *write_to += "(";
    code = string_delete_amount(code,1);
    code = string_kill_all_whitespace(code);
    bool first = true;
    string function_code_prev;
    string argument_type;

    while(code.substr(0,1)!=")" and function_code_prev!=code){
        code = string_kill_whitespace(code);
        function_code_prev = code;

        if(code.substr(0,1)=="," and !first){
            *write_to += ",";
            code = string_delete_amount(code,1);
        }

        first = false;

        //Get Value Type
        if(code_parser.harvest_from_variable_value_type(code,argument_type)==EXIT_FAILURE){
            error_fatal("Couldn't Determine Type for Method Argument");
            pend();
            return EXIT_FAILURE;
        }

        //Handle Value
        if(code_parser.harvest_from_variable_value(code,argument_type,",)")==EXIT_FAILURE){
            return EXIT_FAILURE;
        }

        code = string_kill_whitespace(code);
    }

    *write_to += ")";

    if(function_code_prev==code){
        error_fatal("Internal Method Error");
        pend();
        return EXIT_FAILURE;
    }

    code = string_delete_amount(code,1);
    return EXIT_SUCCESS;
}

int CodeParser::parse_declaration_args(string& code, string method_name){

    if(code.substr(0,1)!="("){
        error_fatal("Expected '(' before '" + code.substr(0,1) + "' when parsing method declaration arguments");
        pend();
        return EXIT_FAILURE;
    }

    *write_to += "(";
    code = string_delete_amount(code,1);
    code = string_kill_all_whitespace(code);
    bool first = true;
    string function_code_prev;
    string argument_type;

    while(code.substr(0,1)!=")" and function_code_prev!=code){
        code = string_kill_whitespace(code);
        function_code_prev = code;

        if(code.substr(0,1)=="," and !first){
            *write_to += ",";
            code = string_delete_amount(code,1);
        }

        code = string_kill_whitespace(code);

        first = false;

        //Get Variable
        string parameter_name = string_get_until_or(code," =");
        code = string_delete_until_or(code," =");
        code = string_kill_whitespace(code);

        if(code.substr(0,1)!="="){
            error_fatal("Expected '=' before '" + code.substr(0,1) + "' in Method Argument Declaration");
            pend();
            return EXIT_FAILURE;
        }

        code = string_delete_amount(code,1);

        //Get Value Type
        if(code_parser.harvest_from_variable_value_type(code,argument_type)==EXIT_FAILURE){
            error_fatal("Couldn't Determine Type for Method Argument Declaration");
            pend();
            return EXIT_FAILURE;
        }

        *write_to += resource(argument_type) + " " + resource(parameter_name) + " = ";
        variable_handler.add(parameter_name,argument_type,function_handler.find(method_name,S_NULL,S_NULL,I_NULL,SCOPETYPE_GLOBAL),SCOPETYPE_FUNCTION);

        //Handle Value
        if(code_parser.harvest_from_variable_value(code,argument_type,",)")==EXIT_FAILURE){
            return EXIT_FAILURE;
        }

        code = string_kill_whitespace(code);
    }

    *write_to += ")";

    if(function_code_prev==code){
        error_fatal("Internal Method Declaration Error");
        pend();
        return EXIT_FAILURE;
    }

    code = string_delete_amount(code,1);
    return EXIT_SUCCESS;
}

//Parses Function that begins with a dot ".show()" for example
int CodeParser::parse_function_from(string& code, bool check_semicolon, int class_id){
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

    *write_to += "." + resource(function_name);

    if(code_parser.parse_args(code)==-1){
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

//Takes string and removes semicolon and newline if needed
void CodeParser::chop(string& code){
    string potential_code = string_kill_whitespace(code);

    if(potential_code.substr(0,1)=="\n"){
        potential_code = string_delete_amount(potential_code,1);
    }

    potential_code = string_kill_whitespace(code);

    if(potential_code.substr(0,1)==";"){
        potential_code  = string_kill_whitespace(code);
        potential_code = string_delete_amount(code,1);
        code = potential_code;
    }
}

//Writes and removes the value in variable expressions
int CodeParser::harvest_from_variable_value(string& code, string &type, string additional_characters){
    /*
        code - code
        type - variable type

        Example:
        "Hello " + "World"\n
    */

    string code_prev;
    int balance = 0;
    bool accept_value = true;

    while( ((code.substr(0,1)!="\n" and code.substr(0,1)!=";" and !string_contains(additional_characters,code.substr(0,1))) or balance!=0) and (code_prev!=code) ){
        code_prev = code;
        code = string_kill_whitespace(code);

        if (code.substr(0,1)=="+"){
            if(accept_value==true){
                error_fatal("Expected a value before '+'");
                pend();
                return EXIT_FAILURE;
            }
            accept_value = true;

            code = string_delete_amount(code,1);
            *write_to += "+";
        }
        else if (code.substr(0,1)=="-"){
            if (accept_value==true and (code.substr(1,1)=="0" or code.substr(1,1)=="1" or code.substr(1,1)=="2" or code.substr(1,1)=="3" or code.substr(1,1)=="4"
            or code.substr(1,1)=="5" or code.substr(1,1)=="6" or code.substr(1,1)=="7" or code.substr(1,1)=="8" or code.substr(1,1)=="9")){
                //Negative Number

                accept_value = false;

                if(type!="Number"){
                    error_fatal("Incompatible Templates '" + type + "' and 'Number'");
                    pend();
                    return EXIT_FAILURE;
                }

                *write_to += "BOOMSLANG_Number(" + harvest_decimal(code) + ")";
            }
            else if(accept_value==true){
                error_fatal("Expected a value before '-'");
                pend();
                return EXIT_FAILURE;
            }
            else {
                accept_value = true;

                code = string_delete_amount(code,1);
                *write_to += "-";
            }
        }
        else if (code.substr(0,1)=="*"){
            if(accept_value==true){
                error_fatal("Expected a value before '*'");
                pend();
                return EXIT_FAILURE;
            }
            accept_value = true;

            code = string_delete_amount(code,1);
            *write_to += "*";
        }
        else if (code.substr(0,1)=="/"){
            if(accept_value==true){
                error_fatal("Expected a value before '/'");
                pend();
                return EXIT_FAILURE;
            }
            accept_value = true;

            code = string_delete_amount(code,1);
            *write_to += "/";
        }
        else if (code.substr(0,1)=="("){
            if(accept_value==false){
                error_fatal("Expected an operator before '('");
                pend();
                return EXIT_FAILURE;
            }

            balance += 1;
            code = string_delete_amount(code,1);
            *write_to += "(";
        }
        else if (code.substr(0,1)==")"){
            if(accept_value==true){
                error_fatal("Expected an operator before ')'");
                pend();
                return EXIT_FAILURE;
            }

            balance -= 1;
            code = string_delete_amount(code,1);
            *write_to += ")";
        }
        else if(string_get_until_or(code," ")=="new"){
            if(accept_value==false){
                error_fatal("Expected an operator before 'new'");
                pend();
                return EXIT_FAILURE;
            }
            accept_value = false;
            //Create new object

            code = string_delete_until_or(code," ");

            code = string_kill_whitespace(code);

            string variable_class = string_get_until_or(code," ;\n+-*/),");

            if(!class_handler.exists(variable_class)){
                error_fatal("Undeclared Template '" + variable_class + "'");
                pend();
                return EXIT_FAILURE;
            } else if(type!=variable_class){
                error_fatal("Template '" + variable_class + "' is Incompatible with Template '" + type +"'");
                pend();
                return EXIT_FAILURE;
            }

            code = string_delete_until_or(code," ;\n+-*/),");
            *write_to += " BOOMSLANGCORE_create<" + resource(variable_class) + ">() ";
            code = string_kill_whitespace(code);
        }
        else if(code_parser.arg_type(code)==ARGTYPE_STRING){

            if(accept_value==false){
                error_fatal("Expected an operator before String");
                pend();
                return EXIT_FAILURE;
            }
            accept_value = false;

            if(type==S_NULL){
                type = "String";
            }
            else if(type!="String"){
                error_fatal("Incompatible Templates '" + type + "' and 'String'");
                pend();
                return EXIT_FAILURE;
            }

            *write_to += "BOOMSLANG_String(\"" + harvest_string(code) + "\")";
        }
        else if(code_parser.arg_type(code)==ARGTYPE_NUMBER){

            if(accept_value==false){
                error_fatal("Expected an operator before Number");
                pend();
                return EXIT_FAILURE;
            }
            accept_value = false;

            if(type==S_NULL){
                type = "Number";
            }
            else if(type!="Number"){
                error_fatal("Incompatible Templates '" + type + "' and 'Number'");
                pend();
                return EXIT_FAILURE;
            }

            *write_to += "BOOMSLANG_Number(" + harvest_decimal(code) + ")";
        }
        else if(code_parser.arg_type(code)==ARGTYPE_VARIABLE){
            if(accept_value==false){
                error_fatal("Expected an operator before variable");
                pend();
                return EXIT_FAILURE;
            }
            accept_value = false;

            string variable_name = string_get_until_or(code," =+-/*.)\n");

            code = string_delete_until_or(code," =+-/*.)\n");

            code = string_kill_whitespace(code);

            *write_to += resource(variable_name);

            if(code.substr(0,1)=="."){
                string return_type = variable_handler.variables[variable_handler.find(variable_name,S_NULL,I_NULL,SCOPETYPE_MAIN)].type;
                string prev_return_type = return_type;

                while(code.substr(0,1)=="." or code.substr(0,1)==","){

                    if(code.substr(0,1)==","){
                        if(variable_handler.exists(variable_name,S_NULL,I_NULL,SCOPETYPE_MAIN)){
                            *write_to += ";\n";
                            return_type = function_handler.functions[function_handler.find(string_get_until_or(string_delete_amount(code,1)," ("),S_NULL,S_NULL,class_handler.find( variable_handler.variables[variable_handler.find(variable_name,S_NULL,I_NULL,SCOPETYPE_MAIN)].type ),SCOPETYPE_TEMPLATE)].type;
                            *write_to += resource(variable_name);
                            if(code_parser.parse_function_from(code,true,class_handler.find( variable_handler.variables[variable_handler.find(variable_name,S_NULL,I_NULL,SCOPETYPE_MAIN)].type ))==-1){
                                return EXIT_FAILURE;
                            }
                            prev_return_type = return_type;
                        } else {
                            error_fatal("Undeclared Variable " + variable_name + ".");
                            pend();
                            return EXIT_FAILURE;
                        }
                    }

                    if(code.substr(0,1)=="."){
                        if(function_handler.exists(string_get_until_or(string_delete_amount(code,1)," ("),S_NULL,S_NULL,class_handler.find(prev_return_type),SCOPETYPE_TEMPLATE) and prev_return_type!="none"){
                            return_type = function_handler.functions[function_handler.find(string_get_until_or(string_delete_amount(code,1)," ("),S_NULL,S_NULL,class_handler.find(prev_return_type),SCOPETYPE_TEMPLATE)].type;
                            if(code_parser.parse_function_from(code,true,class_handler.find(prev_return_type))==-1){
                                return EXIT_FAILURE;
                            }
                            prev_return_type = return_type;
                            } else {
                            if(prev_return_type!="none"){
                                error_fatal("Undeclared Function '" + string_get_until_or(string_delete_amount(code,1)," (") + "' of template '" + prev_return_type + "'.");
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

                code = string_kill_whitespace(code);

                code_parser.chop(code);

                *write_to += ";\n";
            }
        }
        else if(code_parser.arg_type(code)==ARGTYPE_FUNCTION){
            if(accept_value==false){
                error_fatal("Expected an operator before Function");
                pend();
                return EXIT_FAILURE;
            }
            accept_value = false;

            string function_name = string_get_until_or(code,"(");

            if(!function_handler.exists(function_name,S_NULL,S_NULL,I_NULL,SCOPETYPE_GLOBAL)){
                error_fatal("Undeclared function '" + function_name + "'");
                pend();
                return EXIT_FAILURE;
            }

            code = string_delete_until_or(code,"(");
            *write_to += resource(function_name) + "(";

            string function_code_prev;
            string argument_type;

            code = string_delete_amount(code,1);
            code = string_kill_all_whitespace(code);
            bool first = true;

            while(code.substr(0,1)!=")" and function_code_prev!=code){
                code = string_kill_whitespace(code);
                function_code_prev = code;

                if(code.substr(0,1)=="," and !first){
                    *write_to += ",";
                    code = string_delete_amount(code,1);
                }

                first = false;

                //Get Value Type
                if(code_parser.harvest_from_variable_value_type(code,argument_type)==EXIT_FAILURE){
                    error_fatal("Couldn't Determine Type for Argument in Function '" + function_name + "'");
                }

                //Handle Value
                if(code_parser.harvest_from_variable_value(code,argument_type,",)")==EXIT_FAILURE){
                    return EXIT_FAILURE;
                }

                code = string_kill_whitespace(code);
            }

            *write_to += ")";

            if(function_code_prev==code){
                error_fatal("Internal Function Error");
                pend();
                return EXIT_FAILURE;
            }

            code = string_delete_amount(code,1);
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

//Gets the value type in variable expressions
int CodeParser::harvest_from_variable_value_type(string code, string &type){
    /*
        code - code
        type - variable type return

        Example:
        "Hello " + "World"\n
    */


    code = string_kill_whitespace(code);

        if (code.substr(0,1)=="+"){
            error_fatal("Expected a value before '+'");
            pend();
            return EXIT_FAILURE;
        }
        else if (code.substr(0,1)=="-"){
            if (code.substr(1,1)!="0" and code.substr(1,1)!="1" and code.substr(1,1)!="2" and code.substr(1,1)!="3" and code.substr(1,1)!="4"
            and code.substr(1,1)!="5" and code.substr(1,1)!="6" and code.substr(1,1)!="7" and code.substr(1,1)!="8" and code.substr(1,1)!="9"){
                error_fatal("Expected a value before '-'");
                pend();
                return EXIT_FAILURE;
            } else {
                type = "Number";
            }
        }
        else if (code.substr(0,1)=="*"){
            error_fatal("Expected a value before '*'");
            pend();
            return EXIT_FAILURE;
        }
        else if (code.substr(0,1)=="/"){
            error_fatal("Expected a value before '/'");
            pend();
            return EXIT_FAILURE;
        }
        else if (code.substr(0,1)=="("){
            code = string_delete_amount(code,1);
        }
        else if (code.substr(0,1)==")"){
            code = string_delete_amount(code,1);
        }
        else if(string_get_until_or(code," ")=="new"){
            //Create new object

            code = string_delete_until_or(code," ");

            code = string_kill_whitespace(code);

            string variable_class = string_get_until_or(code," ;\n+-*/),");

            if(!class_handler.exists(variable_class)){
                error_fatal("Undeclared Template '" + variable_class + "'");
                pend();
                return EXIT_FAILURE;
            }

            code = string_delete_until_or(code," ;\n+-*/),");
            type = variable_class;

            code = string_kill_whitespace(code);
        }
        else if(code_parser.arg_type(code)==ARGTYPE_STRING){
            type = "String";
        }
        else if(code_parser.arg_type(code)==ARGTYPE_NUMBER){
            type = "Number";
        }
        else if(code_parser.arg_type(code)==ARGTYPE_VARIABLE){//Variable
            string variable_name = string_get_until_or(code," =+-/*.)\n");

            if(!variable_handler.exists(variable_name,S_NULL,I_NULL,SCOPETYPE_MAIN)){
                error_fatal("Undeclared variable '" + variable_name + "'");
                pend();
                return EXIT_FAILURE;
            }

            type = variable_handler.variables[variable_handler.find(variable_name,S_NULL,I_NULL,SCOPETYPE_MAIN)].type;

            //Could add more functionality
        }
        else if(code_parser.arg_type(code)==ARGTYPE_FUNCTION){
            string function_name = string_get_until_or(code,"(");

            if(!function_handler.exists(function_name,S_NULL,S_NULL,I_NULL,SCOPETYPE_GLOBAL)){
                error_fatal("Undeclared function '" + function_name + "'");
                pend();
                return EXIT_FAILURE;
            }

            type = function_handler.functions[function_handler.find(function_name,S_NULL,S_NULL,I_NULL,SCOPETYPE_GLOBAL)].type;

            if(type == "none"){
                error_fatal("Couldn't use type none");
                pend();
                return EXIT_FAILURE;
            }
        }
        else {
            return EXIT_FAILURE;
        }

        code = string_kill_whitespace(code);

    return EXIT_SUCCESS;
}

//Function to Determine what data type is at the beginning of the string
int CodeParser::arg_type(string code){
    if(code.substr(0,1)=="\""){//String
        return ARGTYPE_STRING;
    }
    else if(code.substr(0,1)=="-" or code.substr(0,1)=="0" or code.substr(0,1)=="1" or code.substr(0,1)=="2" or code.substr(0,1)=="3" or code.substr(0,1)=="4" or code.substr(0,1)=="5"
    or code.substr(0,1)=="6" or code.substr(0,1)=="7" or code.substr(0,1)=="8" or code.substr(0,1)=="9"
    or (code.substr(1,1)=="-" and (code.substr(1,1)=="0" or code.substr(1,1)=="1" or code.substr(1,1)=="2" or code.substr(1,1)=="3" or code.substr(1,1)=="4"
    or code.substr(1,1)=="5" or code.substr(1,1)=="6" or code.substr(1,1)=="7" or code.substr(1,1)=="8" or code.substr(1,1)=="9"))){
        //Number
        return ARGTYPE_NUMBER;
    }
    else if(is_identifier(string_get_until_or(compile_code,"("))){
        return ARGTYPE_FUNCTION;
    } else {
        return ARGTYPE_VARIABLE;
    }
}
