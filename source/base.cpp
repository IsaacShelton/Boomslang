
#include <string>
#include <algorithm>
#include <iostream>
#include "../include/management.h"
#include "../include/resource.h"
#include "../include/globals.h"

using namespace std;

//Harvests a raw string
string code_harvest_string(string& code){
    string str;

    if(code.substr(0,1)=="\""){
        code = string_delete_amount(code,1);

        while(code.substr(0,1)!="\""){
            str += code.substr(0,1);
            code = string_delete_amount(code,1);
        }

        code = string_delete_amount(code,1);
    }

    return str;
}

//Harvests a raw number
string code_harvest_decimal(string& code){
    string str;

    while(code.substr(0,1)=="0"
          or code.substr(0,1)=="1"
          or code.substr(0,1)=="2"
          or code.substr(0,1)=="3"
          or code.substr(0,1)=="4"
          or code.substr(0,1)=="5"
          or code.substr(0,1)=="6"
          or code.substr(0,1)=="7"
          or code.substr(0,1)=="8"
          or code.substr(0,1)=="9"
          or code.substr(0,2)==".0"
          or code.substr(0,2)==".1"
          or code.substr(0,2)==".2"
          or code.substr(0,2)==".3"
          or code.substr(0,2)==".4"
          or code.substr(0,2)==".5"
          or code.substr(0,2)==".6"
          or code.substr(0,2)==".7"
          or code.substr(0,2)==".8"
          or code.substr(0,2)==".9"
          or (code.substr(0,1)=="-" and str=="" and (code.substr(1,1)=="0" or code.substr(1,1)=="1" or code.substr(1,1)=="2" or code.substr(1,1)=="3" or code.substr(1,1)=="4"
                or code.substr(1,1)=="5" or code.substr(1,1)=="6" or code.substr(1,1)=="7" or code.substr(1,1)=="8" or code.substr(1,1)=="9"))){
        str += code.substr(0,1);
        code = string_delete_amount(code,1);
    }

    return str;
}

