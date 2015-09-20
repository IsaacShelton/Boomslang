/**
    ============ init.h ============
    Code for Running Boomslang

    This file should NOT be included
    anywhere besides from main.cpp.

    This Header does NOT contain a
    header guard so include with
    cation.
*/

using namespace std;

error_log("Initializing");

function_handler.add("native_execute","null","String",0,SCOPETYPE_GLOBAL);

ve_actions.add("import");
ve_actions.add("type");
ve_actions.add("native_include");
ve_actions.add("register_class");
ve_actions.add("register_class_function");
ve_actions.add("register_function");

ve_keywords.add("void");
ve_keywords.add("or");
ve_keywords.add("and");
ve_keywords.add("if");
ve_keywords.add("public");
ve_keywords.add("global");
ve_keywords.add("template");
ve_keywords.add("object");

class_handler.add("String");
class_handler.add("Number");

function_handler.add("output","null","",class_handler.find("String"),SCOPETYPE_TEMPLATE);
function_handler.add("output","null","",class_handler.find("Number"),SCOPETYPE_TEMPLATE);
function_handler.add("input","null","",class_handler.find("String"),SCOPETYPE_TEMPLATE);
function_handler.add("input","null","",class_handler.find("Number"),SCOPETYPE_TEMPLATE);
function_handler.add("print","null","",class_handler.find("String"),SCOPETYPE_TEMPLATE);
function_handler.add("print","null","",class_handler.find("Number"),SCOPETYPE_TEMPLATE);

file_read.open(file_read_name.c_str());
file_write.open(file_write_name.c_str());

if (!file_read.is_open() and !file_write.is_open()){
    error_fatal("Couldn't open files");
    pend();
    return EXIT_FAILURE;
}

string line;

error_log("Extracting");

while(getline(file_read,line)){
    compile_code += line + "\n";
}

file_read.close();

error_log("Compiling");

file_write << "#include \"boomslang.h\"" << endl;

#include "compile.h"

file_write.close();

#include "preprocess.h"

error_log("Assembling");

#include "assemble.h"

error_log("Cleaning up");

#include "cleanup.h"

error_log("Complete");
