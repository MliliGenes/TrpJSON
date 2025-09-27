#pragma once

#include "TrpValue.hpp"

#ifndef TRPJSONBOOL_HPP
#define TRPJSONBOOL_HPP

class TrpJsonBool : public ITrpJsonValue {
    private:
        bool m_value;
    
    public:
        TrpJsonBool(bool value) : m_value(value) {}
        ~TrpJsonBool( void );
        TrpJsonType getType( void ) const;
        const bool& getValue( void ) const;
};

#endif // TRPJSONBOOL_HPP