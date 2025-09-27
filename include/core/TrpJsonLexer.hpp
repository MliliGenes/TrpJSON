#pragma once

#include <iostream>
#include <fstream>
#include <vector>

#ifndef TRPJSONLEXER_HPP
#define TRPJSONLEXER_HPP

enum TrpTokenType
{
    T_BRACE_OPEN,
    T_BRACE_CLOSE,
    T_BRACKET_OPEN,
    T_BRACKET_CLOSE,
    T_COLON,
    T_COMMA,
    T_STRING,
    T_NUMBER,
    T_TRUE,
    T_FALSE,
    T_NULL,
    T_END_OF_FILE,
    T_ERROR
};

struct token
{
    TrpTokenType type;
    std::string value;
    size_t line; // 0-based line number
    size_t col;  // 0-based column number
};

typedef std::string::iterator stringIterator;
class TrpJsonLexer
{
private:
    // File data
    std::ifstream json_file;
    std::string file_name;

    // Line data
    bool has_next_line;
    std::string current_line;
    std::string next_line;

    // Cords for Debugging and Errors
    size_t line;
    size_t col;

    // Iterator cause it cool
    stringIterator current;
    stringIterator line_end;

    // Skipping whitespaces
    void skipWhitespace();

    // Peeking utils
    char peekChar() const;
    char getChar();

    // Lexer controlers
    void advanceLexer();
    void pushBackLexer();

    // Raw types tokens
    token readString();
    token readNumber();
    token readLiteral();
    token createErrorToken(const std::string &message);

    // controling lines boundries
    bool loadNextLineIfNeeded();
    bool isAtEndOfLine() const;
    bool isAtEnd() const;

public:
    TrpJsonLexer(std::string file_name);
    ~TrpJsonLexer(void);

    // the holy get next token; minishell refrance lmfao
    token getNextToken(void);
};

#endif // TRPJSONLEXER_HPP

// value  → object | array | string | number | true | false | null
// object → '{' (string ':' value (',' string ':' value)*)? '}'
// array  → '[' (value (',' value)*)? ']'