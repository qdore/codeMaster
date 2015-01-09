#ifndef _BUILTIN_PARSE_
#define _BUILTIN_PARSE_

#include <string>

namespace codemaster
{

template <typename T>
class BuiltinParse
{
public:
    virtual ~BuiltinParse() {}
public:
    bool operator()(const std::string&, T&) noexcept;
};

}

#endif
