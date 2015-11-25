
using namespace std;

//String of packages
string packages;

//New Input File
ifstream preprocess_file_read;

#if (BUILD_OS == WINDOWS)
preprocess_file_read.open( ("C:\\Users\\" + USERNAME + "\\AppData\\Roaming\\Boomslang\\source\\prenative.cpp").c_str() );
#elif (BUILD_OS == LINUX)
preprocess_file_read.open( ("/home/" + USERNAME + "/Boomslang/source/prenative.cpp").c_str() );
#endif

//New Output File
ofstream preprocess_file_write;

#if (BUILD_OS == WINDOWS)
preprocess_file_write.open( ("C:\\Users\\" + USERNAME + "\\AppData\\Roaming\\Boomslang\\source\\native.cpp").c_str() );
#elif (BUILD_OS == LINUX)
preprocess_file_write.open( ("/home/" + USERNAME + "/Boomslang/source/native.cpp").c_str() );
#endif

//Ensure they're open
if (!preprocess_file_read.is_open() and !preprocess_file_write.is_open()){
    error_fatal("Couldn't open files");
    pend();
    return EXIT_FAILURE;
}

packages = string_delete_amount(ve_packages.data,1);

//Define USES_PACKAGE for every package
while(packages!=""){
    preprocess_file_write << "#define USES_" + string_upper(string_replace_all(string_get_until(packages,";") + "\n",".","_"));
    packages = string_delete_until(packages,";");
    packages = string_delete_amount(packages,1);
}

//Variable to hold output code
string read;

//Move code to new file
getline(preprocess_file_read,read);
preprocess_file_write << read + "\n";
while(getline(preprocess_file_read,read)){
    preprocess_file_write << read + "\n";
}

//Close Files
preprocess_file_read.close();
preprocess_file_write.close();
