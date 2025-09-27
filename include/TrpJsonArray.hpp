#pragma once

#include "TrpJsonValue.hpp"
#include <vector>

#ifndef TRPJSONARRAY_HPP  
#define TRPJSONARRAY_HPP

class TrpJsonArray : public ITrpJsonValue {
    private:
        std::vector<ITrpJsonValue*> m_elements;
    
    public:
        TrpJsonArray( void );
        ~TrpJsonArray( void );
        TrpJsonType getType( void ) const;
        void add(ITrpJsonValue* value);
}; 

#endif // TRPARRAY_HPP