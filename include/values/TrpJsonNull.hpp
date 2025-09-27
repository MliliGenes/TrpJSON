#pragma once

#include "../core/TrpJsonValue.hpp"

#ifndef TRPJSONNULL_HPP
#define TRPJSONNULL_HPP

class TrpJsonNull : public ITrpJsonValue {
    public:
        TrpJsonType getType( void ) const;
};

#endif // TRPJSONNULL_HPP