#pragma once

#include "TrpJsonValue.hpp"
#include <map>
#include <string>

#ifndef TRPOBJECT_HPP
#define TRPOBJECT_HPP

class TrpJsonObject;

typedef std::map<std::string, ITrpJsonValue*> JsonObjectMap;
typedef std::pair<std::string, ITrpJsonValue*> JsonObjectEntry;

class TrpJsonObject : public ITrpJsonValue {
    private:
        JsonObjectMap   m_members;

    public:
        TrpJsonObject( void );
        ~TrpJsonObject( void );
        TrpJsonType getType( void ) const;
        void add(std::string key, ITrpJsonValue* value);
        ITrpJsonValue* find(std::string key);
};

#endif // TRPOBJECT_HPP