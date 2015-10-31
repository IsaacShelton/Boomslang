/**
    ============ string_handling.h ============
    Code for String Handling Functions

    This file should NOT be included
    anywhere besides from main.cpp.

    This Header does NOT contain a
    header guard so include with
    cation.
*/

using namespace std;

///Declarations
void error_show(string a);
void error_fatal(string a);
void pend();

bool string_contains(string parent_string, string sub_string){
    if (parent_string.find(sub_string)==string::npos)
        return false;
    else
        return true;
}

string string_get_until(string parent_string, string character){
    unsigned int index = 0;

    while(!( parent_string.substr(index,1)==character or index >= parent_string.length() )){
        index++;
    }

    if (index >= parent_string.length()){
        return parent_string;
    }
    else{
        return parent_string.substr(0,index);
    }
}

string string_delete_until(string parent_string, string character){
    unsigned int index = 0;

    while(!( parent_string.substr(index,1)==character or index >= parent_string.length() )){
        index++;
    }

    if (index >= parent_string.length()){
        return parent_string;
    }
    else{
        return parent_string.substr(index,parent_string.length()-index);
    }
}

string string_get_until_or(string parent_string, string characters){
    unsigned int index = 0;

    while(!( string_contains(characters,parent_string.substr(index,1)) or index >= parent_string.length() )){
        index++;
    }

    if (index >= parent_string.length()){
        return parent_string;
    }
    else{
        return parent_string.substr(0,index);
    }
}

string string_delete_until_or(string parent_string, string characters){
    unsigned int index = 0;

    while(!( string_contains(characters,parent_string.substr(index,1)) or index >= parent_string.length() )){
        index++;
    }

    if (index >= parent_string.length()){
        return parent_string;
    }
    else{
        return parent_string.substr(index,parent_string.length()-index);
    }
}

string string_delete_amount(string str, int num){
    return str.substr(num,str.length()-num);
}

string string_replace(string str, const std::string from, const string to) {
    size_t start_pos = str.find(from);
    if(start_pos == std::string::npos)
        return "";
    else
        return str.replace(start_pos, from.length(), to);
}

string string_replace_all(string str, const std::string from, const string to) {
    if(from.empty())
        return "";
    size_t start_pos = 0;
    while((start_pos = str.find(from, start_pos)) != std::string::npos) {
        str.replace(start_pos, from.length(), to);
        start_pos += to.length(); // In case 'to' contains 'from', like replacing 'x' with 'yx'
    }

    return str;
}

string string_kill_whitespace(string str){
    int n = 0;

    while(!(str.substr(n,1)!=" " and str.substr(n,1)!="\t")){
        n++;
    }

    return string_delete_amount(str,n);
}

string string_kill_all_whitespace(string str){
    int n = 0;

    while(!(str.substr(n,1)!=" " and str.substr(n,1)!="\t" and str.substr(n,1)!="\n")){
        n++;
    }

    return string_delete_amount(str,n);
}

string string_kill_newline(string str){
    int n = 0;

    while(!(str.substr(n,1)!="\n")){
        n++;
    }

    return string_delete_amount(str,n);
}

string resource(string a){
    return "boomslang_" + string_replace_all(string_replace_all(a,":","::boomslang_"),".",".boomslang_");
}

string delete_backslash(string a){
    if (a.substr(0,1)=="\\")
        return string_delete_amount(a,1);
    else
        return a;
}

string string_upper(string a){
    string aa = a;
    ::transform(aa.begin(), aa.end(), aa.begin(), ::toupper);
    return aa;
}

