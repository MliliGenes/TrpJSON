#pragma once

#include "TrpJsonType.hpp"

#ifndef TRPVALUE_HPP
#define TRPVALUE_HPP

class ITrpJsonValue;

class ITrpJsonValue {
    public:
        ITrpJsonValue( void ) {}
        virtual ~ITrpJsonValue( void ) = 0;
        virtual TrpJsonType getType( void ) const = 0;
};


#endif // TRPVALUE_HPP