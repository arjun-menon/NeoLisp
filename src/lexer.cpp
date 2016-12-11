#include "common.h"

void static trim(string &str, const string to_remove = " \t")
{
    // trim leading & trailing 'to_remove'
    size_t startpos = str.find_first_not_of(to_remove);
    size_t endpos = str.find_last_not_of(to_remove);

    // if empty or all 'to_remove' return an empty string otherwise trim
    str = (( string::npos == startpos ) || ( string::npos == endpos)) ? "" :
          str.substr( startpos, endpos-startpos+1 );
}

inline static void push_token(string &s, deque<string> &tokens)
{
    trim(s);
    if(s.length())
        tokens.push_back(s);
    s = "";
}

inline static bool isBindingSymbol(char c) {
    return ( c=='(' || c==')' );
}

inline static bool isIdChar(char c) {
    return isalnum(c) || c == '_';
}

inline static bool nonAlnum(char c) {
    return !isalnum(c);
}

deque<string> lex(const char *sp)
{
    const char quoteChar = '\"';
    const char escapeChar = '\\';
    // UNDERSCORES (_) IN VAR. NAMES !!!

    deque<string> tokens;
    string temp;
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
                    temp += currentChar;
                    push_token(temp, tokens);
                }
                else
                    temp += currentChar;
            }
            else
            {
                if(currentChar=='\"')
                {
                    push_token(temp, tokens);
                    inString = true;
                    temp += currentChar;
                }
                else if(isspace(currentChar))
                {
                    push_token(temp, tokens);
                }
                else if(isBindingSymbol(currentChar))
                {
                    push_token(temp, tokens);
                    temp = currentChar;
                    push_token(temp, tokens);
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
                        push_token(temp, tokens);

                    temp += currentChar;
                }
            }

            prevChar2 = prevChar;
            prevChar = currentChar;
        }

        else
            throw SyntaxError("Non-printable ASCII character(" + toString((UC)(currentChar))
                            + ":" + toString((UI)((UC)(currentChar))) + ") detected.");
    }
    push_token(temp, tokens);

    if(inString)
        throw SyntaxError("Unterminated string");

    return tokens;
}

string display_toks(const deque<string> &tokens, bool linear)
{
    stringstream sout;
    if(!linear)
        sout<<endl<<tokens.size()<<" tokens:"<<endl;
    for(deque<string>::const_iterator i = tokens.begin(); i!=tokens.end(); i++)
    {
        sout<<*i;

        if(!linear)
            sout<<endl;
    }
    return sout.str();
}
/*
bool isValid_id(const char *str)
{
    // checks the syntactical validity
    // of a proposed identifier.

    if(*str>='0'&&*str<='9')
        return false;

    for(;*str!='\0';str++)
        if((*str<'A'||*str>'Z')
           && (*str<'a'||*str>'z')
           && (*str<'0'||*str>'9')
           && (*str!='_'))
            return false;

    return true;
}

bool isValidIdentifier(string s)
{
    return isValid_id(s.c_str());
}

inline bool is_separator(char k) {
    return isspace(k) || k == ',';
}

void lex_try1(const char *sp) {
    while(*sp) {
        // get to the first non-separator char
        for (; *sp; sp++)
            if (!is_separator(*sp))
                break;

        const char *start = sp;

        // stop at the next separator, digit, or quote
        for (; *sp; sp++) {
            if (is_separator(*sp) || isdigit(*sp))
                break;
        }

        if (sp > start) {
            string k(start, sp - start);
            cout << '\"' << k << '\"' << endl;
        }
    }
}
*/
