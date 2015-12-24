
#include <string>
#include <iostream>
#include "../include/globals.h"
#include "../include/scripts.h"
#include "../include/resource.h"
#include "../include/assemble.h"

using namespace std;

int compile(int, char**);

int init(int arg_count, char** arg){
    error_log("Initializing");

    //Actions
    ve_actions.add("use");
    ve_actions.add("native_include");
    ve_actions.add("native_library");
    ve_actions.add("native_standard_library");
    ve_actions.add("register_class");
    ve_actions.add("register_class_function");
    ve_actions.add("register_function");

    //Keywords
    ve_keywords.add("on");
    ve_keywords.add("new");
    ve_keywords.add("template");

    //Base Templates
    class_handler.add("String");
    class_handler.add("Number");
    class_handler.add("Byte");

    //None
    variable_handler.add("none","",SCOPETYPE_GLOBAL,I_NULL);

    //Base Template Functions
    function_handler.add("output","none","",class_handler.find("String"),SCOPETYPE_TEMPLATE);
    function_handler.add("output","none","",class_handler.find("Number"),SCOPETYPE_TEMPLATE);
    function_handler.add("input","none","",class_handler.find("String"),SCOPETYPE_TEMPLATE);
    function_handler.add("input","none","",class_handler.find("Number"),SCOPETYPE_TEMPLATE);
    function_handler.add("print","none","",class_handler.find("String"),SCOPETYPE_TEMPLATE);
    function_handler.add("print","none","",class_handler.find("Number"),SCOPETYPE_TEMPLATE);
    function_handler.add("wait","none","",class_handler.find("String"),SCOPETYPE_TEMPLATE);
    function_handler.add("call","none","",class_handler.find("Callback"),SCOPETYPE_TEMPLATE);

    //Open Read and Write Files
    file_read.open(file_read_name.c_str());
    file_write.open(file_write_name.c_str());

    //Ensure they are open
    if (!file_read.is_open() and !file_write.is_open()){
        error_fatal("Couldn't open files");
        pend();
        return EXIT_FAILURE;
    }

    //Variable to contain current line of code that we are extracting
    string line;

    error_log("Extracting");

    //Extract
    while(getline(file_read,line)){
        compile_code += line + "\n";
    }

    //Close the Input File
    file_read.close();

    error_log("Compiling");

    //Include Boomslang Assets
    file_write << "#include \"../core/boomslangcore.h\"" << endl;

    if(compile(arg_count,arg)==EXIT_FAILURE) return EXIT_FAILURE;

    file_write.close();

    #include "../include/preprocess.h"

    error_log("Assembling");

    if(assemble()==EXIT_FAILURE) return EXIT_FAILURE;

    error_log("Cleaning up");

    #include "../include/cleanup.h"

    error_log("Complete");
}
