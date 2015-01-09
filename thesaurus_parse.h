#ifndef _THESAURUS_PARSE_
#define _THESAURUS_PARSE_

#include <string>
#include <iostream>
#include <sstream>
#include <fstream>
#include <unordered_map>
#include <vector>
#include <memory>
#include <exception>
#include "word_parser.h"

namespace codemaster
{

class ThesaurusParse
{
public:
    typedef std::unordered_map<std::string, std::shared_ptr<Parser> > str_parser_map;
    ThesaurusParse() noexcept;
    virtual ~ThesaurusParse() {};
public:
    void read(const std::string& file);
    void read(std::istream& input);
    void parse_row();
    bool has_next_row() const noexcept;
    void add_parser(const std::shared_ptr<Parser>& parser) noexcept;
    void set_scheme(const std::string& scheme);
    std::string get_scheme() const noexcept;

private:
    void parse_word_by_parser(const std::string& word, const std::string& parser) const noexcept;
    std::shared_ptr<std::istream> in;
    str_parser_map _parsers;
    std::vector<std::string> _scheme;
    DISALLOW_COPY_AND_ASSIGN(ThesaurusParse);
};

}

#endif
