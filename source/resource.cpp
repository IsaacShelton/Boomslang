
using namespace std;

#include <string>
#include <stdlib.h>
#include <iostream>
#include "../include/resource.h"
#include "../include/management.h"

using namespace std;

Class::Class(string new_name){
    name = new_name;
}
Variable::Variable(string new_name, string new_type, int new_parent_id, int new_parent_type, unsigned int new_indent){
    name = new_name;
    type = new_type;
    parent_id = new_parent_id;
    parent_type = new_parent_type;
    indent = new_indent;
}
Function::Function(string new_name, string new_type, string new_args, int new_parent_id, int new_parent_type){
    name = new_name;
    type = new_type;
    args = new_args;
    parent_id = new_parent_id;
    parent_type = new_parent_type;
}

//Variables
void VariableHandler::add(string name, string type, int parent_id, int parent_type, unsigned int indent){
    Variable var = Variable(name,type,parent_id,parent_type,indent);
    var.is_unique = false;
    variables.push_back( var );
}

bool VariableHandler::exists(string var_name, string var_type, int var_parent_id, int var_parent_type, unsigned int var_indent){
    bool name_null = (var_name==" ");//" " == null
    bool type_null = (var_type==" ");//" " == null
    bool parent_id_null = (var_parent_id==-1);
    bool parent_type_null = (var_parent_type==-1);

    for(unsigned int i=0;i<variables.size();i++){
        if (
                (variables[i].name == var_name or name_null)
            and (variables[i].type == var_type or type_null)
            and (variables[i].parent_id == var_parent_id or parent_id_null)
            and (variables[i].parent_type == var_parent_type or parent_type_null)
            and (variables[i].indent <= var_indent)
        ) return true;
    }

    return false;
}

bool VariableHandler::exists_in(string var_name, string var_type, int var_parent_id, int var_parent_type, vector<Variable> your_variables, unsigned int var_indent){
    bool name_null = (var_name==" ");//" " == null
    bool type_null = (var_type==" ");//" " == null
    bool parent_id_null = (var_parent_id==-1);
    bool parent_type_null = (var_parent_type==-1);

    for(unsigned int i=0;i<your_variables.size();i++){
        if (
                (your_variables[i].name == var_name or name_null)
            and (your_variables[i].type == var_type or type_null)
            and (your_variables[i].parent_id == var_parent_id or parent_id_null)
            and (your_variables[i].parent_type == var_parent_type or parent_type_null)
            and (your_variables[i].indent <= var_indent)
        ) return true;
    }

    return false;
}

int VariableHandler::find(string var_name, string var_type, int var_parent_id, int var_parent_type){
    bool name_null = (var_name==" ");//" " == null
    bool type_null = (var_type==" ");//" " == null
    bool parent_id_null = (var_parent_id==-1);
    bool parent_type_null = (var_parent_type==-1);

    for(unsigned int i=0;i<variables.size();i++){
        if (
                (variables[i].name == var_name or name_null)
            and (variables[i].type == var_type or type_null)
            and (variables[i].parent_id == var_parent_id or parent_id_null)
            and (variables[i].parent_type == var_parent_type or parent_type_null)
        ) return i;
    }

    return -1;
}

int VariableHandler::find_in(string var_name, string var_type, int var_parent_id, int var_parent_type, vector<Variable> your_variables){
        bool name_null = (var_name==" ");//" " == none
        bool type_null = (var_type==" ");//" " == none
        bool parent_id_null = (var_parent_id==-1);
        bool parent_type_null = (var_parent_type==-1);

        for(unsigned int i=0;i<your_variables.size();i++){
            if (
                    (your_variables[i].name == var_name or name_null)
                and (your_variables[i].type == var_type or type_null)
                and (your_variables[i].parent_id == var_parent_id or parent_id_null)
                and (your_variables[i].parent_type == var_parent_type or parent_type_null)
            ) return i;
        }

        return -1;
    }

//Functions
void FunctionHandler::add(string name, string type, string args, int parent_id, int parent_type){
    Function func = Function(name,type,args,parent_id,parent_type);
    functions.push_back( func );
}

