#ifndef _ITEM_
#define _ITEM_

#include <string>
#include <vector>
#include <sstream>

#include "config.h"

namespace codemaster
{

class Item
{
public:
    Item() {}
    virtual ~Item() {}
public:
    virtual std::string to_string() const noexcept = 0;
    virtual std::string get_catagery() const noexcept = 0;
    virtual const void* get_value() const noexcept = 0;
private:
    DISALLOW_COPY_AND_ASSIGN(Item);
};

template <typename T>
class ParseredItem : public Item
{
public:
    ParseredItem(const std::string& catagery, const T& parsed_value) :
        _catagery(catagery),
        _val(parsed_value) {}
public:
    std::string to_string() const noexcept
    {
        std::ostringstream out;
        out << _val;
        return out.str();
    }
    std::string get_catagery() const noexcept
    {
        return _catagery;
    }
    const void* get_value() const noexcept
    {
        return &_val;
    }
private:
    std::string _catagery;
    T _val;
    DISALLOW_COPY_AND_ASSIGN(ParseredItem);
};

template <typename T>
class ParseredItem<std::vector<T> > : public Item
{
public:
    ParseredItem(const std::string& catagery, const std::vector<T>& parsed_value) :
        _catagery(catagery),
        _val(parsed_value) {}
public:
    std::string to_string() const noexcept
    {
        if (_val.empty()) return "{}";
        std::string ret("{");
        for (const auto& singel_item : _val)
        {
            std::ostringstream out;
            out << singel_item;
            ret += out.str();
            ret += ", ";
        }
        ret.resize(ret.size() - 2);
        ret += "}";
        return ret;
    }
    std::string get_catagery() const noexcept
    {
        return _catagery;
    }
    const void* get_value() const noexcept
    {
        return &_val;
    }
private:
    std::string _catagery;
    std::vector<T> _val;
    DISALLOW_COPY_AND_ASSIGN(ParseredItem);
};

}

#endif
