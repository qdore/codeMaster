#ifndef _BUILTIN_PARSE_
#define _BUILTIN_PARSE_

#include <string>
#include <sstream>

#define BUILTINPARSE_TEMPLATE_SPECIALIZE(type) template <>\
    bool BuiltinParse<type>::operator()(const std::string& str,\
                                        type& value) noexcept

namespace codemaster
{

template <typename T>
class BuiltinParse
{
public:
    BuiltinParse() {}
    virtual ~BuiltinParse() {}
public:
    bool operator()(const std::string& str, T& value) noexcept
    {
        if (str.empty()) return false;
        std::istringstream in(str);
        try {
            in >> value;
            if (in.eof())
            {
                return true;
            }
            return false;
        }
        catch (...) {
            return false;
        }
    }
};

BUILTINPARSE_TEMPLATE_SPECIALIZE(int);
BUILTINPARSE_TEMPLATE_SPECIALIZE(std::string);
BUILTINPARSE_TEMPLATE_SPECIALIZE(long);

}

#endif
