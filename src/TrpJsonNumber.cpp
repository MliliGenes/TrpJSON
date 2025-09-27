#include "../include/TrpJsonNumber.hpp"

TrpJsonNumber::~TrpJsonNumber( void ) {}

TrpJsonType TrpJsonNumber::getType( void ) const {
    return TRP_NUMBER;
}

const double& TrpJsonNumber::getValue( void ) const {
    return m_value;
}