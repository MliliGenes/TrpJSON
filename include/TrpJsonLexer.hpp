#pragma once

#include <iostream>
#include <fstream>
#include <vector>

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

struct token {
    TrpTokenType type;
    std::string value;  // only used for strings, numbers, errors
    size_t line;        // 0-based line number
    size_t col;         // 0-based column number
};

class TrpJsonLexer {
    private:
        bool opened;
        std::vector<std::string> lines;
        std::ifstream json_file;
        std::string file_name;
        size_t line, col;

        void skipWhitespace();
        char peekChar() const;
        char getChar();
        TrpToken readString();
        TrpToken readNumber();
        TrpToken readLiteral();


    public:
        TrpJsonLexer(std::string file_name);
        ~TrpJsonLexer();
        TrpToken getNextToken();
        bool isOpen() const { return opened; }
};

#endif // TRPLEXER_HPP


// value  → object | array | string | number | true | false | null
// object → '{' (string ':' value (',' string ':' value)*)? '}'
// array  → '[' (value (',' value)*)? ']'