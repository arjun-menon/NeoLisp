#include "common.hpp"

list< deque<string> > parse(deque<string> &tokens)
{
    list< deque<string> > expressions;

    while(!tokens.empty())
    {
        deque<string> some_toks;

        // fetches, pops & pushes the front-most token in 'tok_chain':
        string s = tokens.front();
        tokens.pop_front();
        some_toks.push_back(s);

        if(s=="(")
        {
            // balance is the number of yet un-closed parentheses
            int balance = 1;

            while(!tokens.empty())
            {
                // fetches, pops & pushes the front-most token in 'tok_chain': (repeated code)
                s = tokens.front();
                tokens.pop_front();
                some_toks.push_back(s);

                if(s=="(")
                    balance++;
                else if(s==")")
                    balance--;

                if(!balance)
                    break;
            }

            // balance = 0 means the expression is balanced
            if(balance)
                throw SyntaxError("Missing closing parenthesis.");
        }
        else if(s==")")
            throw SyntaxError("Missing opening parenthesis.");

        // push expression
        if(!some_toks.empty())
            expressions.push_back(some_toks);
    }

    return expressions;
}

string display_expressions(const list<deque<string> > &expressions, bool linear)
{
    stringstream sout;
    int counter = 0;

    if(!linear)
        sout<<endl<<expressions.size()<<" expressions:"<<endl;

    for(list< deque<string> >::const_iterator i = expressions.begin(); i!=expressions.end(); i++)
    {
        if(!linear)
            sout<<++counter<<": "<<endl;

        for(deque<string>::const_iterator j = i->begin(); j!=i->end(); j++)
        {
            if(linear)
                sout<<*j;
            else
                sout<<"\t"<<*j<<endl;
        }
        if(!linear)
            sout<<endl;
    }
    return sout.str();
}
