
using namespace std;

int i = 1;
string dash_argument;

while(i < arg_count){
    dash_argument = arg[i];

    if (dash_argument=="-help"){
        ///Show help

        cout << endl;
        cout << "boomslang <filename> [options]" << endl << endl;
        cout << " -help     | help" << endl;
        cout << " -console  | uses the console" << endl;
        cout << " -wait     | wait after complete" << endl;
        cout << " -run      | run after compiled" << endl;
        cout << " -windows  | compile for windows" << endl;
        cout << " -linux    | compile for linux" << endl;
        cout << " -android  | compile for android" << endl;

        return EXIT_FAILURE;
    }
    else if (dash_argument=="-version"){
        ///Show version

        cout << endl;
        cout << "Boomslang Compiler " << BUILD_MAJOR << "." << BUILD_MINOR  << "." << BUILD_RELEASE << endl;
        cout << "open-source standard boomslang compiler" << endl;
        cout << "view the source at github.com/IsaacShelton/Boomslang" << endl;

        return EXIT_FAILURE;
    }
    else if (dash_argument=="-wait"){
        //Wait after compile is finished

        waitafter = true;
    }
    else if (dash_argument=="-branchtest"){
        ofstream branch_write("C:\\Users\\isaac\\Desktop\\o.branch", ios::binary);

        if(!branch_write){
            error_fatal("Failed to make branch");
            pend();
            return EXIT_FAILURE;
        }

        string value = "branch#ifndef BRANCH_O\n#define BRANCH_O\nvoid boomslang_say(boomslang_String);\n#endif\x03\nregister method say()->none\x03\n";

        char* buffer = new char[value.length() + 1];
        strcpy(buffer, value.c_str());
        unsigned int size_of_buffer = value.length() + 1;

        branch_write.write(buffer,size_of_buffer);
        delete[] buffer;

        {
            ifstream library_file("C:/Users/isaac/Desktop/lib.a", ios::binary);

            if(!library_file){
                error_fatal("Failed to open library");
                pend();
                return EXIT_FAILURE;
            }

            while(!library_file.eof()){
                char byte_data;
                library_file.read(&byte_data,1);

                if(!library_file.eof()) branch_write.write(&byte_data,1);
            }

            library_file.close();
        }

        branch_write.close();

        branch_load("C:/Users/isaac/Desktop/o.branch");
        return EXIT_FAILURE;
    }
    else if (dash_argument=="-nativebranch"){
        string output_filename = "";
        string header_filename = "";
        string boomslang_filename = "";
        string lib_filename = "";

        cout << "Enter output filename" << endl;
        cin >> output_filename;

        cout << "Enter header filename" << endl;
        cin >> header_filename;

        cout << "Enter boomslang filename" << endl;
        cin >> boomslang_filename;

        cout << "Enter library filename" << endl;
        cin >> lib_filename;

        if(branch_create(output_filename,lib_filename,header_filename,boomslang_filename)==EXIT_FAILURE) return EXIT_FAILURE;
        return EXIT_SUCCESS;
    }
    else if (dash_argument=="-run"){
        //Run after compile is finished

        runafter = true;
    }
    else if (dash_argument=="-console"){
        console = true;
    }
    else if (dash_argument=="-branch"){
        package = true;
    }
    else if (dash_argument=="-windows"){
        ve_apptype = APPTYPE_WINDOWS;
    }
    else if (dash_argument=="-android"){
        ve_apptype = APPTYPE_ANDROID;
    }
    else if (dash_argument=="-linux"){
        ve_apptype = APPTYPE_LINUX;
    }
    else {
        file_read_name = dash_argument;
    }

    i++;
}
