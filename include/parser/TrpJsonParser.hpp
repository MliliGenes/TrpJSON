#pragma once

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
        bool TrpLoop;

    public:
        TrpJsonParser( void );
        TrpJsonParser( TrpJsonLexer& lexer );
        ~TrpJsonParser( void );
        void parse( void );
};

#endif // TRPJSONPARSER_HPP