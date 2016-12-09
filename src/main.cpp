#include "common.h"

void repl() {
    for (string input; true; cout << endl) {
        cout << "> ";

        if (!getline(std::cin, input))
            break;

        if (input == "exit" || input == "quit") {
            break;
        } else {
            try
            {
                tok_chain tokens = Tokenize(input.c_str());
                cout << display_toks(tokens, false);
                frag_chain frags = Fragmentize(tokens);
                cout << display_frags(frags, false);
            }
            catch(exception &e) {
                cout << e.what();
            }
            catch(...) {
                cout << "Unhandled Exception";
            }
        }
    }
}

void stuff() {
    cout<<"x = "<< sizeof(Real) <<endl;
    Table table;
    DataReader::read_file("test_data/t1.txt", table);
    cout << "table" << endl << table << endl;
    cout << table[3].getMin() << endl;
    table.display(cout, {1, 3});
    cout << endl << *(table[1] + table[3]) << endl;
}

int main() {
    repl();
    // stuff();
    return 0;
}
