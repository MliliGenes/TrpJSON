#include "../include/TrpJsonObject.hpp"

TrpJsonObject::TrpJsonObject( void ) {}

TrpJsonObject::~TrpJsonObject( void ) {
    for (JsonObjectMap::iterator it = m_members.begin();
            it != m_members.end(); it++) {
        delete it->second;
        it->second = NULL;
    }
}

TrpJsonType TrpJsonObject::getType( void ) const {
    return (TRP_OBJECT);
}

void TrpJsonObject::add(std::string key,ITrpJsonValue* value) {
    JsonObjectMap::iterator it = m_members.find(key);
    if (it != m_members.end()) {
        delete it->second;
        it->second = value;
    } else {
        m_members.insert(JsonObjectEntry(key, value));
    }
}

ITrpJsonValue* TrpJsonObject::find(std::string key) {
    JsonObjectMap::iterator it = m_members.find(key);
    if (it != m_members.end()) {
        return it->second;
    }
    return NULL;
}
