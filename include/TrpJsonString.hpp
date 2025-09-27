#pragma once

#include "TrpValue.hpp"
#include <string>

#ifndef TRPJSONSTRING_HPP
#define TRPJSONSTRING_HPP

class TrpJsonString : public ITrpJsonValue {
    private:
        std::string m_value;
    
    public:
        TrpJsonString(std::string value) : m_value(value) {}
        ~TrpJsonString( void );
        TrpJsonType getType( void ) const;
        const std::string& getValue( void ) const;
};

#endif // TRPJSONSTRING_HPP