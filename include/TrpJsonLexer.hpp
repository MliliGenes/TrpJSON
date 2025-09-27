#pragma once

#include <iostream>
#include <fstream>

#ifndef TRPLEXER_HPP
#define TRPLEXER_HPP

enum TrpTokenType {
    T_BRACE_OPEN,       // {
    T_BRACE_CLOSE,      // }
    T_BRACKET_OPEN,     // [
    T_BRACKET_CLOSE,    // ]
    T_COLON,            // :
    T_COMMA,            // ,
    T_STRING,           // "..."
    T_NUMBER,           // 123, 3.14
    T_TRUE,             // true
    T_FALSE,            // false
    T_NULL,             // null
    T_END_OF_FILE,      // EOF
    T_ERROR
};

class TrpJsonLexer {
    private:
        bool opened;
        std::ifstream json_file;
        std::string file_name;
        size_t line, col;

    public:
        TrpJsonLexer(std::string file_name);
};

#endif // TRPLEXER_HPP