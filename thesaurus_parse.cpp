#include <fstream>
#include "thesaurus_parse.h"
#include "builtin_parse.h"
#include "config.h"

using std::ifstream;
using std::string;

namespace codemaster
{

inline void print_warning(const std::string& message)
{
    std::cerr << "waring: [" << __FILE__ << ":" << __FUNCTION__\
        << "+" << __LINE__ << "]: " << message << std::endl;
}

inline void print_parse_succend_message(const std::string& message)
{
    
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

void ThesaurusParse::set_scheme(const string& scheme)
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
        if (index >= _scheme.size())
        {
            _scheme.push_back(ANY_PARSER_INDICATOR);
        }
        parse_word_by_parser(current_word, _scheme[index]);
        current_word.clear();
        ++index;
    }
}

void ThesaurusParse::parse_word_by_parser(const string& word, const string& parser) const noexcept
{
    if (parser == ANY_PARSER_INDICATOR)
    {
        for (auto beg = _parsers.begin(); beg != _parsers.end(); beg++)
        {
            auto word_parser = beg->second;
            auto parsered_item = word_parser->parse(word);
            if (parsered_item.get() != nullptr)
            {
                print_parse_succend_message(word_parser->get_catagery());
            }
        }
    }
    else
    {
        auto word_parser = _parsers.find(parser)->second;
        auto parsered_item = word_parser->parse(word);
        if (parsered_item.get() != nullptr)
        {
            print_parse_succend_message(word_parser->get_catagery());
        }
    }
}

}

