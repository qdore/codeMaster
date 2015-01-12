#ifndef _BUILTIN_PARSE_
#define _BUILTIN_PARSE_

#include <string>
#include <sstream>
#include "../include/lexical_cast.hpp"

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
        if (str.empty()) 
        {
            return false;
        }
        try {
            value = boost::lexical_cast<T>(str);
            return true;
        } catch (...) {
            return false;
        }
    }
};

}

#endif
