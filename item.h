#ifndef _ITEM_
#define _ITEM_

#include <string>
#include <vector>

namespace codemaster
{

template <typename T>
std::string to_string(const T& val)
{
    return std::to_string(val);
}

template <>
std::string to_string(const std::string& val)
{
    return val;
}

class Item
{
public:
    Item() {}
    virtual ~Item() {}
public:
    virtual std::string to_string() const noexcept = 0;
    virtual std::string get_catagery() const noexcept = 0;
    virtual const void* get_value() const noexcept = 0;
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
        return codemaster::to_string(_val);
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
            ret += codemaster::to_string(singel_item);
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
};

}

#endif
