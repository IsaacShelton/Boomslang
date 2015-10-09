/**
    ============ dash.h ============
    Code for Handling Compiler Flags

    This file should NOT be included
    anywhere besides from boot.h.

    This Header does NOT contain a
    header guard so include with
    cation.
*/

using namespace std;

int i = 1;
string dash_argument;

while(i<arg_count){
    dash_argument = arg[i];

    if (dash_argument=="-help"){
        ///Show help

        cout << endl;
        cout << "'boomslang <filename>' - compile program" << endl;
        cout << "'boomslang -help' - help" << endl;

        return EXIT_SUCCESS;
    }
    else
        if (dash_argument=="-wait"){
        ///Show help

        waitafter = true;
    }
    else
    if (dash_argument=="-console"){
        console = true;
    }
    else
    if (dash_argument=="-windows"){
        ve_apptype = APPTYPE_WINDOWS;
    }
    else
    if (dash_argument=="-android"){
        ve_apptype = APPTYPE_ANDROID;
    }
    else
    if (dash_argument=="-linux"){
        ve_apptype = APPTYPE_LINUX;
    }
    else {
        file_read_name = dash_argument;
    }

    i++;
}
