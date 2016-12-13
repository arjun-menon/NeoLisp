#include <catch.hpp>
#include "../src/common.hpp"

bool checkLexerOutput(string input, const vector< pair<size_t, string> > &expectedOutput) {
    Lexer lexer;
    lexer.lex(input);

    cout<<"Testing Lexer Output: ";
    lexer.display(cout, false);
    cout<<endl;

    size_t i = 0;
    for(auto &token : lexer.tokens) {
        const Value &value = *token;

        if (typeid(value).hash_code() != expectedOutput[i].first ||
            toString(value) != expectedOutput[i].second)
            return false;

        i++;
    }

    return true;
}

#define T(TYPE) typeid(TYPE).hash_code()

TEST_CASE("Basic expressions") {
    CHECK(checkLexerOutput(
            "1+2",
            {
                    { T(Real), "1" },
                    { T(Symbol), "+" },
                    { T(Real), "2" }
            }
    ));

    CHECK(checkLexerOutput(
            "1 (2*4) +5",
            {
                    { T(Real), "1" },
                    { T(ExprStart), "(" },
                    { T(Real), "2" },
                    { T(Symbol), "*" },
                    { T(Real), "4" },
                    { T(ExprEnd), ")" },
                    { T(Symbol), "+" },
                    { T(Real), "5" }
            }
    ));

    CHECK(checkLexerOutput(
            " 2  7 asdk ( + ) 34.212aa 909", // todo: '34.212aa' should be two tokens: '34.212' and 'aa'
            {
                    { T(Real), "2" },
                    { T(Real), "7" },
                    { T(Symbol), "asdk" },
                    { T(ExprStart), "(" },
                    { T(Symbol), "+" },
                    { T(ExprEnd), ")" },
                    { T(Real), "34.212" },
                    { T(Real), "909" }
            }
    ));
}
