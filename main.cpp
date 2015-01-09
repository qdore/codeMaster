#include "thesaurus_parse.h"
#include "example/example.h"

using std::cin;
using std::cout;
using std::endl;
using std::cerr;

inline void parse(codemaster::ThesaurusParse& thesaurus)
{
    while (thesaurus.has_next_row())
    {
        thesaurus.parse_row();
    }
}

int main(int argc, char ** argv)
try
{
    codemaster::ThesaurusParse thesaurus;
    //add new parser: example
    thesaurus.add_parser(codemaster::make_parser("example", ext::Example::parse));
    //set scheme
    thesaurus.set_scheme("* example int string");
    cout << "Seted scheme: \" " << thesaurus.get_scheme() << "\"" << endl;
    if (argc < 2)
    {
        thesaurus.read(cin);
        parse(thesaurus);
    }
    else
    {
        for (int i = 1; i != argc; i++)
        {
            thesaurus.read(argv[i]);
            parse(thesaurus);
        }
    }
} catch (std::exception& ex) {
    cerr << "catch exception: " << ex.what() << endl;
} catch (...) {
    cerr << "catch unknown exception." << endl;
}