bool is_identifier(string what){
    //Is it blank?
    if (what=="") return false;

    //Does it start with a number?
    if (what.substr(0,1)=="0"
        or what.substr(0,1)=="1"
        or what.substr(0,1)=="2"
        or what.substr(0,1)=="3"
        or what.substr(0,1)=="4"
        or what.substr(0,1)=="5"
        or what.substr(0,1)=="6"
        or what.substr(0,1)=="7"
        or what.substr(0,1)=="8"
        or what.substr(0,1)=="9")
        return false;

    //Underscore are ok
    string a = string_replace_all(what,"_","");

    //Numbers are ok
    a = string_replace_all(a,"1","");
    a = string_replace_all(a,"2","");
    a = string_replace_all(a,"3","");
    a = string_replace_all(a,"4","");
    a = string_replace_all(a,"5","");
    a = string_replace_all(a,"6","");
    a = string_replace_all(a,"7","");
    a = string_replace_all(a,"8","");
    a = string_replace_all(a,"9","");
    a = string_replace_all(a,"0","");

    //Lowercase Letters are ok
    a = string_replace_all(a,"a","");
    a = string_replace_all(a,"b","");
    a = string_replace_all(a,"c","");
    a = string_replace_all(a,"d","");
    a = string_replace_all(a,"e","");
    a = string_replace_all(a,"f","");
    a = string_replace_all(a,"g","");
    a = string_replace_all(a,"h","");
    a = string_replace_all(a,"i","");
    a = string_replace_all(a,"j","");
    a = string_replace_all(a,"k","");
    a = string_replace_all(a,"l","");
    a = string_replace_all(a,"m","");
    a = string_replace_all(a,"n","");
    a = string_replace_all(a,"o","");
    a = string_replace_all(a,"p","");
    a = string_replace_all(a,"q","");
    a = string_replace_all(a,"r","");
    a = string_replace_all(a,"s","");
    a = string_replace_all(a,"t","");
    a = string_replace_all(a,"u","");
    a = string_replace_all(a,"v","");
    a = string_replace_all(a,"w","");
    a = string_replace_all(a,"x","");
    a = string_replace_all(a,"y","");
    a = string_replace_all(a,"z","");

    //Uppercase Letters are ok
    a = string_replace_all(a,"A","");
    a = string_replace_all(a,"B","");
    a = string_replace_all(a,"C","");
    a = string_replace_all(a,"D","");
    a = string_replace_all(a,"E","");
    a = string_replace_all(a,"F","");
    a = string_replace_all(a,"G","");
    a = string_replace_all(a,"H","");
    a = string_replace_all(a,"I","");
    a = string_replace_all(a,"J","");
    a = string_replace_all(a,"K","");
    a = string_replace_all(a,"L","");
    a = string_replace_all(a,"M","");
    a = string_replace_all(a,"N","");
    a = string_replace_all(a,"O","");
    a = string_replace_all(a,"P","");
    a = string_replace_all(a,"Q","");
    a = string_replace_all(a,"R","");
    a = string_replace_all(a,"S","");
    a = string_replace_all(a,"T","");
    a = string_replace_all(a,"U","");
    a = string_replace_all(a,"V","");
    a = string_replace_all(a,"W","");
    a = string_replace_all(a,"X","");
    a = string_replace_all(a,"Y","");
    a = string_replace_all(a,"Z","");

    if (a==""){
        return true;
    } else {
        return false;
    }
}

bool is_indent(string what){
    if(what.substr(0,1)=="\t" or what.substr(0,4)=="    ")
        return true;
    else
        return false;
}

bool rawvalue_exists(string code){
    if (code=="") return false;

    /*
        code - code
        type - variable type return

        Example:
        "Hello " + "World"\n
    */


    code = string_kill_whitespace(code);

    if (code.substr(0,1)=="+"){
        return false;
    }
    else if (code.substr(0,1)=="-"){
        if (code.substr(1,1)!="0" and code.substr(1,1)!="1" and code.substr(1,1)!="2" and code.substr(1,1)!="3" and code.substr(1,1)!="4"
        and code.substr(1,1)!="5" and code.substr(1,1)!="6" and code.substr(1,1)!="7" and code.substr(1,1)!="8" and code.substr(1,1)!="9"){
            return false;
        } else {
            return true;
        }
    }
    else if (code.substr(0,1)=="*"){
        return false;
    }
    else if (code.substr(0,1)=="/"){
        return false;
    }
    else if (code.substr(0,1)=="("){
        return true;
    }
    else if (code.substr(0,1)==")"){
        return true;
    }
    else if(string_get_until(code," ")=="new"){
        //Create new object

        return true;
    }
    else if(code_parser.arg_type(code)==ARGTYPE_STRING){
        return true;
    }
    else if(code_parser.arg_type(code)==ARGTYPE_NUMBER){
        return true;
    }
    else if(code_parser.arg_type(code)==ARGTYPE_VARIABLE){
        ///TODO Variable Handling code in rawvalue_exists()
        return false;
    }
    else if(code_parser.arg_type(code)==ARGTYPE_FUNCTION){
        ///TODO Function Handling code in rawvalue_exists()
        return false;
    }

    return false;
}

