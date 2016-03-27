
#ifndef RESOURCE_H_INCLUDED
#define RESOURCE_H_INCLUDED

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

using namespace std;

#include <vector>
#include "base.h"

class Class{
    public:
    string name = "";
    vector<unsigned int> parents;

    Class(string new_name);
};
class ClassHandler{
public:
    vector<Class> classes;
    void add(string name);
    bool exists(string name);
    int find(string name);
    Class get(string name);
};

class Resource{};

class Variable: public Resource{
    //Data
public:
    string name = "";
    string type = "";
    string sub_types = "";
    int parent_id = -1;
    int parent_type = -1;
    unsigned int indent = 0;
    bool is_unique = false;

    Variable(string new_name, string new_type, int new_parent_id, int new_parent_type, unsigned int new_indent = 0);
};
class VariableHandler{
    public:
    vector<Variable> variables;
    void add(string name, string type, int parent_id, int parent_type, unsigned int indent = 0);
    bool exists(string var_name, string var_type, int var_parent_id, int var_parent_type, unsigned int var_indent = 0);
    bool available(string var_name, string var_type, string method_name, string template_name);
    Variable available_get(string var_name, string var_type, string method_name, string template_name);
    bool exists_in(string var_name, string var_type, int var_parent_id, int var_parent_type, vector<Variable> your_variables, unsigned int var_indent = 0);
    int find(string var_name, string var_type, int var_parent_id, int var_parent_type);
    Variable get(string var_name, string var_type, int var_parent_id, int var_parent_type);
    int find_in(string var_name, string var_type, int var_parent_id, int var_parent_type, vector<Variable> your_variables);
};

class Function: public Resource{
    public:
    string name = "";
    string type = "";
    string args = "";/*SomeClass,SomeClass,SomeClass*/
    int parent_id = -1;
    int parent_type = -1;
    bool pure = false;

    Function(string new_name, string new_type, string new_args, int new_parent_id, int new_parent_type);
};
class FunctionHandler{
    public:
    vector<Function> functions;
    void add(string name, string type, string args, int parent_id, int parent_type);
    bool exists(string func_name, string func_type, string func_args, int func_parent_id, int func_parent_type);
    bool exists_in(string func_name, string func_type, string func_args, int func_parent_id, int func_parent_type, vector<Function> your_functions);
    int find(string func_name, string func_type, string func_args, int func_parent_id, int func_parent_type);
    int find_in(string func_name, string func_type, string func_args, int func_parent_id, int func_parent_type, vector<Function> your_functions);
    Function get(string func_name, string func_type, string func_args, int func_parent_id, int func_parent_type);
};

extern ClassHandler class_handler;
extern VariableHandler variable_handler;
extern FunctionHandler function_handler;

string string_template(string);
string string_base_template(string);
string string_sub_template(string);

#endif // RESOURCE_H_INCLUDED
