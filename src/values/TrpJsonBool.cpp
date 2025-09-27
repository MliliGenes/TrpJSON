#include "../../include/values/TrpJsonBool.hpp"

TrpJsonBool::~TrpJsonBool( void ) {}

TrpJsonType TrpJsonBool::getType( void ) const {
    return TRP_BOOL;
}

const bool& TrpJsonBool::getValue( void ) const {
    return m_value;
}