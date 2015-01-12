#ifndef _EXAMPLE_
#define _EXAMPLE_

#include <string>
#include <iostream>

namespace example
{

struct Name
{
    std::string first_name;
    std::string second_name;

    static bool parse(const std::string& str, Name& name);
    friend std::ostream& operator<<(std::ostream&, const Name& name);
};

}
#endif
