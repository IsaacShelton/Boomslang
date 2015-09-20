
using namespace std;

string packages;
ifstream preprocess_file_read;
preprocess_file_read.open( ("C:\\Users\\" + USERNAME + "\\AppData\\Roaming\\Boomslang\\source\\prenative.cpp").c_str() );
ofstream preprocess_file_write;
preprocess_file_write.open( ("C:\\Users\\" + USERNAME + "\\AppData\\Roaming\\Boomslang\\source\\native.cpp").c_str() );

if (!preprocess_file_read.is_open() and !preprocess_file_write.is_open()){
    error_fatal("Couldn't open files");
    pend();
    return EXIT_FAILURE;
}

packages = string_delete_amount(ve_packages.data,1);

while(packages!=""){
    preprocess_file_write << "#define USES_" + string_upper(string_replace_all(string_get_until(packages,";") + "\n",".","_"));
    packages = string_delete_until(packages,";");
    packages = string_delete_amount(packages,1);
}

string read;

getline(preprocess_file_read,read);
preprocess_file_write << read + "\n";

while(getline(preprocess_file_read,read)){
    preprocess_file_write << read + "\n";
}

preprocess_file_read.close();
preprocess_file_write.close();