//Harvests a raw expression
int code_harvest_raw_expression(string& code, string& exp, string& type, string method_name, string template_name, string& write_to){
    /*
        code - code to harvest raw expression from
            example: ("Hello" + " World")blah blah blah

        exp - returns expression

        type - returns type of the expression
            return example: String1

        returns success or failure
    */

    exp = "(";
    string code_prev;
    int balance = 0;
    bool accept_value = true;

    while(code.substr(0,1)=="("){
        code = string_delete_amount(code,1);
        balance += 1;
    }

    if(code_harvest_value_type(code,type,method_name,template_name,indentation)==EXIT_FAILURE){
        return EXIT_FAILURE;
    }

    code = string_kill_whitespace(code);

    while( (( code.substr(0,1)!=")") or balance!=1 ) and (code.substr(0,1)!="\n" or (accept_value!=false and type!="Boolean")) and (code_prev!=code) ){
        //write_to = &exp;
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
            exp += "+";
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

                exp += "boomslang_Number(" + code_harvest_decimal(code) + ")";
            }
            else if(accept_value==true){
                code = string_delete_amount(code,1);
                exp += "-";
            }
            else {
                accept_value = true;

                code = string_delete_amount(code,1);
                exp += "-";
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
            exp += "*";
        }
        else if (code.substr(0,1)=="/"){
            if(accept_value==true){
                error_fatal("Expected a value before '/'");
                pend();
                return EXIT_FAILURE;
            }
            accept_value = true;

            code = string_delete_amount(code,1);
            exp += "/";
        }
        else if (code.substr(0,1)=="("){
            if(accept_value==false){
                error_fatal("Expected an operator before '('");
                pend();
                return EXIT_FAILURE;
            }

            balance += 1;
            code = string_delete_amount(code,1);
            exp += "(";
        }
        else if (code.substr(0,1)==")"){
            if(accept_value==true){
                error_fatal("Expected an operator before ')'");
                pend();
                return EXIT_FAILURE;
            }

            balance -= 1;
            code = string_delete_amount(code,1);
            exp += ")";
        }
        else if(string_get_until(code," ")=="new"){
            if(accept_value==false){
                error_fatal("Expected an operator before 'new'");
                pend();
                return EXIT_FAILURE;
            }
            accept_value = false;
            //Create new object

            code = string_delete_until(code," ");
            code = string_kill_whitespace(code);

            string variable_class;

            if(code.substr(0,1)=="("){
                string prev;
                variable_class += "(";

                code = string_delete_amount(code,1);
                code = string_kill_whitespace(code);

                while(code.substr(0,1)!=")" and code!=prev){
                    prev = code;

                    if(code.substr(0,1)==","){
                        code = string_delete_amount(code,1);
                    }

                    variable_class += string_get_until_or(code," ,)");

                    code = string_delete_until_or(code," ,)");
                    code = string_kill_whitespace(code);

                    if(code.substr(0,1)!=")") variable_class += ",";
                }

                variable_class += ")";

                if(code==prev){
                    error_fatal("Internal Parse Error");
                    pend();
                    return EXIT_FAILURE;
                }
                code = string_delete_amount(code,1);
                code = string_kill_whitespace(code);
            }

            variable_class = string_get_until_or(code," ;\n+-*/().") + variable_class;

            if(!class_handler.exists( string_get_until(variable_class,"(") )){
                error_fatal("Undeclared Template '" + string_get_until(variable_class,"(") + "'");
                pend();
                return EXIT_FAILURE;
            } else if(type!=variable_class){
                error_fatal("Template '" + variable_class + "' is Incompatible with Template '" + type +"'");
                pend();
                return EXIT_FAILURE;
            }

            code = string_delete_until_or(code," ;\n+-*/().");
            exp += string_template(variable_class);
            code = string_kill_whitespace(code);

            if(code.substr(0,1)!="("){
                exp += "()";
            } else {
                code_parse_args(code,method_name,template_name,exp);
            }

            code = string_kill_whitespace(code);
        }
        else if (code.substr(0,2)=="=="){
            if(accept_value==true){
                error_fatal("Expected a value before '=='");
                pend();
                return EXIT_FAILURE;
            }
            accept_value = true;

            code = string_delete_amount(code,2);
            exp += "==";
        }
        else if (code.substr(0,1)=="="){
            if(accept_value==true){
                error_fatal("Expected a value before '='");
                pend();
                return EXIT_FAILURE;
            }
            accept_value = true;

            code = string_delete_amount(code,1);
            exp += "==";
        }
        else if (code.substr(0,1)=="!"){
            if(accept_value==false){
                error_fatal("Expected a operator before '!'");
                pend();
                return EXIT_FAILURE;
            }

            code = string_delete_amount(code,1);
            exp += "!";
        }
        else if(string_get_until(code," ")=="not"){
            if(accept_value==false){
                error_fatal("Expected a operator before 'not'");
                pend();
                return EXIT_FAILURE;
            }

            code = string_delete_amount(code,3);
            code = string_kill_whitespace(code);
            exp += "!";
        }
        else if (code.substr(0,2)=="or"){
            if(accept_value==true){
                error_fatal("Expected a value before 'or'");
                pend();
                return EXIT_FAILURE;
            }
            accept_value = true;

            code = string_delete_amount(code,2);
            exp += "||";
        }
        else if (code.substr(0,3)=="and"){
            if(accept_value==true){
                error_fatal("Expected a value before 'and'");
                pend();
                return EXIT_FAILURE;
            }
            accept_value = true;

            code = string_delete_amount(code,3);
            exp += "&&";
        }
        else if (code.substr(0,2)=="<="){
            if(accept_value==true){
                error_fatal("Expected a value before '<='");
                pend();
                return EXIT_FAILURE;
            }
            accept_value = true;

            code = string_delete_amount(code,2);
            exp += "<=";
        }
        else if (code.substr(0,2)==">="){
            if(accept_value==true){
                error_fatal("Expected a value before '>='");
                pend();
                return EXIT_FAILURE;
            }
            accept_value = true;

            code = string_delete_amount(code,2);
            exp += ">=";
        }
        else if (code.substr(0,1)=="<"){
            if(accept_value==true){
                error_fatal("Expected a value before '<'");
                pend();
                return EXIT_FAILURE;
            }
            accept_value = true;

            code = string_delete_amount(code,1);
            exp += "<";
        }
        else if (code.substr(0,1)==">"){
            if(accept_value==true){
                error_fatal("Expected a value before '<'");
                pend();
                return EXIT_FAILURE;
            }
            accept_value = true;

            code = string_delete_amount(code,1);
            exp += ">";
        }
        else if(code_arg_type(code)==ARGTYPE_STRING){
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

            exp += "boomslang_String(\"" + code_harvest_string(code) + "\")";
        }
        else if(code_arg_type(code)==ARGTYPE_NUMBER){

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

            exp += "boomslang_Number(" + code_harvest_decimal(code) + ")";
        }
        else if(string_get_until_or(code,".=\n ")=="true"){
            if(accept_value==false){
                error_fatal("Expected an operator before Boolean");
                pend();
                return EXIT_FAILURE;
            }
            accept_value = false;

            code = string_delete_amount(code,4);

            if(type==S_NULL){
                type = "Boolean";
            }
            else if(type!="Boolean"){
                error_fatal("Incompatible Templates '" + type + "' and 'Boolean'");
                pend();
                return EXIT_FAILURE;
            }

            exp += "boomslang_Boolean(true)";
        }
        else if(string_get_until_or(code,".=\n ")=="false"){
            if(accept_value==false){
                error_fatal("Expected an operator before Boolean");
                pend();
                return EXIT_FAILURE;
            }
            accept_value = false;

            code = string_delete_amount(code,5);

            if(type==S_NULL){
                type = "Boolean";
            }
            else if(type!="Boolean"){
                error_fatal("Incompatible Templates '" + type + "' and 'Boolean'");
                pend();
                return EXIT_FAILURE;
            }

            exp += "boomslang_Boolean(false)";
        }
        else if(code_arg_type(code)==ARGTYPE_VARIABLE){
            if(accept_value==false){
                error_fatal("Expected an operator before variable");
                pend();
                return EXIT_FAILURE;
            }
            accept_value = false;

            string variable_name = string_get_until_or(code," =+-/*.)[\n");

            if(!variable_handler.available(variable_name,S_NULL,method_name,template_name)){
                error_fatal("Undeclared Variable '" + variable_name + "'");
                pend();
                return EXIT_FAILURE;
            }

            if(variable_handler.available_get(variable_name,S_NULL,method_name,template_name).is_unique){
                error_fatal("Couldn't pass unique object '" + variable_name + "'");
                pend();
                return EXIT_FAILURE;
            }

            if(type==S_NULL){
                type = variable_handler.available_get(variable_name,S_NULL,method_name,template_name).type;
            }
            else if(variable_handler.available_get(variable_name,S_NULL,method_name,template_name).type!=type){
                error_fatal("Incompatible Templates '" + variable_handler.available_get(variable_name,S_NULL,method_name,template_name).type + "' and '" + type + "'");
                pend();
                return EXIT_FAILURE;
            }

            code = string_delete_until_or(code," =+-/*.)[\n");
            code = string_kill_whitespace(code);

            exp += resource(variable_name);

            if(code.substr(0,1)=="["){
                exp += "[";
                code = string_delete_amount(code,1);

                //Get Value Type
                string argument_type;

                if(code_harvest_value_type(code,argument_type,method_name,template_name,indentation)==EXIT_FAILURE){
                    error_fatal("Couldn't Determine Type for Method Argument");
                    pend();
                    return EXIT_FAILURE;
                }

                //Handle Value
                if(code_harvest_value(code,argument_type,"]",method_name,template_name,indentation,exp)==EXIT_FAILURE){
                    return EXIT_FAILURE;
                }

                code = string_kill_whitespace(code);

                if(code.substr(0,1)!="]"){
                    error_fatal("Expected ']' before '" + code.substr(0,1) + "'");
                    pend();
                    return EXIT_FAILURE;
                }

                exp += "]";
                code = string_delete_amount(code,1);

            }

            if(code.substr(0,1)=="."){

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
                //write_to = &exp;

                while(code.substr(0,1)=="."){
                    if(function_handler.exists(string_get_until_or(string_delete_amount(code,1)," ("),S_NULL,S_NULL,class_handler.find(prev_return_type),SCOPETYPE_TEMPLATE) and prev_return_type!="none"){
                        return_type = function_handler.functions[function_handler.find(string_get_until_or(string_delete_amount(code,1)," ("),S_NULL,S_NULL,class_handler.find(prev_return_type),SCOPETYPE_TEMPLATE)].type;
                        if(code_parse_function_from(code,false,class_handler.find(prev_return_type),method_name,template_name,exp)==EXIT_FAILURE){
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

                code = string_kill_whitespace(code);
            }
        }
        else if(code_arg_type(code)==ARGTYPE_FUNCTION){
            if(accept_value==false){
                error_fatal("Expected an operator before Function");
                pend();
                return EXIT_FAILURE;
            }
            accept_value = false;

            //write_to = &exp;

            string function_name = string_get_until_or(code,"(");

            if(!function_handler.exists(function_name,S_NULL,S_NULL,I_NULL,SCOPETYPE_GLOBAL)){
                error_fatal("Undeclared function '" + function_name + "'");
                pend();
                return EXIT_FAILURE;
            }

            code = string_delete_until_or(code,"(");
            exp += resource(function_name) + "(";

            string function_code_prev;
            string argument_type;

            code = string_delete_amount(code,1);
            code = string_kill_all_whitespace(code);
            bool first = true;

            while(code.substr(0,1)!=")" and function_code_prev!=code){
                code = string_kill_whitespace(code);
                function_code_prev = code;

                if(code.substr(0,1)=="," and !first){
                    write_to += ",";
                    code = string_delete_amount(code,1);
                }

                first = false;

                //Get Value Type
                if(code_harvest_value_type(code,argument_type,method_name,template_name,indentation)==EXIT_FAILURE){
                    error_fatal("Couldn't Determine Type for Argument in Function '" + function_name + "'");
                }

                //Handle Value
                if(code_harvest_value(code,argument_type,",)",method_name,template_name,indentation,exp)==EXIT_FAILURE){
                    return EXIT_FAILURE;
                }

                code = string_kill_whitespace(code);
            }

            exp += ")";

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
        cout << code << endl;
        pend();
        return EXIT_FAILURE;
    }

    if(code.substr(0,1)==")"){
        exp += ")";
    }

    code = string_delete_amount(code,1);

    return EXIT_SUCCESS;
}

//Parses Function Arguments, example input: ("hello world",10)
//Returns code with semicolon and newline attached.
int code_parse_args(string& code, string method_name, string template_name, string& write_to){

    if(code.substr(0,1)!="("){
        error_fatal("Expected '(' before '" + code.substr(0,1) + "' when parsing method arguments");
        pend();
        return EXIT_FAILURE;
    }

    write_to += "(";
    code = string_delete_amount(code,1);
    code = string_kill_all_whitespace(code);
    bool first = true;
    string function_code_prev;
    string argument_type;

    while(code.substr(0,1)!=")" and function_code_prev!=code){
        code = string_kill_all_whitespace(code);
        function_code_prev = code;

        if(code.substr(0,1)=="," and !first){
            write_to += ",";
            code = string_delete_amount(code,1);
        }

        first = false;

        //Get Value Type
        if(code_harvest_value_type(code,argument_type,method_name,template_name,indentation)==EXIT_FAILURE){
            error_fatal("Couldn't Determine Type for Method Argument");
            pend();
            return EXIT_FAILURE;
        }

        //Handle Value
        if(code_harvest_value(code,argument_type,",)",method_name,template_name,indentation,write_to)==EXIT_FAILURE){
            return EXIT_FAILURE;
        }

        code = string_kill_all_whitespace(code);
    }

    write_to += ")";

    if(function_code_prev==code){
        error_fatal("Internal Method Error");
        pend();
        return EXIT_FAILURE;
    }

    code = string_delete_amount(code,1);
    return EXIT_SUCCESS;
}

//Parses Function Declaration Arguments
int code_parse_declaration_args(string& code, string method_name, string template_name, string& write_to){

    if(code.substr(0,1)!="("){
        error_fatal("Expected '(' before '" + code.substr(0,1) + "' when parsing method declaration arguments");
        pend();
        return EXIT_FAILURE;
    }

    write_to += "(";
    code = string_delete_amount(code,1);
    code = string_kill_all_whitespace(code);
    bool first = true;
    string function_code_prev;
    string argument_type;

    while(code.substr(0,1)!=")" and function_code_prev!=code){
        code = string_kill_whitespace(code);
        function_code_prev = code;

        if(code.substr(0,1)=="," and !first){
            write_to += ",";
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

        string character = code.substr(0,1);
        code = string_delete_amount(code,1);

        if(character=="="){
            //Get Value Type
            if(code_harvest_value_type(code,argument_type,method_name,template_name,indentation)==EXIT_FAILURE){
                error_fatal("Couldn't Determine Type for Method Argument Declaration");
                pend();
                return EXIT_FAILURE;
            }

            write_to += string_template(argument_type) + " " + resource(parameter_name) + "=";

            //Add the variable
            if (template_name!="" and method_name==""){//Template non-methods
                variable_handler.add(parameter_name,argument_type,class_handler.find(template_name),SCOPETYPE_TEMPLATE);
            } else if (template_name!="" and method_name!=""){//Template method
                variable_handler.add(parameter_name,argument_type,function_handler.find(method_name,S_NULL,S_NULL,class_handler.find(template_name),SCOPETYPE_TEMPLATE),SCOPETYPE_FUNCTION);
            } else if (template_name=="" and method_name!=""){//Method
                variable_handler.add(parameter_name,argument_type,function_handler.find(method_name,S_NULL,S_NULL,I_NULL,SCOPETYPE_GLOBAL),SCOPETYPE_FUNCTION);
            }

            //Handle Value
            if(code_harvest_value(code,argument_type,",)",method_name,template_name,indentation,write_to)==EXIT_FAILURE){
                return EXIT_FAILURE;
            }
            code = string_kill_whitespace(code);
        }
    }

    write_to += ")";

    if(function_code_prev==code){
        error_fatal("Internal Method Declaration Error");
        pend();
        return EXIT_FAILURE;
    }

    code = string_delete_amount(code,1);
    return EXIT_SUCCESS;
}

//Parses Function that begins with a dot ".show()" for example
int code_parse_function_from(string& code, bool check_semicolon, int class_id, string method_name, string template_name, string& write_to){
    code = string_kill_whitespace(code);

    if(code.substr(0,1)!="."){
        error_fatal("Expected '.' before '" + code.substr(0,1) + "'.");
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
    write_to += "." + resource(function_name);

    if(code_parse_args(code,method_name,template_name,write_to)==EXIT_FAILURE){
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}

//Takes string and removes semicolon and newline if needed
void code_chop(string& code){
    string potential_code = string_kill_whitespace(code);

    if(potential_code.substr(0,1)=="\n"){
        potential_code = string_delete_amount(potential_code,1);
    }

    potential_code = string_kill_whitespace(code);

    if(potential_code.substr(0,1)==";"){
        potential_code = string_delete_amount(code,1);
        code = potential_code;
    }
}

//Writes and removes the value in expression
int code_harvest_value(string& code, string &type, string additional_characters, string method_name, string template_name, unsigned int indentation, string& write_to){
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
        code = string_kill_all_whitespace(code);

        if (code.substr(0,1)=="+"){
            if(accept_value==true){
                error_fatal("Expected a value before '+'");
                pend();
                return EXIT_FAILURE;
            }
            accept_value = true;

            code = string_delete_amount(code,1);
            write_to += "+";
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

                write_to += "boomslang_Number(" + code_harvest_decimal(code) + ")";
            }
            else if(accept_value==true){
                code = string_delete_amount(code,1);
                write_to += "-";
            }
            else {
                accept_value = true;

                code = string_delete_amount(code,1);
                write_to += "-";
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
            write_to += "*";
        }
        else if (code.substr(0,1)=="/"){
            if(accept_value==true){
                error_fatal("Expected a value before '/'");
                pend();
                return EXIT_FAILURE;
            }
            accept_value = true;

            code = string_delete_amount(code,1);
            write_to += "/";
        }
        else if (code.substr(0,1)=="("){
            if(accept_value==false){
                error_fatal("Expected an operator before '('");
                pend();
                return EXIT_FAILURE;
            }

            balance += 1;
            code = string_delete_amount(code,1);
            write_to += "(";
        }
        else if (code.substr(0,1)==")"){
            if(accept_value==true){
                error_fatal("Expected an operator before ')'");
                pend();
                return EXIT_FAILURE;
            }

            balance -= 1;
            code = string_delete_amount(code,1);
            write_to += ")";
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

            string variable_class;

            if(code.substr(0,1)=="("){
                string prev;
                variable_class += "(";

                code = string_delete_amount(code,1);
                code = string_kill_whitespace(code);

                while(code.substr(0,1)!=")" and code!=prev){
                    prev = code;

                    if(code.substr(0,1)==","){
                        code = string_delete_amount(code,1);
                    }

                    variable_class += string_get_until_or(code," ,)");

                    code = string_delete_until_or(code," ,)");
                    code = string_kill_whitespace(code);

                    if(code.substr(0,1)!=")") variable_class += ",";
                }

                variable_class += ")";

                if(code==prev){
                    error_fatal("Internal Parse Error");
                    pend();
                    return EXIT_FAILURE;
                }

                code = string_delete_amount(code,1);
                code = string_kill_whitespace(code);
            }

            variable_class = string_get_until_or(code," ;\n+-*/(),.") + variable_class;

            if(!class_handler.exists( string_get_until(variable_class,"(") )){
                error_fatal("Undeclared Template '" + variable_class + "'");
                pend();
                return EXIT_FAILURE;
            } else if(type!=variable_class){
                error_fatal("Template '" + variable_class + "' is Incompatible with Template '" + type +"'");
                pend();
                return EXIT_FAILURE;
            }

            code = string_delete_until_or(code," ;\n+-*/(),.");
            write_to += string_template(variable_class);

            code = string_kill_whitespace(code);

            if(code.substr(0,1)!="("){
                write_to += "()";
            } else {
                code_parse_args(code,method_name,template_name,write_to);
            }

            code = string_kill_whitespace(code);

            if(code.substr(0,1)=="."){
                string return_type = variable_class;
                string prev_return_type = variable_class;

                while(code.substr(0,1)=="."){
                    if(function_handler.exists(string_get_until_or(string_delete_amount(code,1)," ("),S_NULL,S_NULL,class_handler.find(prev_return_type),SCOPETYPE_TEMPLATE) and prev_return_type!="none"){
                        return_type = function_handler.functions[function_handler.find(string_get_until_or(string_delete_amount(code,1)," ("),S_NULL,S_NULL,class_handler.find(prev_return_type),SCOPETYPE_TEMPLATE)].type;

                        if(code_parse_function_from(code,false,class_handler.find(prev_return_type),"","",write_to)==EXIT_FAILURE){
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

                code = string_kill_whitespace(code);
            }
        }
        else if (code.substr(0,1)=="!"){
            if(accept_value==false){
                error_fatal("Expected a operator before '!'");
                pend();
                return EXIT_FAILURE;
            }

            code = string_delete_amount(code,1);
            write_to += "!";
        }
        else if(string_get_until(code," ")=="not"){
            if(accept_value==false){
                error_fatal("Expected a operator before 'not'");
                pend();
                return EXIT_FAILURE;
            }

            code = string_delete_amount(code,3);
            code = string_kill_whitespace(code);
            write_to += "!";
        }
        else if(string_get_until_or(code," ")=="embedded"){
            code = string_delete_until_or(code," ");
            code = string_kill_whitespace(code);
            string embedded_filename = code_harvest_string(code);
            file_write << "boomslang_List<boomslang_Byte> boomslangEmbedded" + to_string(next_embedded_id) + " = boomslang_List<boomslang_Byte>({";

            streampos file_size;
            char* data;

            ifstream file (embedded_filename.c_str(), ios::in|ios::binary|ios::ate);
            if (file.is_open()){
                file_size = file.tellg();
                data = new char[file_size];
                file.seekg(0, ios::beg);
                file.read(data, file_size);
                file.close();

                for(int i = 0; i < file_size; i++){
                    file_write << (unsigned int)(unsigned char)data[i];
                    if(i!=file_size-1){
                        file_write << ",";
                    }
                }

                delete[] data;
            } else {
                error_fatal("Failed to open embedded file " + embedded_filename);
                pend();
                return EXIT_FAILURE;
            }

            file_write << "});\n";
            write_to += "boomslangEmbedded" + to_string(next_embedded_id);
            next_embedded_id++;
        }
        else if(code_arg_type(code)==ARGTYPE_STRING){

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

            write_to += "boomslang_String(\"" + code_harvest_string(code) + "\")";
        }
        else if(code_arg_type(code)==ARGTYPE_NUMBER){

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

            write_to += "boomslang_Number(" + code_harvest_decimal(code) + ")";
        }
        else if(string_get_until_or(code,".=\n ")=="true"){
            if(accept_value==false){
                error_fatal("Expected an operator before Boolean");
                pend();
                return EXIT_FAILURE;
            }
            accept_value = false;

            code = string_delete_amount(code,4);

            if(type==S_NULL){
                type = "Boolean";
            }
            else if(type!="Boolean"){
                error_fatal("Incompatible Templates '" + type + "' and 'Boolean'");
                pend();
                return EXIT_FAILURE;
            }

            write_to += "boomslang_Boolean(true)";
        }
        else if(string_get_until_or(code,".=\n ")=="false"){
            if(accept_value==false){
                error_fatal("Expected an operator before Boolean");
                pend();
                return EXIT_FAILURE;
            }
            accept_value = false;

            code = string_delete_amount(code,5);

            if(type==S_NULL){
                type = "Boolean";
            }
            else if(type!="Boolean"){
                error_fatal("Incompatible Templates '" + type + "' and 'Boolean'");
                pend();
                return EXIT_FAILURE;
            }

            write_to += "boomslang_Boolean(false)";
        }
        else if(code_arg_type(code)==ARGTYPE_VARIABLE){
            if(accept_value==false){
                error_fatal("Expected an operator before variable");
                pend();
                return EXIT_FAILURE;
            }
            accept_value = false;

            string variable_name = string_get_until_or(code," =+-/*.)[,\n");

            if(!variable_handler.available(variable_name,S_NULL,method_name,template_name)){
                error_fatal("Undeclared Variable '" + variable_name + "'");
                pend();
                return EXIT_FAILURE;
            }

            if(variable_handler.available_get(variable_name,S_NULL,method_name,template_name).is_unique){
                error_fatal("Couldn't pass unique object '" + variable_name + "'");
                pend();
                return EXIT_FAILURE;
            }

            code = string_delete_until_or(code," =+-/*.)[,\n");
            code = string_kill_whitespace(code);

            string variable_type;
            variable_type = variable_handler.available_get(variable_name,S_NULL,method_name,template_name).type;

            write_to += resource(variable_name);

            if(code.substr(0,1)=="["){
                write_to += "[";
                code = string_delete_amount(code,1);

                //Get Value Type
                string argument_type;

                if(code_harvest_value_type(code,argument_type,method_name,template_name,indentation)==EXIT_FAILURE){
                    error_fatal("Couldn't Determine Type for Method Argument");
                    pend();
                    return EXIT_FAILURE;
                }

                //Handle Value
                if(code_harvest_value(code,argument_type,"]",method_name,template_name,indentation,write_to)==EXIT_FAILURE){
                    return EXIT_FAILURE;
                }

                code = string_kill_whitespace(code);

                if(code.substr(0,1)!="]"){
                    error_fatal("Expected ']' before '" + code.substr(0,1) + "'");
                    pend();
                    return EXIT_FAILURE;
                }

                write_to += "]";
                code = string_delete_amount(code,1);
                variable_type = string_sub_template(variable_type);
            }

            if(type==S_NULL){
                type = variable_type;
            }
            else if(type!=variable_type){
                error_fatal("Incompatible Templates '" + type + "' and '" + variable_type + "'");
                pend();
                return EXIT_FAILURE;
            }

            if(code.substr(0,1)=="."){
                string return_type = variable_type;

                string prev_return_type = return_type;

                while(code.substr(0,1)=="."){
                    if(function_handler.exists(string_get_until_or(string_delete_amount(code,1)," ("),S_NULL,S_NULL,class_handler.find(prev_return_type),SCOPETYPE_TEMPLATE) and prev_return_type!="none"){
                        return_type = function_handler.functions[function_handler.find(string_get_until_or(string_delete_amount(code,1)," ("),S_NULL,S_NULL,class_handler.find(prev_return_type),SCOPETYPE_TEMPLATE)].type;
                        if(code_parse_function_from(code,true,class_handler.find(prev_return_type),method_name,template_name,write_to)==EXIT_FAILURE){
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

                code = string_kill_whitespace(code);
            }
        }
        else if(code_arg_type(code)==ARGTYPE_LIST){
            if(accept_value==false){
                error_fatal("Expected an operator before list");
                pend();
                return EXIT_FAILURE;
            }
            accept_value = false;

            if(code.substr(0,1)!="["){
                error_fatal("Expected '[' before '" + code.substr(0,1) + "'");
                pend();
                return EXIT_FAILURE;
            }

            code = string_delete_amount(code,1);

            code = string_kill_all_whitespace(code);

            string list_data;
            string list_code_prev;
            string list_type = "none";
            string argument_type;
            bool first = true;

            while(code.substr(0,1)!="]" and list_code_prev!=code){
                code = string_kill_all_whitespace(code);
                list_code_prev = code;

                if(code.substr(0,1)=="," and !first){
                    list_data += ",";
                    code = string_delete_amount(code,1);
                }

                code = string_kill_all_whitespace(code);

                first = false;

                //Get Value Type
                if(code_harvest_value_type(code,argument_type,method_name,template_name,indentation)==EXIT_FAILURE){
                    error_fatal("Couldn't Determine Type for Argument in List");
                }

                if(argument_type == "none"){
                    error_fatal("Can't use template none in a list");
                    pend();
                    return EXIT_FAILURE;
                }

                if(list_type == "none"){
                    list_type = argument_type;
                } else if (list_type!=argument_type){
                    error_fatal("Incompatible templates '" + list_type + "' and '" + argument_type + "'");
                    pend();
                    return EXIT_FAILURE;
                }

                //Handle Value
                if(code_harvest_value(code,argument_type,",]",method_name,template_name,indentation,list_data)==EXIT_FAILURE){
                    return EXIT_FAILURE;
                }

                code = string_kill_all_whitespace(code);
            }

            code = string_delete_amount(code,1);

            if(type==S_NULL){
                type = "List(" + argument_type + ")";
            }
            else if(type!="List(" + argument_type + ")"){
                error_fatal("Incompatible Templates '" + type + "' and 'List(" + argument_type + ")'");
                pend();
                return EXIT_FAILURE;
            }

            write_to += "boomslang_List<" + resource(argument_type) + ">({" + list_data + "})";
        }
        else if(code_arg_type(code)==ARGTYPE_FUNCTION){
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
            write_to += resource(function_name) + "(";

            string function_code_prev;
            string argument_type;

            code = string_delete_amount(code,1);
            code = string_kill_all_whitespace(code);
            bool first = true;

            while(code.substr(0,1)!=")" and function_code_prev!=code){
                code = string_kill_whitespace(code);
                function_code_prev = code;

                if(code.substr(0,1)=="," and !first){
                    write_to += ",";
                    code = string_delete_amount(code,1);
                }

                first = false;

                //Get Value Type
                if(code_harvest_value_type(code,argument_type,method_name,template_name,indentation)==EXIT_FAILURE){
                    error_fatal("Couldn't Determine Type for Argument in Function '" + function_name + "'");
                }

                //Handle Value
                if(code_harvest_value(code,argument_type,",)",method_name,template_name,indentation,write_to)==EXIT_FAILURE){
                    return EXIT_FAILURE;
                }

                code = string_kill_whitespace(code);
            }

            write_to += ")";

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

//Gets the first type in a expression
int code_harvest_value_type(string code, string &type, string method_name, string template_name,unsigned int indentation){
    /*
        code - code
        type - variable type return

        Example:
        "Hello " + "World"\n
    */

    code = string_kill_all_whitespace(code);

    if (code.substr(0,1)=="+"){
            error_fatal("Expected a value before '+'");
            pend();
            return EXIT_FAILURE;
        }
    else if (code.substr(0,1)=="-"){
        if (code.substr(1,1)!="0" and code.substr(1,1)!="1" and code.substr(1,1)!="2" and code.substr(1,1)!="3" and code.substr(1,1)!="4"
        and code.substr(1,1)!="5" and code.substr(1,1)!="6" and code.substr(1,1)!="7" and code.substr(1,1)!="8" and code.substr(1,1)!="9"){
            if(code_harvest_value_type(string_delete_amount(code,1), type, method_name, template_name,indentation)==EXIT_FAILURE) return EXIT_FAILURE;
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

        code = string_delete_until(code," ");
        code = string_kill_whitespace(code);

        string variable_class;

        if(code.substr(0,1)=="("){
            string prev;
            variable_class += "(";

            code = string_delete_amount(code,1);
            code = string_kill_whitespace(code);

            while(code.substr(0,1)!=")" and code!=prev){
                prev = code;

                if(code.substr(0,1)==","){
                    code = string_delete_amount(code,1);
                }

                variable_class += string_get_until_or(code," ,)");

                code = string_delete_until_or(code," ,)");
                code = string_kill_whitespace(code);

                if(code.substr(0,1)!=")") variable_class += ",";
            }

            variable_class += ")";

            if(code==prev){
                error_fatal("Internal Parse Error");
                pend();
                return EXIT_FAILURE;
            }

            code = string_delete_amount(code,1);
            code = string_kill_whitespace(code);
        }

        variable_class = string_get_until_or(code," ;\n+-*/(),.") + variable_class;

        if(!class_handler.exists(string_get_until(variable_class,"("))){
            error_fatal("Undeclared Template '" + string_get_until(variable_class,"(") + "'");
            pend();
            return EXIT_FAILURE;
        }

        code = string_delete_until_or(code," ;\n+-*/(),.");
        type = variable_class;

        code = string_kill_whitespace(code);
    }
    else if(string_get_until_or(code," ")=="embedded"){
        code = string_delete_until_or(code," ");
        code = string_kill_whitespace(code);
        string embedded_filename = code_harvest_string(code);

        if(file_exists(embedded_filename)){
            type = "List<boomslang_Byte>";
        } else {
            error_fatal("The embedded file '" + embedded_filename + "' does not exist");
            pend();
            return EXIT_FAILURE;
        }
    }
    else if (code.substr(0,1)=="!"){
        if(code_harvest_value_type(string_delete_amount(code,1),type,method_name,template_name,indentation)==EXIT_FAILURE) return EXIT_FAILURE;
    }
    else if (string_get_until(code," ")=="not"){
        if(code_harvest_value_type(string_delete_amount(code,3),type,method_name,template_name,indentation)==EXIT_FAILURE) return EXIT_FAILURE;
    }
    else if(code_arg_type(code)==ARGTYPE_STRING){
        type = "String";
    }
    else if(code_arg_type(code)==ARGTYPE_NUMBER){
        type = "Number";
    }
    else if(string_get_until_or(code,".+-*/=()[];\n ")=="true"){
        type = "Boolean";
    }
    else if(string_get_until_or(code,".+-*/=()[];\n ")=="false"){
        type = "Boolean";
    }
    else if(code_arg_type(code)==ARGTYPE_VARIABLE){//Variable
        string variable_name = string_get_until_or(code," =+-/*.)[,\n");

        //Exists
        if(!variable_handler.available(variable_name,S_NULL,method_name,template_name)){
            error_fatal("Undeclared Variable '" + variable_name + "'");
            pend();
            return EXIT_FAILURE;
        }

        //Unique
        if(variable_handler.available_get(variable_name,S_NULL,method_name,template_name).is_unique){
            error_fatal("Couldn't pass unique object '" + variable_name + "'");
            pend();
            return EXIT_FAILURE;
        }

        //Get type
        type = variable_handler.available_get(variable_name,S_NULL,method_name,template_name).type;

        code = string_delete_until_or(code," =+-/*.)[,\n");
        code = string_kill_whitespace(code);

        if(code.substr(0,1)=="["){
            type = string_sub_template(type);
        }

        //Could add more functionality
    }
    else if(code_arg_type(code)==ARGTYPE_FUNCTION){
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
    else if(code_arg_type(code)==ARGTYPE_LIST){
        if(code.substr(0,1)!="["){
            error_fatal("Expected '[' before '" + code.substr(0,1) + "'");
            pend();
            return EXIT_FAILURE;
        }

        code = string_delete_amount(code,1);

        code = string_kill_all_whitespace(code);

        string argument_type;

        if(code_harvest_value_type(code,argument_type,method_name,template_name,indentation)==EXIT_FAILURE){
            error_fatal("Couldn't Determine Type for Argument in List");
        }

        type = "List(" + argument_type + ")";
    }
    else {
        return EXIT_FAILURE;
    }

    code = string_kill_whitespace(code);

    return EXIT_SUCCESS;
}

//Function to Determine what data type is at the beginning of the string
int code_arg_type(string code){
    if(code.substr(0,1)=="\""){//String
        return ARGTYPE_STRING;
    }
    //Number
    else if(code.substr(0,1)=="-" or code.substr(0,1)=="0" or code.substr(0,1)=="1" or code.substr(0,1)=="2" or code.substr(0,1)=="3" or code.substr(0,1)=="4" or code.substr(0,1)=="5"
    or code.substr(0,1)=="6" or code.substr(0,1)=="7" or code.substr(0,1)=="8" or code.substr(0,1)=="9"
    or (code.substr(1,1)=="-" and (code.substr(1,1)=="0" or code.substr(1,1)=="1" or code.substr(1,1)=="2" or code.substr(1,1)=="3" or code.substr(1,1)=="4"
    or code.substr(1,1)=="5" or code.substr(1,1)=="6" or code.substr(1,1)=="7" or code.substr(1,1)=="8" or code.substr(1,1)=="9"))){
        //Number
        return ARGTYPE_NUMBER;
    }
    else if(code.substr(0,1)=="["){//List
        return ARGTYPE_LIST;
    }
    else if(is_identifier(string_get_until_or(code,"("))){//Function or Variable
        return ARGTYPE_FUNCTION;
    } else {
        return ARGTYPE_VARIABLE;
    }
}
