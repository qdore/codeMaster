#include "builtin_parse.h"

using std::string;

namespace codemaster
{

template <>
bool BuiltinParse<int>::operator()(const string& str, int& value) noexcept
{
    try {
        value = std::stoi(str);
        return true;
    }
    catch (...) {
        return false;
    }
}

template <>
bool BuiltinParse<float>::operator()(const string& str, float& value) noexcept
{
    try {
        value = std::stof(str);
        return true;
    }
    catch (...) {
        return false;
    }
}

template <>
bool BuiltinParse<double>::operator()(const string& str, double& value) noexcept
{
    try {
        value = std::stod(str);
        return true;
    }
    catch (...) {
        return false;
    }
}


template <>
bool BuiltinParse<string>::operator()(const string& str, string& value) noexcept
{
    value = str;
    return true;
}

}