bool FunctionHandler::exists(string func_name, string func_type, string func_args, int func_parent_id, int func_parent_type){
    bool name_null = (func_name==" ");//" " == none
    bool type_null = (func_type==" ");//" " == none
    bool args_null = (func_args==" ");//" " == none
    bool parent_id_null = (func_parent_id==-1);
    bool parent_type_null = (func_parent_type==-1);

    for(unsigned int i=0;i<functions.size();i++){
        if (
                (functions[i].name == func_name or name_null)
            and (functions[i].type == func_type or type_null)
            and (functions[i].args == func_args or args_null)
            and (functions[i].parent_id == func_parent_id or parent_id_null)
            and (functions[i].parent_type == func_parent_type or parent_type_null)
        ) return true;
    }

    return false;
}

bool FunctionHandler::exists_in(string func_name, string func_type, string func_args, int func_parent_id, int func_parent_type, vector<Function> your_functions){
    bool name_null = (func_name==" ");//" " == none
    bool type_null = (func_type==" ");//" " == none
    bool args_null = (func_args==" ");//" " == none
    bool parent_id_null = (func_parent_id==-1);
    bool parent_type_null = (func_parent_type==-1);

    for(unsigned int i=0;i<your_functions.size();i++){
        if (
                (your_functions[i].name == func_name or name_null)
            and (your_functions[i].type == func_type or type_null)
            and (your_functions[i].args == func_args or args_null)
            and (your_functions[i].parent_id == func_parent_id or parent_id_null)
            and (your_functions[i].parent_type == func_parent_type or parent_type_null)
        ) return true;
    }

    return false;
}

int FunctionHandler::find(string func_name, string func_type, string func_args, int func_parent_id, int func_parent_type){
    bool name_null = (func_name==" ");//" " == none
    bool type_null = (func_type==" ");//" " == none
    bool args_null = (func_args==" ");//" " == none
    bool parent_id_null = (func_parent_id==-1);
    bool parent_type_null = (func_parent_type==-1);

    for(unsigned int i=0;i<functions.size();i++){
        if (
                (functions[i].name == func_name or name_null)
            and (functions[i].type == func_type or type_null)
            and (functions[i].args == func_args or args_null)
            and (functions[i].parent_id == func_parent_id or parent_id_null)
            and (functions[i].parent_type == func_parent_type or parent_type_null)
        ) return i;
    }

    return -1;
}

int FunctionHandler::find_in(string func_name, string func_type, string func_args, int func_parent_id, int func_parent_type, vector<Function> your_functions){
    bool name_null = (func_name==" ");//" " == none
    bool type_null = (func_type==" ");//" " == none
    bool args_null = (func_args==" ");//" " == none
    bool parent_id_null = (func_parent_id==-1);
    bool parent_type_null = (func_parent_type==-1);

    for(unsigned int i=0;i<your_functions.size();i++){
        if (
                (your_functions[i].name == func_name or name_null)
            and (your_functions[i].type == func_type or type_null)
            and (your_functions[i].args == func_args or args_null)
            and (your_functions[i].parent_id == func_parent_id or parent_id_null)
            and (your_functions[i].parent_type == func_parent_type or parent_type_null)
        ) return i;
    }

    return -1;
}

//Classes
void ClassHandler::add(string name){
    Class klass = Class(name);
    classes.push_back( klass );
}

bool ClassHandler::exists(string name){
    bool name_null = (name==" ");//" " == none

    for(unsigned int i=0;i<classes.size();i++){
        if (
            (classes[i].name == name or name_null)
        ) return true;
    }

    return false;
}

int ClassHandler::find(string name){
    bool name_null = (name==" ");//" " == none

    for(unsigned int i=0;i<classes.size();i++){
        if (
            (classes[i].name == name or name_null)
        ) return i;
    }

    return EXIT_FAILURE;
}

string string_template(string template_name){
    string output_template = resource(string_get_until(template_name,"("));

    if(string_count(template_name,"(") > 0){
        template_name = string_delete_until(template_name,"(");
    } else {
        return resource(template_name);
    }

    template_name = string_delete_amount(template_name,1);

    string prev;
    output_template += "<";

    while(template_name!="" and prev!=template_name){
        prev = template_name;

        if(template_name.substr(0,1)==","){
            template_name = string_delete_amount(template_name,1);
            output_template += ",";
        }

        if(template_name.substr(0,1)==")"){
            template_name = string_delete_amount(template_name,1);
            break;
        }

        output_template += resource(string_get_until_or(template_name,",)"));

        if(string_count(template_name,",") > 0){
            template_name = string_delete_until_or(template_name,",)");
        } else {
            template_name = "";
        }
    }

    output_template += ">";

    if(template_name==prev){
        error_fatal("Failed to parse internal sub templates");
        return "";
    }

    return output_template;
}

