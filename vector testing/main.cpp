#include <iostream>
#include <vector>

using namespace std;

vector<int> global_ints;

void elem_test(vector<int>& int_vector, int& an_int){
    cout << "a size: " << int_vector.size() << endl << "b value: " << an_int << endl << endl;

    int_vector.pop_back();
    an_int = 6;
    cout << an_int << " " << global_ints[1] << endl;

    cout << "a size: " << int_vector.size() << endl << "b value: " << an_int << endl << endl;
}

int main()
{
    global_ints.push_back(10);
    global_ints.push_back(22);

    elem_test(global_ints,global_ints[1]);

    cout << "global_ints size: " << global_ints.size() << endl;
    cout << "global_ints[0] value: " << global_ints[0] << endl;
    cout << "global_ints[1] value: " << global_ints[1] << endl;
    return 0;
}
