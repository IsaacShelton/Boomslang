
/*
    (c) 2016 Isaac Shelton

    This file is part of Boomslang.

    Boomslang is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Boomslang is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Boomslang. If not, see <http://www.gnu.org/licenses/>.
*/

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>

using namespace std;

//Converts string to double
double to_double(string str){
    double dec;
    if( ! (std::istringstream(str) >> dec) ) dec = 0;
    return dec;
}

//Converts double to string
string to_string(double dec){
    std::ostringstream convert;
    convert << dec;
    return convert.str();
}

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

    while(!(str.substr(n,1)!=" " and str.substr(n,1)!="\t" and str.substr(n,1)!="\n"  and str.substr(n,1)!="\r")){
        n++;
    }

    return string_delete_amount(str,n);
}

//Kills newlines
string string_kill_newline(string str){
    int n = 0;

    while((str.substr(n,1)=="\n" and str.substr(n,1)!="\r")){
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
    char character;

    //Is it blank?
    if (what=="") return false;

    character = what[0];

    //Does it start with a number?
    if ( (int)character >= 48 and (int)character <= 57) return false;

    for(unsigned int i = 0; i < what.length(); i++){
        character = what[i];

        if( !((int)character>=48&&(int)character<=57)
        &&  !((int)character>=65&&(int)character<=90)
        &&  !((int)character>=97&&(int)character<=122)
        &&  !((int)character==95) ){
            return false;
        }
    }

    return true;
}

//Checks the see if the string is an indent character
bool is_indent(string what){
    if(what.substr(0,1)=="\t" or what.substr(0,4)=="    ")
        return true;
    else
        return false;
}

//Gets the name of the file from path and filename
string filename_name(string a){
    if (a.find_last_of("\\/")==string::npos){
        return a;
    } else {
        return string_delete_amount(a.substr(a.find_last_of("\\/"), a.length() - a.find_last_of("\\/")),1);
    }
}

//Gets the path of the file from path and filename
string filename_path(string a){
    if (a.find_last_of("\\/")==string::npos){
        return a;
    } else {
        return a.substr(0, a.find_last_of("\\/")) + "/";
    }
}

//Gets the new filename from filename and new extension
string filename_change_ext(string filename, string ext_without_dot){
    string file_no_ext = string_get_until(filename,".");
    return file_no_ext + "." + ext_without_dot;
}

ifstream::pos_type file_size(string size_filename){
    std::ifstream file_stream(size_filename.c_str(), std::ifstream::ate | std::ifstream::binary);
    unsigned int size_of_file = file_stream.tellg();
    file_stream.close();
    return size_of_file;
}
