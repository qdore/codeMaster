#include <cstdlib>
#include <sstream>
#include <inttypes.h>

#include "builtin_parse.h"

using std::string;

namespace codemaster
{

BUILTINPARSE_TEMPLATE_SPECIALIZE(int)
{
    if(str.empty() ||
             (!isdigit(str[0]) &&
             (str[0] != '-') &&
             (str[0] != '+')))
    {
        return false;
    }
    char * p;
    strtol(str.c_str(), &p, 10);
    if (*p == 0)
    {
        try {
            value = std::stoi(str);
        } catch (...) {
            return false;
        }
        return true;
    }
    return false;
}

BUILTINPARSE_TEMPLATE_SPECIALIZE(std::string)
{
    if (str.empty()) return false;
    value = str;
    return true;
}

BUILTINPARSE_TEMPLATE_SPECIALIZE(long)
{
    char *rem = NULL;
    long tmp = strtoimax(str.c_str(), &rem, 0);
    if (*rem != '\0') return false;
    if ((tmp == INT_MIN || tmp == INT_MAX) && errno == ERANGE) return false;
    value = tmp;
    return true;
}

}
