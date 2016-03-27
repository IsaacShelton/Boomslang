
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
#include "../include/file.h"
#include "../include/management.h"
#include "../include/resource.h"
#include "../include/branch.h"

using namespace std;

int action(string action_name){
    if(action_name=="import"){
        if(compile_code.substr(0,1)!="\""){
            string package_to_import = string_get_until_or(compile_code,";\n");

            compile_code = string_delete_until_or(compile_code,";\n");
            compile_code = string_delete_amount(compile_code,1);

            import_boomslang(package_to_import);
        } else {
            compile_code = string_delete_amount(compile_code,1);

            string import_file_name = string_get_until_or(compile_code,"\"");
            compile_code = string_delete_until_or(compile_code," \"");
            compile_code = string_delete_amount(compile_code,1);

            if(import_file_name.substr(import_file_name.length()-7,7)==".branch"){
                if(file_exists(filename_path(file_read_name) + import_file_name)){
                    if(branch_load(filename_path(file_read_name) + import_file_name)==EXIT_FAILURE) return EXIT_FAILURE;
                } else if(file_exists(import_file_name)){
                    if(branch_load(import_file_name)==EXIT_FAILURE) return EXIT_FAILURE;
                } else {
                    error_fatal("The branch '" + import_file_name + "' does not exist");
                }
            }
            else if(file_exists(filename_path(file_read_name) + import_file_name)){
                string read_line;
                ifstream import_file_stream;
                string imported_code;
                import_file_stream.open((filename_path(file_read_name) + import_file_name).c_str());

                if(!import_file_stream){
                    error_fatal("Failed to open file '" + (filename_path(file_read_name) + import_file_name) + "'");
                    pend();
                    return EXIT_FAILURE;
                }

                while(getline(import_file_stream,read_line)){
                    imported_code += read_line + "\n";
                }

                compile_code = imported_code + compile_code;

                import_file_stream.close();
            } else if(file_exists(import_file_name)){
                string read_line;
                ifstream import_file_stream;
                string imported_code;
                import_file_stream.open(import_file_name.c_str());

                if(!import_file_stream){
                    error_fatal("Failed to open file '" + (filename_path(file_read_name) + import_file_name) + "'");
                    pend();
                    return EXIT_FAILURE;
                }

                while(getline(import_file_stream,read_line)){
                    imported_code += read_line + "\n";
                }

                compile_code = imported_code + compile_code;

                import_file_stream.close();
            }
        }
    }

    if(action_name=="register"){
        string prev = "";

        bool pure = false;

        while( (string_get_until(compile_code," ")!="template" and string_get_until(compile_code," ")!="method") and compile_code!=prev){
            prev = compile_code;

            if(string_get_until(compile_code," ")=="pure"){
                compile_code = string_delete_until(compile_code," ");
                pure = true;
                continue;
            }

            compile_code = string_kill_whitespace(compile_code);
        }

        if(prev == compile_code){
            error_fatal("Internal Error in register statement");
            pend();
            return EXIT_FAILURE;
        }

        string register_type = string_get_until(compile_code," ");
        compile_code = string_delete_until(compile_code," ");
        compile_code = string_kill_whitespace(compile_code);

        if(register_type=="template"){
            string template_name = string_get_until_or(compile_code," \n");
            compile_code = string_delete_until_or(compile_code," \n");

            if(class_handler.exists(template_name)){
                error_fatal("The template '" + template_name + "' was already declared");
                pend();
                return EXIT_FAILURE;
            }

            class_handler.add(template_name);
        }
        if(register_type=="method"){
            string method_name;
            string function_args;

            method_name = string_get_until_or(compile_code," (");
            compile_code = string_delete_until_or(compile_code," (");
            compile_code = string_kill_whitespace(compile_code);

            if(compile_code.substr(0,1)!="("){
                error_fatal("Expected '(' before '" + compile_code.substr(0,1) + "' in register method arguments");
                pend();
                return EXIT_FAILURE;
            }

            compile_code = string_delete_amount(compile_code,1);

            if(compile_code.substr(0,1)!=")"){
                //Parse Function arguments

                while(compile_code.substr(0,1)!=")"){
                    if(class_handler.exists(string_get_until_or(compile_code," ,)"))){
                        function_args += string_get_until_or(compile_code," ,)");
                        compile_code = string_delete_until_or(compile_code," ,)");

                        if(compile_code.substr(0,1)!=")"){
                            compile_code = string_kill_whitespace(compile_code);

                            if(compile_code.substr(0,1)==","){
                                function_args += ",";
                                compile_code = string_delete_amount(compile_code,1);
                                compile_code = string_kill_whitespace(compile_code);
                            } else {
                                error_fatal("Expected ',' Before Next Template in register method arguments");
                                pend();
                                return EXIT_FAILURE;
                            }
                        }
                    } else {
                        error_fatal("Unknown Template '" + string_get_until_or(compile_code," ,)") + "' in register method arguments");
                        pend();
                        return EXIT_FAILURE;
                    }
                }
            }

            compile_code = string_delete_amount(compile_code,1);
            compile_code = string_kill_whitespace(compile_code);

            if(compile_code.substr(0,2)!="->"){
                error_fatal("Expected '->' before '" + compile_code.substr(0,1) + "' in register method arguments");
                pend();
                return EXIT_FAILURE;
            }

            compile_code = string_delete_amount(compile_code,2);
            compile_code = string_kill_whitespace(compile_code);

            string return_type = string_get_until_or(compile_code," \n\r");
            compile_code = string_delete_until_or(compile_code," \n\r");

            if(!class_handler.exists(return_type) and return_type!="none"){
                error_fatal("Unknown Template '' in register method return type");
                pend();
                return EXIT_FAILURE;
            }

            if(string_contains(method_name,".")){
                function_handler.add(string_delete_amount(string_delete_until(method_name,"."),1),return_type,function_args,class_handler.find(string_get_until(method_name,".")),SCOPETYPE_TEMPLATE);
                unsigned int new_function = function_handler.find(string_delete_amount(string_delete_until(method_name,"."),1),return_type,function_args,class_handler.find(string_get_until(method_name,".")),SCOPETYPE_TEMPLATE);

                if(pure) function_handler.functions[new_function].pure = true;
            } else {
                function_handler.add(method_name,return_type,function_args,I_NULL,SCOPETYPE_GLOBAL);
            }

            compile_code = string_kill_whitespace(compile_code);
        }
    }

    if(action_name=="native"){
        if(string_get_until(compile_code," ")=="header"){
            compile_code = string_delete_until(compile_code," ");
            compile_code = string_kill_whitespace(compile_code);

            string header_to_include = string_get_until_or(compile_code,";\n");

            header_to_include = delete_backslash(header_to_include);

            if(file_exists("C:\\Users\\" + USERNAME + "\\AppData\\Roaming\\Boomslang\\packages\\" + header_to_include)){
                file_write << "#include \"" + string("C:\\Users\\") + USERNAME + "\\AppData\\Roaming\\Boomslang\\packages\\" + header_to_include + "\"" << endl;
            } else {
                error_show("Couldn't find Native Header '" + string("C:\\Users\\") + USERNAME + "\\AppData\\Roaming\\Boomslang\\packages\\" + header_to_include + "'");
            }

            compile_code = string_delete_until_or(compile_code,";\n");
            compile_code = string_delete_amount(compile_code,1);
        }
        if(string_get_until(compile_code," ")=="library"){
            compile_code = string_delete_until(compile_code," ");
            compile_code = string_kill_whitespace(compile_code);

            string library_to_include = string_get_until_or(compile_code,";\n");

            library_to_include = delete_backslash(library_to_include);

            if(file_exists("C:\\Users\\" + USERNAME + "\\AppData\\Roaming\\Boomslang\\packages\\" + library_to_include)){
                linker_flags += " \"" + string("C:\\Users\\") + USERNAME + "\\AppData\\Roaming\\Boomslang\\packages\\" + library_to_include + "\" ";
            } else {
                error_show("Couldn't find Native Library '" + string("C:\\Users\\") + USERNAME + "\\AppData\\Roaming\\Boomslang\\packages\\" + library_to_include + "'");
            }

            compile_code = string_delete_until_or(compile_code,";\n");
            compile_code = string_delete_amount(compile_code,1);
        }
        if(string_get_until(compile_code," ")=="stdlib"){
            compile_code = string_delete_until(compile_code," ");
            compile_code = string_kill_whitespace(compile_code);

            string header_to_include = string_get_until_or(compile_code,";\n");

            header_to_include = delete_backslash(header_to_include);

            if(file_exists("C:\\Users\\" + USERNAME + "\\AppData\\Roaming\\Boomslang\\resources\\MinGW\\lib\\" + header_to_include)){
                linker_flags += " \"" + string("C:\\Users\\") + USERNAME + "\\AppData\\Roaming\\Boomslang\\resources\\MinGW\\lib\\" + header_to_include + "\" ";
            } else {
                error_show("Couldn't find Native Header '" + string("C:\\Users\\") + USERNAME + "\\AppData\\Roaming\\Boomslang\\resources\\MinGW\\lib\\" + header_to_include + "'");
            }

            compile_code = string_delete_until_or(compile_code,";\n");
            compile_code = string_delete_amount(compile_code,1);
        }
    }
}
