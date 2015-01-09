#include "example.h"

namespace ext
{

bool Example::parse(const std::string& str, Example& ext)
{
    if (str.size() != 2) return false;
    ext.a = str[0];
    ext.b = str[1];
    return true;
}

std::ostream& operator<<(std::ostream& out, const Example& ext)
{
    return out << "A:" << ext.a << " and " << "B:" << ext.b;
}

}
