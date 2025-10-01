#include "../../include/parser/TrpJsonParser.hpp"
#include "../../include/values/TrpJsonObject.hpp"
#include "../../include/values/TrpJsonArray.hpp"
#include "../../include/values/TrpJsonString.hpp"
#include "../../include/values/TrpJsonNumber.hpp"
#include "../../include/values/TrpJsonBool.hpp"
#include <iostream>
#include <string>
#include <sstream>

#define RESET       "\033[0m"
#define STRING_COLOR "\033[31m"      // Red for strings
#define NUMBER_COLOR "\033[33m"      // Yellow for numbers
#define BOOL_COLOR   "\033[32m"      // Green for true/false
#define NULL_COLOR   "\033[35m"      // Magenta for null
#define KEY_COLOR    "\033[94m"      // Bright blue for keys
#define BRACE_COLOR  "\033[36m"      // Cyan for {} []
#define PUNCT_COLOR  "\033[37m"      // White for punctuation

std::string createTabIndent(int level) {
    return std::string(level, '\t');
}

std::string astValueToString(ITrpJsonValue* value, int indentLevel = 0);

std::string astObjectToString(TrpJsonObject* obj, int indentLevel) {
    std::ostringstream oss;
    std::string indent = createTabIndent(indentLevel);
    std::string nextIndent = createTabIndent(indentLevel + 1);
    
    oss << BRACE_COLOR << "{" << RESET << "\n";
    
    size_t count = 0;
    for (JsonObjectMap::const_iterator it = obj->begin(); it != obj->end(); ++it) {
        oss << nextIndent 
            << KEY_COLOR << "\"" << it->first << "\"" << RESET 
            << PUNCT_COLOR << ": " << RESET 
            << astValueToString(it->second, indentLevel + 1);
        
        if (++count < obj->size()) {
            oss << PUNCT_COLOR << "," << RESET;
        }
        oss << "\n";
    }
    
    oss << indent << BRACE_COLOR << "}" << RESET;
    return oss.str();
}

std::string astArrayToString(TrpJsonArray* arr, int indentLevel) {
    std::ostringstream oss;
    std::string indent = createTabIndent(indentLevel);
    std::string nextIndent = createTabIndent(indentLevel + 1);
    
    oss << BRACE_COLOR << "[" << RESET << "\n";
    
    for (size_t i = 0; i < arr->size(); ++i) {
        oss << nextIndent << astValueToString(arr->at(i), indentLevel + 1);
        
        if (i < arr->size() - 1) {
            oss << PUNCT_COLOR << "," << RESET;
        }
        oss << "\n";
    }
    
    oss << indent << BRACE_COLOR << "]" << RESET;
    return oss.str();
}

std::string astValueToString(ITrpJsonValue* value, int indentLevel) {
    if (!value) {
        return NULL_COLOR "null" RESET;
    }
    
    switch (value->getType()) {
        case TRP_STRING: {
            TrpJsonString* str = static_cast<TrpJsonString*>(value);
            return STRING_COLOR "\"" + str->getValue() + "\"" RESET;
        }
        
        case TRP_NUMBER: {
            TrpJsonNumber* num = static_cast<TrpJsonNumber*>(value);
            std::ostringstream oss;
            oss << NUMBER_COLOR << num->getValue() << RESET;
            return oss.str();
        }
        
        case TRP_BOOL: {
            TrpJsonBool* boolean = static_cast<TrpJsonBool*>(value);
            return std::string(BOOL_COLOR) + (boolean->getValue() ? "true" : "false") + RESET;
        }
        
        case TRP_NULL: {
            return NULL_COLOR "null" RESET;
        }
        
        case TRP_ARRAY: {
            TrpJsonArray* arr = static_cast<TrpJsonArray*>(value);
            return astArrayToString(arr, indentLevel);
        }
        
        case TRP_OBJECT: {
            TrpJsonObject* obj = static_cast<TrpJsonObject*>(value);
            return astObjectToString(obj, indentLevel);
        }
        
        default:
            return STRING_COLOR "\"UNKNOWN_TYPE\"" RESET;
    }
}

std::string TrpJsonParser::astToString(void) const {
    if (!parsed || !head) {
        return NULL_COLOR "null" RESET;
    }
    
    return astValueToString(head, 0);
}

void TrpJsonParser::prettyPrint() const {
    std::cout << astToString() << std::endl;
}