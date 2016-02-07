
#include <iostream>
#include <string>
#include "../include/base.h"
#include "../include/globals.h"
#include "../include/scripts.h"
#include <algorithm>

using namespace std;

//Checks if parent string contains substring
bool string_contains(string parent_string, string sub_string){
    if (parent_string.find(sub_string)==string::npos)
        return false;
    else
        return true;
}

//Gets text until character
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

unsigned int string_count(string a, string character){
    unsigned int char_count = 0;
    for(unsigned int i = 0; i < a.length(); i++){
        if(a.substr(i,1)==character) char_count++;
    }
    return char_count;
}

//Deletes text until character
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

//Gets text until character(s)
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

//Deletes text until character(s)
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

//Deletes the amount of characters from the start of the string
string string_delete_amount(string str, int num){
    return str.substr(num,str.length()-num);
}

//Replaces first string with new string
string string_replace(string str, const std::string from, const string to) {
    size_t start_pos = str.find(from);
    if(start_pos == std::string::npos)
        return "";
    else
        return str.replace(start_pos, from.length(), to);
}

//Replaces all string(s) with new string
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

//Kills spaces and tabs
string string_kill_whitespace(string str){
    int n = 0;

    while(!(str.substr(n,1)!=" " and str.substr(n,1)!="\t")){
        n++;
    }

    return string_delete_amount(str,n);
}

//Kills spaces, tabs, and newlines
string string_kill_all_whitespace(string str){
    int n = 0;

    while(!(str.substr(n,1)!=" " and str.substr(n,1)!="\t" and str.substr(n,1)!="\n")){
        n++;
    }

    return string_delete_amount(str,n);
}

//Kills newlines
string string_kill_newline(string str){
    int n = 0;

    while((str.substr(n,1)=="\n")){
        n++;
    }

    return string_delete_amount(str,n);
}

//Gets a boomslang resource name
string resource(string a){
    return "boomslang_" + string_replace_all(string_replace_all(a,":","::boomslang_"),".",".boomslang_");
}

//Deletes Backslash if there is one
string delete_backslash(string a){
    if (a.substr(0,1)=="\\" or a.substr(0,1)=="/")
        return string_delete_amount(a,1);
    else
        return a;
}

//Turns the string uppercase
string string_upper(string a){
    ::transform(a.begin(), a.end(), a.begin(), ::toupper);
    return a;
}

//Checks to see if the string is an identifier
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

//Checks to see if the string is an identifier
bool is_identifier(string what, string extras){
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

    while(extras!=""){
        a = string_replace_all(a,extras.substr(0,1),"");
        extras = string_delete_amount(extras,1);
    }

    if (a==""){
        return true;
    } else {
        return false;
    }
}

//Checks the see if the string is an indent character
bool is_indent(string what){
    if(what.substr(0,1)=="\t" or what.substr(0,4)=="    ")
        return true;
    else
        return false;
}

//Checks to see if a raw value exists
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
    else if(code_arg_type(code)==ARGTYPE_STRING){
        return true;
    }
    else if(code_arg_type(code)==ARGTYPE_NUMBER){
        return true;
    }
    else if(code_arg_type(code)==ARGTYPE_VARIABLE){
        ///TODO Variable Handling code in rawvalue_exists()
        return false;
    }
    else if(code_arg_type(code)==ARGTYPE_FUNCTION){
        ///TODO Function Handling code in rawvalue_exists()
        return false;
    }

    return false;
}
