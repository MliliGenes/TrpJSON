#pragma once

#include "TrpJsonValue.hpp"
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
}; 

#endif // TRPARRAY_HPP