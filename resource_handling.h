/**
    ============ resource_handling.h ============
    Code for Compile Resource Functions

    This file should NOT be included
    anywhere besides from main.cpp.

    This Header does NOT contain a
    header guard so include with
    cation.
*/

using namespace std;

//Base Resource Class
class Resource{};

//Variables
class Variable: public Resource{
    //Data
public:
    string name = "";
    string type = "";
    int parent_id = -1;
    int parent_type = -1;

    Variable(string new_name, string new_type, int new_parent_id, int new_parent_type){
        name = new_name;
        type = new_type;
        parent_id = new_parent_id;
        parent_type = new_parent_type;
    }
};

class VariableHandler{
    //Handle Variables
public:
    vector<Variable> variables;

    void add(string name, string type, int parent_id, int parent_type){
        Variable* ptr_var = (new Variable(name,type,parent_id,parent_type));
        variables.push_back( *ptr_var );
        delete ptr_var;
    }

    bool exists(string var_name, string var_type, int var_parent_id, int var_parent_type){
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
            ) return true;
        }

        return false;
    }

    bool exists_in(string var_name, string var_type, int var_parent_id, int var_parent_type, vector<Variable> your_variables){
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
            ) return true;
        }

        return false;
    }

    int find(string var_name, string var_type, int var_parent_id, int var_parent_type){
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

        return EXIT_FAILURE;
    }

    int find_in(string var_name, string var_type, int var_parent_id, int var_parent_type, vector<Variable> your_variables){
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
            ) return i;
        }

        return EXIT_FAILURE;
    }
}variable_handler;

//Functions
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
    //Handle Functions
public:
    vector<Function> functions;

    void add(string name, string type, string args, int parent_id, int parent_type){
        Function* ptr_function = (new Function(name,type,args,parent_id,parent_type));
        functions.push_back( *ptr_function );
        delete ptr_function;
    }

    bool exists(string func_name, string func_type, string func_args, int func_parent_id, int func_parent_type){
        bool name_null = (func_name==" ");//" " == null
        bool type_null = (func_type==" ");//" " == null
        bool args_null = (func_args==" ");//" " == null
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

    bool exists_in(string func_name, string func_type, string func_args, int func_parent_id, int func_parent_type, vector<Function> your_functions){
        bool name_null = (func_name==" ");//" " == null
        bool type_null = (func_type==" ");//" " == null
        bool args_null = (func_args==" ");//" " == null
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

    int find(string func_name, string func_type, string func_args, int func_parent_id, int func_parent_type){
        bool name_null = (func_name==" ");//" " == null
        bool type_null = (func_type==" ");//" " == null
        bool args_null = (func_args==" ");//" " == null
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

        return EXIT_FAILURE;
    }

    int find_in(string func_name, string func_type, string func_args, int func_parent_id, int func_parent_type, vector<Function> your_functions){
        bool name_null = (func_name==" ");//" " == null
        bool type_null = (func_type==" ");//" " == null
        bool args_null = (func_args==" ");//" " == null
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

        return EXIT_FAILURE;
    }
}function_handler;

//Classes
class Class{
    //Data
public:
    string name = "";

    Class(string new_name){
        name = new_name;
    }
};

class ClassHandler{
        //Handle Functions
public:
    vector<Class> classes;

    void add(string name){
        Class* ptr_class = (new Class(name));
        classes.push_back( *ptr_class );
        delete ptr_class;
    }

    bool exists(string name){
        bool name_null = (name==" ");//" " == null

        for(unsigned int i=0;i<classes.size();i++){
            if (
                (classes[i].name == name or name_null)
            ) return true;
        }

        return false;
    }

    int find(string name){
        bool name_null = (name==" ");//" " == null

        for(unsigned int i=0;i<classes.size();i++){
            if (
                (classes[i].name == name or name_null)
            ) return i;
        }

        return EXIT_FAILURE;
    }
}class_handler;
