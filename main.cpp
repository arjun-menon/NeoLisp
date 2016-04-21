
#include <iostream>
#include <fstream>
#include <stdexcept>

using namespace std;


void load_data_from_file(string filename)
{
    ifstream csv_file(filename);

    if(csv_file.fail()) {
        throw runtime_error(string("File \"") + filename + "\" does not exist.");
    }

    string line;
    while(getline(csv_file, line)) {
        cout << line << endl;
    }
}

int main() {
    load_data_from_file("test_data/t1.txt");

    return 0;
}
