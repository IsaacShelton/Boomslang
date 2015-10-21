///Build Information
#include "build.h"

///Basic Headers
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <algorithm>
#include <vector>

///Windows Specific Headers
#if (BUILD_OS == WINDOWS)
#include <windows.h>
#endif

///Linux Specific Headers
#if (BUILD_OS == LINUX)
//Not supported
#include <stdlib.h>
#endif

///Mac Specific Headers
#if (BUILD_OS == Mac)
//Not supported
#include <stdlib.h>
#endif

using namespace std;

//Predefinition
class CodeParser{
public:
    int parse_args(string& code);
    int parse_declaration_args(string& code, string method_name);
    int parse_function_from(string& code, bool check_semicolon, int class_id);
    void chop(string& code);
    int harvest_from_variable_value(string& code, string &type, string additional_characters);
    int harvest_from_variable_value_type(string code, string &type);
    int arg_type(string code);
}code_parser;

class Class{
    //Data
public:
    string name = "";

    Class(string new_name){
        name = new_name;
    }
};

class ClassHandler{
public:
    vector<Class> classes;
    void add(string name);
    bool exists(string name);
    int find(string name);
}class_handler;

class Resource{};

class Variable: public Resource{
    //Data
public:
    string name = "";
    string type = "";
    int parent_id = -1;
    int parent_type = -1;
    unsigned int indent = 0;

    Variable(string new_name, string new_type, int new_parent_id, int new_parent_type, unsigned int new_indent = 0){
        name = new_name;
        type = new_type;
        parent_id = new_parent_id;
        parent_type = new_parent_type;
        indent = new_indent;
    }
};

class VariableHandler{
public:
    vector<Variable> variables;
    void add(string name, string type, int parent_id, int parent_type, unsigned int indent = 0);
    bool exists(string var_name, string var_type, int var_parent_id, int var_parent_type, unsigned int var_indent = 0);
    bool exists_in(string var_name, string var_type, int var_parent_id, int var_parent_type, vector<Variable> your_variables, unsigned int var_indent = 0);
    int find(string var_name, string var_type, int var_parent_id, int var_parent_type);
    int find_in(string var_name, string var_type, int var_parent_id, int var_parent_type, vector<Variable> your_variables);
}variable_handler;

class Function: public Resource{
    //Data
public:
    string name = "";
    string type = "";
    string args = "";/*SomeClass,SomeClass,SomeClass*/
    int parent_id = -1;
    int parent_type = -1;

    Function(string new_name, string new_type, string new_args, int new_parent_id, int new_parent_type){
        name = new_name;
        type = new_type;
        args = new_args;
        parent_id = new_parent_id;
        parent_type = new_parent_type;
    }
};

class FunctionHandler{
    public:
    vector<Function> functions;
    void add(string name, string type, string args, int parent_id, int parent_type);
    bool exists(string func_name, string func_type, string func_args, int func_parent_id, int func_parent_type);
    bool exists_in(string func_name, string func_type, string func_args, int func_parent_id, int func_parent_type, vector<Function> your_functions);
    int find(string func_name, string func_type, string func_args, int func_parent_id, int func_parent_type);
    int find_in(string func_name, string func_type, string func_args, int func_parent_id, int func_parent_type, vector<Function> your_functions);
}function_handler;

//Boomslang Global Variables
#include "globals.h"

//Boomslang String Handling Functions
#include "string_handling.h"

//Boomslang Resource Handling Functions
#include "resource_handling.h"

//Boomslang General Purpose Functions
#include "scripts.h"

//Boomslang File Handling Functions
#include "file.h"

//Boomslang Base Compile Functions
#include "base.h"

int main(int arg_count, char* arg[])
{
    //Start Up Boomslang
    #include "boot.h"

    //Run Boomslang
    #include "init.h"

    //Return Success
    pend();
    return EXIT_SUCCESS;
}
