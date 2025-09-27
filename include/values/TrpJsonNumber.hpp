#pragma once

#include "../core/TrpJsonValue.hpp"

#ifndef TRPJSONNUMBER_HPP
#define TRPJSONNUMBER_HPP

class TrpJsonNumber : public ITrpJsonValue {
    private:
        double m_value;
    
    public:
        TrpJsonNumber(double value) : m_value(value) {}
        ~TrpJsonNumber( void );
        TrpJsonType getType( void ) const;
        const double& getValue( void ) const;
};

#endif // TRPJSONNUMBER_HPP