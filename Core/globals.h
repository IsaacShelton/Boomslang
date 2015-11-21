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

//Platforms
#define APPTYPE_WINDOWS WINDOWS
#define APPTYPE_ANDROID ANDROID
#define APPTYPE_LINUX LINUX

//Scope Types
#define SCOPETYPE_VARIABLE 0
#define SCOPETYPE_FUNCTION 1
#define SCOPETYPE_GLOBAL 2
#define SCOPETYPE_MAIN 3
#define SCOPETYPE_TEMPLATE 4

//Specified 'Null' Values
#define S_NULL " "
#define I_NULL -1

//Argument Types
#define ARGTYPE_STRING 0
#define ARGTYPE_NUMBER 1
#define ARGTYPE_VARIABLE 2
#define ARGTYPE_FUNCTION 3

//Function to check if a string contains another string
bool string_contains(string,string);

//Simple String Container Class
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

//Current User Name
#if (BUILD_OS == WINDOWS)
string USERNAME = getenv("USERNAME");
#elif (BUILD_OS == LINUX)
string USERNAME;
#endif

//Names of read and write files
string file_read_name = "";

#if (BUILD_OS == WINDOWS)
string file_write_name = "C:\\Users\\" + USERNAME + "\\AppData\\Roaming\\Boomslang\\source\\prenative.cpp";
#endif

//Platform to compile to
#if (BUILD_OS == WINDOWS)
int ve_apptype = APPTYPE_WINDOWS;
#elif (BUILD_OS == LINUX)
int ve_apptype = APPTYPE_LINUX;
#endif

//Does the program use the console?
bool console = false;

//Current Line Indentation
unsigned int indentation = 0;

//Containers to store names of actions, constants, keywords, and packages
Container ve_actions;
Container ve_constants;
Container ve_keywords;
Container ve_packages;

//Main Code Variable
string ve_main_code = "";

//Current Scope
string ve_scope = "BOOMSLANG";

//Current Context
string ve_context = S_NULL;

//Wait after compiled?
bool waitafter = false;

//Run after compile?
bool runafter = false;

//Local Path?
bool local_path = false;
char terminal_complete_path[255];
string terminal_path;

//Compiler and Linker Flags
string compile_flags = "";
string linker_flags = "";

//Boomslang Code
string compile_code = "";

//Input and Output files
ifstream file_read;
ofstream file_write;

//Pointer to Variable that functions write output code to
string* write_to;

//Number of Errors and Warnings
int errors = 0;
int warnings = 0;