string harvest_string(string& code){
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

string harvest_decimal(string& code){
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

int harvest_raw_expression(string& code, string& exp, string& type, string method = ""){
    /*
        code - code to harvest raw expression from
            example: ("Hello" + " World")blah blah blah

        exp - returns expression

        type - returns type of the expression
            return example: String

        returns success or failure
    */

    exp = "(";
    string code_prev;
    int balance = 0;
    bool accept_value = true;

    while(compile_code.substr(0,1)=="("){
        compile_code = string_delete_amount(compile_code,1);
        balance += 1;
    }

    if(code_parser.harvest_from_variable_value_type(code,type,method)==EXIT_FAILURE)
        return EXIT_FAILURE;

    code = string_kill_whitespace(code);


    while( ((code.substr(0,1)!=")") or balance!=1) and (code_prev!=code) ){
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

                exp += "boomslang_Number(" + harvest_decimal(code) + ")";
            }
            else if(accept_value==true){
                error_fatal("Expected a value before '-'");
                pend();
                return EXIT_FAILURE;
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

            string variable_class = string_get_until_or(code," ;\n+-*/)");

            if(!class_handler.exists(variable_class)){
                error_fatal("Undeclared Template '" + variable_class + "'");
                pend();
                return EXIT_FAILURE;
            } else if(type!=variable_class){
                error_fatal("Template '" + variable_class + "' is Incompatible with Template '" + type +"'");
                pend();
                return EXIT_FAILURE;
            }

            code = string_delete_until_or(code," ;\n+-*/)");
            exp += resource(variable_class) + "()";

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

            exp += "boomslang_String(\"" + harvest_string(code) + "\")";
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

            exp += "boomslang_Number(" + harvest_decimal(code) + ")";
        }
        else if(code_parser.arg_type(code)==ARGTYPE_VARIABLE){
            if(accept_value==false){
                error_fatal("Expected an operator before variable");
                pend();
                return EXIT_FAILURE;
            }
            accept_value = false;

            string variable_name = string_get_until_or(code," =+-/*.)");

            code = string_delete_until_or(code," =+-/*.)");

            code = string_kill_whitespace(code);

            exp += resource(variable_name);

            if(code.substr(0,1)=="."){
                string return_type = variable_handler.variables[variable_handler.find(variable_name,S_NULL,I_NULL,SCOPETYPE_MAIN)].type;
                string prev_return_type = return_type;

                while(code.substr(0,1)=="." or code.substr(0,1)==","){

                    if(code.substr(0,1)==","){
                        if(variable_handler.exists(variable_name,S_NULL,I_NULL,SCOPETYPE_MAIN) and method==""){
                            exp += ";\n";
                            return_type = function_handler.functions[function_handler.find(string_get_until_or(string_delete_amount(code,1)," ("),S_NULL,S_NULL,class_handler.find( variable_handler.variables[variable_handler.find(variable_name,S_NULL,I_NULL,SCOPETYPE_MAIN)].type ),SCOPETYPE_TEMPLATE)].type;
                            exp += resource(variable_name);
                            write_to = &ve_main_code;
                            if(code_parser.parse_function_from(code,true,class_handler.find( variable_handler.variables[variable_handler.find(variable_name,S_NULL,I_NULL,SCOPETYPE_MAIN)].type ))==-1){
                                return EXIT_FAILURE;
                            }
                            prev_return_type = return_type;
                        } else if(variable_handler.exists(variable_name,S_NULL,function_handler.find(method,S_NULL,S_NULL,I_NULL,SCOPETYPE_GLOBAL),SCOPETYPE_FUNCTION) and method!=""){
                            exp += ";\n";
                            return_type = function_handler.functions[function_handler.find(string_get_until_or(string_delete_amount(code,1)," ("),S_NULL,S_NULL,class_handler.find( variable_handler.variables[variable_handler.find(variable_name,S_NULL,function_handler.find(method,S_NULL,S_NULL,I_NULL,SCOPETYPE_GLOBAL),SCOPETYPE_FUNCTION)].type ),SCOPETYPE_TEMPLATE)].type;
                            exp += resource(variable_name);
                            write_to = &ve_main_code;
                            if(code_parser.parse_function_from(code,true,class_handler.find( variable_handler.variables[variable_handler.find(variable_name,S_NULL,function_handler.find(method,S_NULL,S_NULL,I_NULL,SCOPETYPE_GLOBAL),SCOPETYPE_FUNCTION)].type ))==-1){
                                return EXIT_FAILURE;
                            }
                            prev_return_type = return_type;
                        } else {
                            error_fatal("Undeclared Variable '" + variable_name + "'");
                            pend();
                            return EXIT_FAILURE;
                        }
                    }

                    if(code.substr(0,1)=="."){
                        if(function_handler.exists(string_get_until_or(string_delete_amount(code,1)," ("),S_NULL,S_NULL,class_handler.find(prev_return_type),SCOPETYPE_TEMPLATE) and prev_return_type!="none"){
                            return_type = function_handler.functions[function_handler.find(string_get_until_or(string_delete_amount(code,1)," ("),S_NULL,S_NULL,class_handler.find(prev_return_type),SCOPETYPE_TEMPLATE)].type;
                            *write_to = ve_main_code;
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

                exp += ";\n";
            }
        }
        else if(code_parser.arg_type(code)==ARGTYPE_FUNCTION){
            ///TODO Function Handling code in code_parser.harvest_from_variable_value()

            if(accept_value==false){
                error_fatal("Expected an operator before Function");
                pend();
                return EXIT_FAILURE;
            }
            accept_value = false;
        }

        code = string_kill_whitespace(code);
    }

    if(code_prev==code){
        error_fatal("Internal Variable Value Error");
        pend();
        return EXIT_FAILURE;
    }

    code = string_delete_amount(code,1);
    exp += ")";

    return EXIT_SUCCESS;
}
