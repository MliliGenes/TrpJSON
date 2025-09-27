#pragma once

#include <iostream>
#include <fstream>
#include <vector>

#ifndef TRPJSONLEXER_HPP
#define TRPJSONLEXER_HPP

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

typedef std::string::iterator stringIterator;
class TrpJsonLexer {
    private:
        std::ifstream json_file;
        std::string file_name;
        std::string current_line;    // Currently processed line
        std::string next_line;      // Buffer for next line (for lookahead)
        size_t line;                // Current line number
        size_t col;                 // Current column number (for error reporting)
        stringIterator current;  // Current position in the line
        stringIterator line_end; // End of the current line
        bool has_next_line;         // Indicates if next_line is valid

        void skipWhitespace();
        char peekChar() const;
        char getChar();
        void advanceLexer();
        void pushBackLexer();
        bool loadNextLineIfNeeded();
        token readString();
        token readNumber();
        token readLiteral();
        token createErrorToken(const std::string& message);
        bool isAtEndOfLine() const;
        bool isAtEnd() const;


    public:
        TrpJsonLexer(std::string file_name);
        ~TrpJsonLexer( void );
        token getNextToken( void );
};

#endif // TRPJSONLEXER_HPP


// value  → object | array | string | number | true | false | null
// object → '{' (string ':' value (',' string ':' value)*)? '}'
// array  → '[' (value (',' value)*)? ']'