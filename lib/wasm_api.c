#include "./TrpJson.hpp"
#include <string>
#include <emscripten.h>

extern "C" {
    // --- Parser Lifecycle ---

    EMSCRIPTEN_KEEPALIVE
    void* parse_json_to_ast(const char* json_raw) {
        TrpJsonLexer* lexer = new TrpJsonLexer(json_raw, true);
        TrpJsonParser* parser = new TrpJsonParser();
        parser->setLexer(lexer);
        
        if (parser->parse()) {
            // Release ownership of the AST from the parser so it isn't deleted
            ITrpJsonValue* root = parser->release();
            delete parser; // cleans up lexer
            return static_cast<void*>(root);
        }
        delete parser;
        return NULL;
    }

    EMSCRIPTEN_KEEPALIVE
    void free_ast(void* node_ptr) {
        if (node_ptr) {
            delete static_cast<ITrpJsonValue*>(node_ptr);
        }
    }

    // --- Node Inspection ---

    EMSCRIPTEN_KEEPALIVE
    int get_node_type(void* node_ptr) {
        if (!node_ptr) return TRP_NULL;
        return static_cast<ITrpJsonValue*>(node_ptr)->getType();
    }

    // --- Object Traversal ---

    EMSCRIPTEN_KEEPALIVE
    size_t get_object_size(void* node_ptr) {
        TrpJsonObject* obj = static_cast<TrpJsonObject*>(node_ptr);
        return obj->size();
    }

    // Helper to get a key by index (since JS can't easily iterate a C++ std::map)
    EMSCRIPTEN_KEEPALIVE
    const char* get_object_key_at(void* node_ptr, size_t index) {
        TrpJsonObject* obj = static_cast<TrpJsonObject*>(node_ptr);
        size_t i = 0;
        for (JsonObjectMap::const_iterator it = obj->begin(); it != obj->end(); ++it) {
            if (i == index) return it->first.c_str();
            i++;
        }
        return "";
    }

    EMSCRIPTEN_KEEPALIVE
    void* get_object_value_at(void* node_ptr, size_t index) {
        TrpJsonObject* obj = static_cast<TrpJsonObject*>(node_ptr);
        size_t i = 0;
        for (JsonObjectMap::const_iterator it = obj->begin(); it != obj->end(); ++it) {
            if (i == index) return static_cast<void*>(it->second);
            i++;
        }
        return NULL;
    }

    // --- Array Traversal ---

    EMSCRIPTEN_KEEPALIVE
    size_t get_array_size(void* node_ptr) {
        TrpJsonArray* arr = static_cast<TrpJsonArray*>(node_ptr);
        return arr->size();
    }

    EMSCRIPTEN_KEEPALIVE
    void* get_array_value_at(void* node_ptr, size_t index) {
        TrpJsonArray* arr = static_cast<TrpJsonArray*>(node_ptr);
        return static_cast<void*>(arr->at(index));
    }

    // --- Value Extraction ---

    EMSCRIPTEN_KEEPALIVE
    const char* get_value_string(void* node_ptr) {
        return static_cast<TrpJsonString*>(node_ptr)->getValue().c_str();
    }

    EMSCRIPTEN_KEEPALIVE
    double get_value_number(void* node_ptr) {
        return static_cast<TrpJsonNumber*>(node_ptr)->getValue();
    }

    EMSCRIPTEN_KEEPALIVE
    bool get_value_bool(void* node_ptr) {
        return static_cast<TrpJsonBool*>(node_ptr)->getValue();
    }
}
