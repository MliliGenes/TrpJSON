#pragma once

#include "TrpValue.hpp"
#include <string>

#ifndef TRPSTRING_HPP
#define TRPSTRING_HPP

class TrpJsonString;

class TrpJsonString : public ITrpJsonValue {
    private:
        std::string string;
    
    public:
        TrpJsonString(std::string _string) : string(_string) {}
        ~TrpJsonString( void );
        TrpJsonType getType( void ) const;
        TrpJsonType getValue( void ) const;
};

#endif // TRPSTRING_HPP