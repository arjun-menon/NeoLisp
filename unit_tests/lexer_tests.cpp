#include <catch.hpp>
#include "../src/common.hpp"

#define T(TYPE) typeid(TYPE).hash_code()

bool checkLexerOutput(string input, const vector< pair<size_t, string> > &expectedOutput) {
    Lexer lexer;
    lexer.lex(input);

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

TEST_CASE("Basic expressions") {
    CHECK(checkLexerOutput("1+2", {
            { typeid(Real).hash_code(), "1" },
            { typeid(Symbol).hash_code(), "+" },
            { typeid(Real).hash_code(), "2" }
    }));
}
