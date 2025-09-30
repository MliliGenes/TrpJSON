#pragma once

#include "../../include/core/TrpAutoPointer.hpp"
#include <string>
#include <stdexcept>
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


class TrpJsonParser {
    private:
        TrpJsonLexer* lexer;
        ITrpJsonValue* head;
        bool parsed;
        std::string last_err;


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

        bool parse( void );
        ITrpJsonValue* getAST( void ) const;

        bool isParsed( void ) const;                       
        const std::string& getLastError( void ) const;
        void clearAST( void );



    private:
        TrpJsonParser( const TrpJsonParser& other );
        TrpJsonParser& operator=( const TrpJsonParser& other );
};

#endif // TRPJSONPARSER_HPP