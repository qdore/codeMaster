#include <fstream>
#include <iomanip>

#include "thesaurus_parse.h"
#include "builtin_parse.h"
#include "config.h"

using std::ifstream;
using std::string;

namespace codemaster
{

static inline void print_warning(const std::string& message)
{
    std::cerr << "waring: [" << __FILE__ << ":" << __FUNCTION__\
        << "+" << __LINE__ << "]: " << message << std::endl;
}

static inline void print_parse_succend_message(const std::shared_ptr<Item> message)
{
    std::cout << "It may be "<< std::setw(10) << message->get_catagery()
       << '\t' << "=>" << '\t' << message->to_string() << std::endl;
}

static inline void print_get_word(const std::string& word,
        const std::string& scheme)
{
    std::cout << "Get word (" << word << ")\t" << "Scheme: " << scheme 
        << std::endl << "====================" << std::endl;
}

ThesaurusParse::ThesaurusParse() noexcept
{
    add_parser(make_parser("int", BuiltinParse<int>()));
    add_parser(make_parser("float", BuiltinParse<float>()));
    add_parser(make_parser("string", BuiltinParse<string>()));
    add_parser(make_array_parser("int[]", BuiltinParse<int>()));
    add_parser(make_array_parser("float[]", BuiltinParse<float>()));
    add_parser(make_array_parser("string[]", BuiltinParse<string>()));
}

void ThesaurusParse::read(const string& file)
{
    ifstream * file_in = new ifstream(file);
    in.reset(file_in);
}

void ThesaurusParse::read(std::istream& input)
{
    in.reset(&input, [](...){});
}

void ThesaurusParse::add_parser(const std::shared_ptr<Parser>& parser) noexcept
{
    if (parser->get_catagery() == ANY_PARSER_INDICATOR) {
        string message("indicator cannot be ");
        message += ANY_PARSER_INDICATOR;
        print_warning(message);
        return;
    }
    _parsers.insert({parser->get_catagery(), parser});
}

void ThesaurusParse::set_scheme(const string& scheme) noexcept
{
    std::istringstream scheme_in(scheme);
    string word_scheme;
    while (scheme_in)
    {
        scheme_in >> word_scheme;
        if (word_scheme.empty()) continue;
        if (word_scheme == ANY_PARSER_INDICATOR) 
        {
            _scheme.push_back(std::move(word_scheme));
            continue;
        }
        if (_parsers.find(word_scheme) == _parsers.end())
        {
            string message("cannot find parser: ");
            message += word_scheme;
            print_warning(message);
            _scheme.push_back(ANY_PARSER_INDICATOR);
            word_scheme.clear();
        }
        else
        {
            _scheme.push_back(std::move(word_scheme));
        }
    }
}

bool ThesaurusParse::has_next_row() const noexcept
{
    if (in.get() == nullptr || !(*in))
    {
        return false;
    }
    return true;
}

void ThesaurusParse::parse_row()
{
    string current_row;
    string current_word;
    std::getline(*in, current_row, THESAURUS_ROW_DELIM);
    if (current_row.empty()) return;
    std::istringstream string_in(current_row);
    size_t index = 0;
    while (string_in)
    {
        std::getline(string_in, current_word, THESAURUS_COLUMN_DELIM);
        trim(current_word);
        if (index >= _scheme.size())
        {
            _scheme.push_back(ANY_PARSER_INDICATOR);
        }
        if (current_word.empty())
        {
            if (string_in)
            {
                print_warning("get null word!");
                index++;
            }
            continue;
        }
        print_get_word(current_word, _scheme[index]);
        parse_word_by_parser(current_word, _scheme[index]);
        current_word.clear();
        ++index;
    }
}

void ThesaurusParse::parse_word_by_parser(const string& word,
        const string& parser) const noexcept
{
    if (parser == ANY_PARSER_INDICATOR)
    {
        for (auto beg = _parsers.begin(); beg != _parsers.end(); beg++)
        {
            auto word_parser = beg->second;
            auto parsered_item = word_parser->parse(word);
            if (parsered_item.get() != nullptr)
            {
                print_parse_succend_message(parsered_item);
            }
        }
    }
    else
    {
        auto word_parser = _parsers.find(parser)->second;
        auto parsered_item = word_parser->parse(word);
        if (parsered_item.get() != nullptr)
        {
            print_parse_succend_message(parsered_item);
        }
        else
        {
            std::string warning_message("It cannot be parsed by ");
            warning_message += word_parser->get_catagery();
            print_warning(warning_message);
        }
    }
}

std::string ThesaurusParse::get_scheme() const noexcept
{
    string ret;
    for (const auto& str : _scheme)
    {
        ret += str;
        ret += " ";
    }
    return ret;
}

}
