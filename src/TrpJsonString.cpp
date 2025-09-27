#include "../include/TrpJsonString.hpp"

TrpJsonString::~TrpJsonString( void ) {}

TrpJsonType TrpJsonString::getType( void ) const {
    return TRP_STRING;
}

const std::string& TrpJsonString::getValue( void ) const {
    return m_value;
}