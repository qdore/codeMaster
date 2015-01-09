#ifndef _WORD_PARSER_
#define _WORD_PARSER_

#include <string>
#include <sstream>
#include <memory>

#include "builtin_parse.h"
#include "parsed_item.h"
#include "config.h"

namespace codemaster
{

// trim space from start 
static inline void ltrim(std::string &s)
{
    s.erase(s.begin(),
            std::find_if(s.begin(), s.end(),
            std::not1(std::ptr_fun<int, int>(std::isspace)))); 
} 

// trim space from end 
static inline void rtrim(std::string &s)
{ 
    s.erase(std::find_if(s.rbegin(), s.rend(),
                std::not1(std::ptr_fun<int, int>(std::isspace))).base(), s.end()); 
} 

static inline void trim(std::string &s)
{ 
    ltrim(s); 
    rtrim(s);
} 

template <typename T> struct parser_traits;
template <typename T>
struct parser_traits<bool(*)(const char*, T&)> {
    typedef T value_type;
};

template <typename T>
struct parser_traits<bool(*)(const std::string&, T&)> {
    typedef T value_type;
};

template <typename T>
struct parser_traits<BuiltinParse<T> > {
    typedef T value_type;
};

class Parser
{
public:
    Parser() {}
    virtual ~Parser() {};
public:
    virtual std::string get_catagery() const noexcept = 0;
    virtual std::shared_ptr<Item> parse(const std::string& word) noexcept = 0;
private:
    DISALLOW_COPY_AND_ASSIGN(Parser);
};

template <typename function>
class WordParser : public Parser
{
public:
    typedef typename parser_traits<function>::value_type value_type;
    WordParser(const std::string& catagery,
            const function& parse) :
        _catagery(catagery),
        _parse(parse) {}
public:
    std::string get_catagery() const noexcept
    {
        return this->_catagery;
    }
    std::shared_ptr<Item> parse(const std::string& word) noexcept
    {
        value_type parsed_value;
        std::string word_trim(word);
        trim(word_trim);
        if (_parse(word_trim, parsed_value))
        {
            return std::shared_ptr<Item>(
                    new ParseredItem<value_type>(
                        this->_catagery, parsed_value
                        )
                    );
        }
        else
        {
            return std::shared_ptr<Item>();
        }
    }
private:
    function _parse;
    std::string _catagery;
    DISALLOW_COPY_AND_ASSIGN(WordParser);
};

template <typename function>
class WordParserToArray : public Parser
{
public:
    typedef typename parser_traits<function>::value_type value_type;
    WordParserToArray(const std::string& catagery,
            const function& parse) :
        _catagery(catagery),
        _parse(parse) {}
public:
    std::shared_ptr<Item> parse(const std::string& word) noexcept
    {
        try {
            std::vector<value_type> parsed_value;
            size_t colon_position = word.find_first_of(ARRAY_INDICATE);
            if (colon_position != std::string::npos && colon_position != 0)
            {
                int vec_size;
                if (!BuiltinParse<int>()(word.substr(0, colon_position), vec_size))
                {
                    throw -1;
                }
                std::istringstream in(word.substr(colon_position + 1, word.size()));
                std::string item;
                while (std::getline(in, item, ARRAY_DELIM))
                {
                    value_type parsed_single_value;
                    trim(item);
                    if (_parse(item, parsed_single_value))
                    {
                        parsed_value.push_back(std::move(parsed_single_value));
                    }
                    else
                    {
                        throw -1;
                    }
                }
                if (parsed_value.size() != vec_size)
                {
                    throw -1;
                }
                return std::shared_ptr<Item>(
                        new ParseredItem<std::vector<value_type> >(
                            this->_catagery, parsed_value
                            )
                        );
            }
            else
            {
                throw -1;
            }
        }
        catch (...) {
            return std::shared_ptr<Item>();
        }
    }
    std::string get_catagery() const noexcept
    {
        return this->_catagery;
    }
private:
    function _parse;
    std::string _catagery;
    DISALLOW_COPY_AND_ASSIGN(WordParserToArray);
};

template <typename function>
std::shared_ptr<Parser> make_parser(const std::string& catagery,
        function parse)
{
    return std::shared_ptr<Parser>(new WordParser<function>(catagery, parse));
}

template <typename function>
std::shared_ptr<Parser> make_array_parser(const std::string& catagery,
        function parse)
{
    return std::shared_ptr<Parser>(
            new WordParserToArray<function>(catagery, parse));
}

}

#endif
