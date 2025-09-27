#include "../include/TrpJsonArray.hpp"

TrpJsonArray::TrpJsonArray( void ) {};

TrpJsonArray::~TrpJsonArray( void ) {
    for (JsonArrayVector::iterator it = m_elements.begin();
            it != m_elements.end(); it++) {
        delete *it;
    }
}

TrpJsonType TrpJsonArray::getType( void ) const {
    return (TRP_ARRAY);
}

void TrpJsonArray::add(ITrpJsonValue *value) {
    m_elements.push_back(value);
}

ITrpJsonValue* TrpJsonArray::at(size_t index) {
    return m_elements.at(index);
}