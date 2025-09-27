#pragma once

#include "TrpJsonValue.hpp"
#include <vector>

#ifndef TRPARRAY_HPP
#define TRPARRAY_HPP

class trpJsonArray : public ITrpJsonValue {
    private:
        vector<ITrpJsonValue*> m_value;
}; 

#endif // TRPARRAY_HPP