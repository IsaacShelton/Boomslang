
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
#include <iostream>
#include "../include/globals.h"
#include "../include/scripts.h"
#include "../include/resource.h"
#include "../include/assemble.h"

using namespace std;

int compile(int, char**, string& output);

int init(int arg_count, char** arg){
    error_log("Initializing");

    //Actions
    ve_actions.add("import");
    ve_actions.add("native");
    ve_actions.add("register");
    ve_actions.add("module");
    ve_actions.add("boomslang");

    //Keywords
    ve_keywords.add("on");
    ve_keywords.add("when");
    ve_keywords.add("new");
    ve_keywords.add("unique");
    ve_keywords.add("template");

    //Base Templates
    class_handler.add("String");
    class_handler.add("Number");
    class_handler.add("Byte");
    class_handler.add("Boolean");
    class_handler.add("List");
    class_handler.add("Map");

    //Base Exceptions
    class_handler.add("Error");
    class_handler.add("Exception");
    class_handler.add("NotFound");
    class_handler.add("Invalid");
    class_handler.add("OutOfIndex");
    class_handler.add("MemoryFailure");
    class_handler.add("ZeroDivision");

    function_handler.add("getCause","String","",class_handler.find("Error"),SCOPETYPE_TEMPLATE);
    function_handler.add("getCause","String","",class_handler.find("Exception"),SCOPETYPE_TEMPLATE);
    function_handler.add("getCause","String","",class_handler.find("NotFound"),SCOPETYPE_TEMPLATE);
    function_handler.add("getCause","String","",class_handler.find("Invalid"),SCOPETYPE_TEMPLATE);
    function_handler.add("getCause","String","",class_handler.find("OutOfIndex"),SCOPETYPE_TEMPLATE);
    function_handler.add("getCause","String","",class_handler.find("MemoryFailure"),SCOPETYPE_TEMPLATE);
    function_handler.add("getCause","String","",class_handler.find("ZeroDivision"),SCOPETYPE_TEMPLATE);

    //None
    variable_handler.add("none","",I_NULL,SCOPETYPE_GLOBAL,0);
    variable_handler.add("Arguments","List(String)",I_NULL,SCOPETYPE_MAIN,0);

    //Base Template Functions
    function_handler.add("output","none","",class_handler.find("String"),SCOPETYPE_TEMPLATE);
    function_handler.add("output","none","",class_handler.find("Number"),SCOPETYPE_TEMPLATE);
    function_handler.add("output","none","",class_handler.find("Boolean"),SCOPETYPE_TEMPLATE);
    function_handler.add("input","none","",class_handler.find("String"),SCOPETYPE_TEMPLATE);
    function_handler.add("input","none","",class_handler.find("Number"),SCOPETYPE_TEMPLATE);
    function_handler.add("print","none","",class_handler.find("String"),SCOPETYPE_TEMPLATE);
    function_handler.add("print","none","",class_handler.find("Number"),SCOPETYPE_TEMPLATE);
    function_handler.add("print","none","",class_handler.find("Boolean"),SCOPETYPE_TEMPLATE);
    function_handler.add("wait","none","",class_handler.find("String"),SCOPETYPE_TEMPLATE);

    function_handler.add("string","String","",class_handler.find("Number"),SCOPETYPE_TEMPLATE);
    function_handler.add("number","Number","",class_handler.find("String"),SCOPETYPE_TEMPLATE);

    function_handler.add("string","String","",class_handler.find("Boolean"),SCOPETYPE_TEMPLATE);
    function_handler.add("number","Number","",class_handler.find("Boolean"),SCOPETYPE_TEMPLATE);

    function_handler.add("append","none","",class_handler.find("List"),SCOPETYPE_TEMPLATE);
    function_handler.add("prepend","none","",class_handler.find("List"),SCOPETYPE_TEMPLATE);
    function_handler.add("remove","none","",class_handler.find("List"),SCOPETYPE_TEMPLATE);
    function_handler.add("insert","none","",class_handler.find("List"),SCOPETYPE_TEMPLATE);

    function_handler.add("die","none","",I_NULL,SCOPETYPE_GLOBAL);

    //Open Read and Write Files
    file_read.open(file_read_name.c_str());
    file_write.open(file_write_name.c_str());
    current_filename = file_read_name;

    if(package){
        file_write_header.open(file_write_header_name.c_str());
        file_write_register.open(file_write_register_name.c_str());
    }

    //Ensure they are open
    if (!file_read or !file_write){
        error_fatal("Couldn't open files");
        pend();
        return EXIT_FAILURE;
    } else if(package and (!file_write_header or !file_write_register)){
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

    compile_code += "module *\n";

    //Close the Input File
    file_read.close();

    error_log("Compiling");

    //Include Boomslang Assets
    if(package){
        file_write << "/*This C++ code was generated by boomslang, modify at your own risk*/\n\n#include \"../core/boomslangcore.h\"" << endl << "\nextern int* argc;\nextern char*** argv;\n" << endl;
        file_write_header << "/*This C++ code was generated by boomslang, modify at your own risk*/\n" << endl;
    } else {
        file_write << "/*This C++ code was generated by boomslang, modify at your own risk*/\n\n#include \"../core/boomslangcore.h\"" << endl << "\nint* argc;\nchar*** argv;\n" << endl;
    }

    if(compile(arg_count,arg,ve_main_code)==EXIT_FAILURE) return EXIT_FAILURE;

    file_write.close();

    if(package){
        file_write_header.close();
        file_write_register.close();
    }

    #include "../include/preprocess.h"

    error_log("Assembling");

    if(assemble()==EXIT_FAILURE) return EXIT_FAILURE;

    error_log("Cleaning up");

    #include "../include/cleanup.h"

    error_log("Complete");

    return EXIT_SUCCESS;
}
