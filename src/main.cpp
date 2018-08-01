#include "common.hpp"

int repl() {
    Env env;

    for (string input; true; cout << endl) {
        cout << "> ";

        if (!getline(std::cin, input))
            break;

        if (input == "exit" || input == "quit") {
            break;
        } else {
            try {
                shared_ptr<Value> ast = parse(input);
                cout << *eval(ast, env) << endl;
            }
            catch(ExitNow &e) {
                return e.exit_code;
            }
            catch(exception &e) {
                cout << e.what() << endl;
            }
            catch(...) {
                cout << "Error: Unhandled Exception" << endl;
            }
        }
    }

    return 0;
}

void stuff() {
    cout<<"x = "<< sizeof(Real) <<endl;
    Matrix table;
    CsvReader::read_file("test_data/t1.txt", table);
    cout << "table" << endl << table << endl;
    cout << table[3].getMin() << endl;
    table.display(cout, {1, 3});
    cout << endl << *(table[1] + table[3]) << endl;
}

int main() {
    // stuff();
    return repl();
}
