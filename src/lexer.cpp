#include "common.hpp"

inline static bool isBindingSymbol(char c) {
    return ( c=='(' || c==')' );
}

inline static bool isIdChar(char c) {
    return isalnum(c) || c == '_';
}

inline static bool nonAlnum(char c) {
    return !isalnum(c);
}

static void trim(string &str, const string to_remove = " \t") {
    // trim leading & trailing 'to_remove'
    size_t startpos = str.find_first_not_of(to_remove);
    size_t endpos = str.find_last_not_of(to_remove);

    // if empty or all 'to_remove' return an empty string otherwise trim
    str = (( string::npos == startpos ) || ( string::npos == endpos)) ? "" :
          str.substr( startpos, endpos-startpos+1 );
}

void Lexer::constructValue() {
    if (token == "(") {
        tokens.emplace_back(new ExprStart());
    }
    else if(token == ")") {
        tokens.emplace_back(new ExprEnd());
    }
    else if(*token.begin() == '\"') {
        tokens.emplace_back(new UserString(token.substr(1, token.length() - 2)));
    }
    else if(isdigit(*token.begin())) {
        tokens.emplace_back(new Real(token));
    }
    else {
        tokens.emplace_back(new Symbol(token));
    }
}

void Lexer::addToken() {
    trim(token);

    if(token.length()) {
        constructValue();
    }

    token = "";
}

const TokenQueue& Lexer::lex(const char *sp) {
    const char quoteChar = '\"';
    const char escapeChar = '\\';

    bool inString = false;

    for(char currentChar = *sp, prevChar = '\0', prevChar2 = '\0',
                nextChar = '\0' ; currentChar ; currentChar = *++sp )
    {
        /*
        Set nextChar
        remember, if *sp = '\0' then we would never entered
        this loop, so dereferencing (sp+1) is okay.
        similarly, in the last iteration nextChar will be equal to '\0' and
        there is no further iteration, so we will never read beyond sp.
        */
        nextChar = *(sp+1);

        if(!iscntrl(currentChar))
        {
            if(inString)
            {
                if((currentChar==quoteChar && prevChar==escapeChar && prevChar2==escapeChar) ||  // e.g. (1, "\\", 3)
                   (currentChar==quoteChar && prevChar!=escapeChar))  // the normal case, e.g. (1, "abc", 3)
                {
                    // string literal termination
                    inString = false;
                    token += currentChar;
                    addToken();
                }
                else
                    token += currentChar;
            }
            else
            {
                if(currentChar=='\"')
                {
                    addToken();
                    inString = true;
                    token += currentChar;
                }
                else if(isspace(currentChar))
                {
                    addToken();
                }
                else if(isBindingSymbol(currentChar))
                {
                    addToken();
                    token = currentChar;
                    addToken();
                }
                else
                {
                    if(!
                            // The following conditions denote characters that may appear together in
                            // a token. If the condition fails, the token in hand so-far is pushed.
                            (
                                    // contiguous alphanumeric characters (abc, ab5d, 5e6) and
                                    // contiguous non-alphanumeric characters (%, &&, +++, ==, -, ...)
                                    ( isIdChar(prevChar) && isIdChar(currentChar) ) ||
                                    ( nonAlnum(prevChar) && nonAlnum(currentChar) ) ||
                                    // this test is for real number literals (example: 2.34)
                                    (isdigit(prevChar) && currentChar=='.' && isdigit(nextChar)) ||
                                    (isdigit(prevChar2) && prevChar=='.' && isdigit(currentChar))
                            )
                            )
                        addToken();

                    token += currentChar;
                }
            }

            prevChar2 = prevChar;
            prevChar = currentChar;
        }

        else
            throw SyntaxError("Non-printable ASCII character(" + toString(static_cast<unsigned char>(currentChar))
                            + ":" + toString(static_cast<unsigned int>(currentChar)) + ") detected.");
    }
    addToken();

    if(inString)
        throw SyntaxError("Unterminated string");

    return tokens;
}

ostream& Lexer::display(const deque< unique_ptr<Value> > &tokens, ostream &o, bool pretty_print) {
    if(pretty_print)
        o << "There are " << tokens.size() << " tokens:" << endl;

    for(auto &token : tokens)
    {
        if(pretty_print) {
            const Value &value = *token;
            o << typeid(value).name() << ": ";
        }

        o<<*token;

        if(pretty_print)
            o<<endl;
    }

    return o;
}

ostream& operator<<(ostream& stream, const TokenQueue &tokens) {
    return Lexer::display(tokens, stream, true);
}
