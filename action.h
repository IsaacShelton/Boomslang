/**
    ============ action.h ============
    Code for Compiling Actions

    This file should NOT be included
    anywhere besides from init.h when
    action is found.

    This Header does NOT contain a
    header guard so include with
    cation.
*/

using namespace std;

if(action=="import"){
    string package_to_import = string_get_until_or(compile_code,";\n");

    compile_code = string_delete_until_or(compile_code,";\n");
    compile_code = string_delete_amount(compile_code,1);

    import_boomslang(package_to_import);
}

if(action=="native_include"){
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

if(action=="register_function"){
    string function_return_type = string_get_until(compile_code," ");

    compile_code = string_delete_until(compile_code," ");
    compile_code = string_kill_whitespace(compile_code);

    string function_name = string_get_until_or(compile_code," (");

    compile_code = string_delete_until_or(compile_code," (");
    compile_code = string_kill_whitespace(compile_code);

    if(compile_code.substr(0,1)!="("){
        error_fatal("Expected '(' before '" + compile_code.substr(0,1) + "' in action 'register_function'.");
        pend();
        return EXIT_FAILURE;
    }

    compile_code = string_delete_amount(compile_code,1);
    compile_code = string_kill_whitespace(compile_code);

    string function_args = "";

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
                        error_fatal("Expected ',' Before Next Class in 'register_function' function arguments.");
                        pend();
                        return EXIT_FAILURE;
                    }
                }
            } else {
                error_fatal("Unknown Class '" + string_get_until_or(compile_code," ,)") + "' in action 'register_function' function arguments.");
                pend();
                return EXIT_FAILURE;
            }
        }
    }

    compile_code = string_delete_amount(compile_code,1);
    code_parser.chop(compile_code);

    function_handler.add(function_name,function_return_type,function_args,I_NULL,SCOPETYPE_GLOBAL);
}

if(action=="register_class_function"){
    string function_return_type = string_get_until(compile_code," ");

    compile_code = string_delete_until(compile_code," ");
    compile_code = string_kill_whitespace(compile_code);

    string function_name = string_get_until_or(compile_code," (");

    compile_code = string_delete_until_or(compile_code," (");
    compile_code = string_kill_whitespace(compile_code);

    if(compile_code.substr(0,1)!="("){
        error_fatal("Expected '(' before '" + compile_code.substr(0,1) + "' in action 'register_function'.");
        pend();
        return EXIT_FAILURE;
    }

    compile_code = string_delete_amount(compile_code,1);
    compile_code = string_kill_whitespace(compile_code);

    string function_args = "";

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
                        error_fatal("Expected ',' Before Next Class in 'register_function' function arguments.");
                        pend();
                        return EXIT_FAILURE;
                    }
                }
            } else {
                error_fatal("Unknown Class '" + string_get_until_or(compile_code," ,)") + "' in action 'register_function' function arguments.");
                pend();
                return EXIT_FAILURE;
            }
        }
    }

    compile_code = string_delete_amount(compile_code,1);

    compile_code = string_delete_amount(compile_code,1);
    code_parser.chop(compile_code);

    function_handler.add(string_delete_amount(string_delete_until(function_name,"."),1),function_return_type,function_args,class_handler.find(string_get_until(function_name,".")),SCOPETYPE_TEMPLATE);
}

if(action="exit"){

}
