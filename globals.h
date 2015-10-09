/**
    ============ globals.h ============
    Code for global variables and
    constants.

    This file should NOT be included
    anywhere besides from main.cpp.

    This Header does NOT contain a
    header guard so include with
    cation.
*/

#define APPTYPE_WINDOWS WINDOWS
#define APPTYPE_ANDROID ANDROID
#define APPTYPE_LINUX LINUX

#define SCOPETYPE_VARIABLE 0
#define SCOPETYPE_FUNCTION 1
#define SCOPETYPE_GLOBAL 2
#define SCOPETYPE_MAIN 3
#define SCOPETYPE_TEMPLATE 4

#define S_NULL " "
#define I_NULL -1

#define ARGTYPE_STRING 0
#define ARGTYPE_NUMBER 1
#define ARGTYPE_VARIABLE 2
#define ARGTYPE_FUNCTION 3

bool string_contains(string,string);

class Container{
public:
    string data = ";";

    bool exists(string what){
        if(string_contains(data,";" + what + ";") and what!="")
            return true;
        else
            return false;
    }

    void add(string what){
        data += what + ";";
    }
};

string USERNAME = getenv("USERNAME");
string file_read_name = "";
string file_write_name = "C:\\Users\\" + USERNAME + "\\AppData\\Roaming\\Boomslang\\source\\prenative.cpp";
int ve_apptype = APPTYPE_WINDOWS;
bool console = false;

Container ve_actions;

Container ve_constants;

Container ve_keywords;

Container ve_packages;
string ve_main_code = "";
string ve_scope = "BOOMSLANG";
string ve_context = S_NULL;

bool waitafter = false;
string compile_flags = "";
string compile_code = "";
ifstream file_read;
ofstream file_write;

int errors = 0;
int warnings = 0;
