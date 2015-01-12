#include <sstream>
#include "name.h"

namespace example
{

bool Name::parse(const std::string& str, Name& name)
{
    std::istringstream in(str);
    if (in >> name.first_name >> name.second_name)
    {
        return true;
    }
    return false;
}

std::ostream& operator<<(std::ostream& out, const Name& name)
{
    return out << "First name:" << name.first_name
        << "\tSecond name:" << name.second_name;
}

}
