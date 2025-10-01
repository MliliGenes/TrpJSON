#pragma once

#include "../../include/core/TrpAutoPointer.hpp"
#include <string>
#include <stdexcept>
#include <cstdlib>
#include "../core/TrpJsonLexer.hpp"
#include "../core/TrpJsonValue.hpp"
#include "../values/TrpJsonObject.hpp"
#include "../values/TrpJsonArray.hpp"
#include "../values/TrpJsonString.hpp"
#include "../values/TrpJsonNumber.hpp"
#include "../values/TrpJsonBool.hpp"
#include "../values/TrpJsonNull.hpp"

#ifndef TRPJSONPARSER_HPP
#define TRPJSONPARSER_HPP

#define RESET       "\033[0m"
#define STRING_COLOR "\033[31m"      // Red for strings
#define NUMBER_COLOR "\033[33m"      // Yellow for numbers
#define BOOL_COLOR   "\033[32m"      // Green for true/false
#define NULL_COLOR   "\033[35m"      // Magenta for null
#define KEY_COLOR    "\033[94m"      // Bright blue for keys
#define BRACE_COLOR  "\033[36m"      // Cyan for {} []
#define PUNCT_COLOR  "\033[37m"      // White for punctuation

class TrpJsonParser {
    private:
        TrpJsonLexer* lexer;
        ITrpJsonValue* head;
        bool parsed;
        token last_err;


        ITrpJsonValue* parseArray( token& current_token );
        ITrpJsonValue* parseObject( token& current_token );
        ITrpJsonValue* parseString( token& current_token );
        ITrpJsonValue* parseNumber( token& current_token );
        ITrpJsonValue* parseLiteral( token& current_token );

        ITrpJsonValue* parseValue( token& current_token );
        

    public:
        TrpJsonParser( void );
        TrpJsonParser( const std::string _file_nmae );
        ~TrpJsonParser( void );

        void resetLexer( TrpJsonLexer* new_lexer );
        void setLexer( TrpJsonLexer* _lexer);

        bool parse( void );
        ITrpJsonValue* getAST( void ) const;

        bool isParsed( void ) const;                       
        const token& getLastError( void ) const;
        void lastError( void ) const;
        void clearAST( void );

        void reset( void );
        ITrpJsonValue* release( void );

        std::string astToString( void ) const;
        void prettyPrint() const;

    private:
        TrpJsonParser( const TrpJsonParser& other );
        TrpJsonParser& operator=( const TrpJsonParser& other );
};

#endif // TRPJSONPARSER_HPP