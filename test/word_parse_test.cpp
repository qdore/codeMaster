#include <string>
#include <gtest/gtest.h>
#include <memory>

#include "../word_parser.h"

using std::string;
using std::shared_ptr;
using codemaster::BuiltinParse;
using codemaster::make_parser;
using codemaster::make_array_parser;
using codemaster::Parser;

#define TEST_WORD_PARSER(parse_string, string_can_be_parsed, parsed_value)      \
    if (string_can_be_parsed)                                                   \
    {                                                                           \
        EXPECT_TRUE(word_parser->parse(parse_string).get() != nullptr);         \
        if (word_parser->parse(parse_string).get() != nullptr)                  \
        {                                                                       \
            EXPECT_EQ(word_parser->parse(parse_string)->to_string(),            \
                parsed_value);                                                  \
        }                                                                       \
    }                                                                           \
    else                                                                        \
    {                                                                           \
        EXPECT_TRUE(word_parser->parse(parse_string).get() == nullptr);         \
    }

TEST(TestWordParser, int)
{
    shared_ptr<Parser> word_parser;
    word_parser = make_parser("int", BuiltinParse<int>());

    TEST_WORD_PARSER("", false, "0");
    TEST_WORD_PARSER("123", true, "123");
    TEST_WORD_PARSER("-123", true, "-123");
    TEST_WORD_PARSER("0000", true, "0");
    TEST_WORD_PARSER("0000120", true, "120");
    TEST_WORD_PARSER("123asd", false, "0");
    TEST_WORD_PARSER("asd123", false, "0");
}

TEST(TestWordParser, float)
{
    shared_ptr<Parser> word_parser;
    word_parser = make_parser("float", BuiltinParse<float>());

    TEST_WORD_PARSER("   ", false, "0");
    TEST_WORD_PARSER("123.1847", true, "123.185");
    TEST_WORD_PARSER("-123.12", true, "-123.12");
    TEST_WORD_PARSER("0000", true, "0");
    TEST_WORD_PARSER("0000120.23", true, "120.23");
    TEST_WORD_PARSER("123asd", false, "0");
    TEST_WORD_PARSER("asd123", false, "0");
}

TEST(TestWordParser, double)
{
    shared_ptr<Parser> word_parser;
    word_parser = make_parser("double", BuiltinParse<double>());

    TEST_WORD_PARSER("", false, "0");
    TEST_WORD_PARSER("1230000000000.1847", true, "1.23e+12");
    TEST_WORD_PARSER("-123000000000.12", true, "-1.23e+11");
    TEST_WORD_PARSER("0000", true, "0");
    TEST_WORD_PARSER("0000120.23", true, "120.23");
    TEST_WORD_PARSER("123asd", false, "0");
    TEST_WORD_PARSER("asd123", false, "0");
}

TEST(TestWordParser, string)
{
    shared_ptr<Parser> word_parser;
    word_parser = make_parser("string", BuiltinParse<string>());

    TEST_WORD_PARSER("", false, "0");
    TEST_WORD_PARSER("1230000000000.1847", true, "1230000000000.1847");
    TEST_WORD_PARSER("-123000000000.12", true, "-123000000000.12");
    TEST_WORD_PARSER("0000", true, "0000");
    TEST_WORD_PARSER("0000120.23", true, "0000120.23");
    TEST_WORD_PARSER("123asd", true, "123asd");
    TEST_WORD_PARSER("asd123", true, "asd123");
    TEST_WORD_PARSER(" asdjh asjdh asjhd  ", true, "asdjh asjdh asjhd");
}

TEST(TestWordParserArray, int)
{
    shared_ptr<Parser> word_parser;
    word_parser = make_array_parser("int[]", BuiltinParse<int>());

    TEST_WORD_PARSER("", false, "0");
    TEST_WORD_PARSER("123", false, "123");
    TEST_WORD_PARSER("2:-123,123", true, "{-123, 123}");
    TEST_WORD_PARSER("2: -123, 123", true, "{-123, 123}");
    TEST_WORD_PARSER("3:0000, 0000120, 021", true, "{0, 120, 21}");
    TEST_WORD_PARSER("3:asd1, 0000120, 021", false, "0");
    TEST_WORD_PARSER("3:12, 000f120, 021", false, "0");
    TEST_WORD_PARSER("3:12, , 021", false, "0");
}

TEST(TestWordParserArray, float)
{
    shared_ptr<Parser> word_parser;
    word_parser = make_array_parser("float[]", BuiltinParse<float>());

    TEST_WORD_PARSER("", false, "0");
    TEST_WORD_PARSER("123", false, "123");
    TEST_WORD_PARSER("2:-123.1837,123.12", true, "{-123.184, 123.12}");
    TEST_WORD_PARSER("2:-123.1837, 123.12", true, "{-123.184, 123.12}");
    TEST_WORD_PARSER("3:0000.12, 0000120.12, 021.0", true, "{0.12, 120.12, 21}");
    TEST_WORD_PARSER("3:asd1, 0000120, 021", false, "0");
    TEST_WORD_PARSER("3:12, 000f120, 021", false, "0");
    TEST_WORD_PARSER("3:12, , 021", false, "0");
}

TEST(TestWordParserArray, double)
{
    shared_ptr<Parser> word_parser;
    word_parser = make_array_parser("double", BuiltinParse<double>());

    TEST_WORD_PARSER("", false, "0");
    TEST_WORD_PARSER("123", false, "123");
    TEST_WORD_PARSER("2:-123.1837,123.12", true, "{-123.184, 123.12}");
    TEST_WORD_PARSER("2:-123.1837, 123.12", true, "{-123.184, 123.12}");
    TEST_WORD_PARSER("3:0000.12, 0000120.12, 021.0", true, "{0.12, 120.12, 21}");
    TEST_WORD_PARSER("3:asd1, 0000120, 021", false, "0");
    TEST_WORD_PARSER("3:12, 000f120, 021", false, "0");
    TEST_WORD_PARSER("3:12, , 021", false, "0");
}

TEST(TestWordParserArray, string)
{
    shared_ptr<Parser> word_parser;
    word_parser = make_array_parser("string", BuiltinParse<string>());

    TEST_WORD_PARSER("", false, "0");
    TEST_WORD_PARSER("123", false, "123");
    TEST_WORD_PARSER("2:-123.1837,123.12", true, "{-123.1837, 123.12}");
    TEST_WORD_PARSER("2:-123.1837, 123.12", true, "{-123.1837, 123.12}");
    TEST_WORD_PARSER("3:0000.12, .12, 021.0", true, "{0000.12, .12, 021.0}");
    TEST_WORD_PARSER("3:asd1, 0000120, 021", true, "{asd1, 0000120, 021}");
    TEST_WORD_PARSER("3:12, 000f120, 021", true, "{12, 000f120, 021}");
    TEST_WORD_PARSER("3:12, , 021", false, "0");
}

TEST(TestTrimSpase, trim)
{
    string str("      a  sd      ");
    codemaster::ltrim(str);
    EXPECT_EQ(str, "a  sd      ");

    str = "      a  sd      ";
    codemaster::rtrim(str);
    EXPECT_EQ(str, "      a  sd");

    str = "      a  sd      ";
    codemaster::trim(str);
    EXPECT_EQ(str, "a  sd");
}
