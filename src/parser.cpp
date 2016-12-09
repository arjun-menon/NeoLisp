#include "common.h"

frag_chain Fragmentize(tok_chain &toks)
{
    frag_chain frags;

    while(!toks.empty())
    {
        tok_chain some_toks;

        // fetches, pops & pushes the front-most token in 'tok_chain':
        string s = toks.front();
        toks.pop_front();
        some_toks.push_back(s);

        if(s=="(")
        {
            // balance is the number of yet un-closed parentheses
            int balance = 1;

            while(!toks.empty())
            {
                // fetches, pops & pushes the front-most token in 'tok_chain': (repeated code)
                s = toks.front();
                toks.pop_front();
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

        // push fragment
        if(!some_toks.empty())
            frags.push_back(some_toks);
    }

    return frags;
}

string display_frags(const frag_chain &c, bool linear)
{
    stringstream sout;
    int counter = 0;

    if(!linear)
        sout<<endl<<c.size()<<" fragments:"<<endl;

    for(frag_chain::const_iterator i = c.begin(); i!=c.end(); i++)
    {
        if(!linear)
            sout<<++counter<<": "<<endl;

        for(tok_chain::const_iterator j = i->begin(); j!=i->end(); j++)
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
