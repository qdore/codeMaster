#ifndef _CONFIG_H_
#define _CONFIG_H_

#include <string>
#include <iostream>

#define DISALLOW_COPY_AND_ASSIGN(TypeName) \
        TypeName(const TypeName&); \
        TypeName& operator=(const TypeName&)

#define THESAURUS_ROW_DELIM '\n'
#define THESAURUS_COLUMN_DELIM '\t'
#define ARRAY_INDICATE ':'
#define ARRAY_DELIM ','
#define ANY_PARSER_INDICATOR "*"

#endif
