
using namespace std;

string variable_type = S_NULL;
compile_code = string_kill_all_whitespace(compile_code);

//Get Value Type
if(code_harvest_value_type(compile_code,variable_type)==EXIT_FAILURE){
    error_fatal("Couldn't Determine Type for Variable '" + variable_name + "'");
    pend();
    return EXIT_FAILURE;
}

*write_to += resource(variable_type) + " " + resource(variable_name) + "=";

//Handle Value
if(code_harvest_value(compile_code,variable_type,"")==EXIT_FAILURE){
    return EXIT_FAILURE;
}

code_chop(compile_code);

*write_to += ";\n";

if(method_name==""){
    if(template_name==""){
        variable_handler.add(variable_name,variable_type,I_NULL,SCOPETYPE_MAIN,indentation);
    } else {
        variable_handler.add(variable_name,variable_type,class_handler.find(template_name),SCOPETYPE_TEMPLATE);
    }
} else {
    if(template_name==""){
        variable_handler.add(variable_name,variable_type,function_handler.find(method_name,S_NULL,S_NULL,class_handler.find(template_name),SCOPETYPE_TEMPLATE),SCOPETYPE_FUNCTION);
    } else {
        variable_handler.add(variable_name,variable_type,function_handler.find(method_name,S_NULL,S_NULL,I_NULL,SCOPETYPE_GLOBAL),SCOPETYPE_FUNCTION);
    }
}
