#ifndef _EXAMPLE_
#define _EXAMPLE_

#include <string>
#include <iostream>

namespace ext
{

struct Example
{
    char a;
    char b;

    static bool parse(const std::string& str, Example& ext);
    friend std::ostream& operator<<(std::ostream&, const Example& ext);
};

}
#endif
