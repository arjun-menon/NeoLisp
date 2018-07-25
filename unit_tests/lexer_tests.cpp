#include <catch.hpp>
#include "../src/common.hpp"

#define T(TYPE) typeid(TYPE).hash_code()

bool checkLexerOutput(string input, const vector< pair<size_t, string> > &expectedOutput) {
    Lexer lexer;
    lexer.lex(input);

    // cout << "Testing Lexer Output: ";
    // lexer.display(cout, false);
    // cout << endl;

    size_t i = 0;
    for (auto &token : lexer.tokens) {
        const Value &value = *token;

        // cout << value << endl;
        if (T(value) != expectedOutput[i].first ||
            toString(value) != expectedOutput[i].second)
            return false;

        i++;
    }

    return true;
}

TEST_CASE("Lex basic expressions") {
    CHECK(checkLexerOutput("", {}));
    CHECK(checkLexerOutput("  \r\n  \t  \n  ", {}));

    CHECK(checkLexerOutput(
            "1+2",
            {
                    { T(Real), "1" },
                    { T(Symbol), "+" },
                    { T(Real), "2" }
            }
    ));

    CHECK(checkLexerOutput(
            "1 (2*'4') +5",
            {
                    { T(Real), "1" },
                    { T(Symbol), "(" },
                    { T(Real), "2" },
                    { T(Symbol), "*" },
                    { T(UserString), "'4'" },
                    { T(Symbol), ")" },
                    { T(Symbol), "+" },
                    { T(Real), "5" }
            }
    ));

    CHECK(checkLexerOutput(
            " 2  7 asdk (0xBEEF + 'hmm') 34.212aa 909",  // todo: '34.212aa' should be two tokens: '34.212' and 'aa'
            {
                    { T(Real), "2" },
                    { T(Real), "7" },
                    { T(Symbol), "asdk" },
                    { T(Symbol), "(" },
                    { T(Real), "48879" },
                    { T(Symbol), "+" },
                    { T(UserString), "'hmm'" },
                    { T(Symbol), ")" },
                    { T(Real), "34.212" },
                    { T(Real), "909" }
            }
    ));
}
