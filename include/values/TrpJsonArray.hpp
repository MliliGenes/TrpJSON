#pragma once

#include "../core/TrpJsonValue.hpp"
#include <vector>
#include <cstddef>

#ifndef TRPJSONARRAY_HPP  
#define TRPJSONARRAY_HPP

class TrpJsonArray;

typedef std::vector<ITrpJsonValue*> JsonArrayVector;

class TrpJsonArray : public ITrpJsonValue {
    private:
        JsonArrayVector m_elements;

    public:
        TrpJsonArray( void );
        ~TrpJsonArray( void );
        TrpJsonType getType( void ) const;
        void add(ITrpJsonValue* value);
        ITrpJsonValue* at(size_t index);
        size_t size( void ) const;
}; 

#endif // TRPARRAY_HPP