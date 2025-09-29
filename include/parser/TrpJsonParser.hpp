#pragma once

#include "../../include/core/AutoPointer.hpp"
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
        TrpJsonLexer lexer;
        bool trp_loop;

        ITrpJsonValue* parseArray( token& current_token );
        ITrpJsonValue* parseObject( token& current_token );
        ITrpJsonValue* parseString( token& current_token );
        ITrpJsonValue* parseNumber( token& current_token );
        ITrpJsonValue* parseLiteral( token& current_token );

    public:
        TrpJsonParser( const std::string _file_nmae );
        ~TrpJsonParser( void );
        ITrpJsonValue* parse( void );
};

#endif // TRPJSONPARSER_HPP