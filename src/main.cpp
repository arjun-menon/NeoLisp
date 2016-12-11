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
                Lexer lexer;
                lexer.lex(input.c_str());
                lexer.display();

                list< deque<string> > expressions = parse(lexer.tokens);
                //cout << display_expressions(expressions, false);
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
