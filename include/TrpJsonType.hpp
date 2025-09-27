#pragma once

#ifndef TRP_TOKEN_TYPE
#define TRP_TOKEN_TYPE

enum TrpType {
    TRP_NULL,
    TRP_BOOL,
    TRP_NUMBER,
    TRP_STRING,
    TRP_ARRAY,
    TRP_OBJECT
};

typedef enum TrpType TrpJsonType;

#endif
