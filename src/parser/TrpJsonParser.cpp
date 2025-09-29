#include "../../include/parser/TrpJsonParser.hpp"

TrpJsonParser::TrpJsonParser( const std::string _file_name ) : lexer(_file_name), trp_loop(true) {
    if (lexer.isOpen()) {
        trp_loop = false;
        std::cerr << "Error: Failed to open file: " << _file_name << std::endl;
        return;
    }
}

TrpJsonParser::~TrpJsonParser( void ) {
    // idk
}

ITrpJsonValue* TrpJsonParser::parse( void ) {
    if (!trp_loop) {
        std::cerr << "Error: No file provided." << std::endl;
        return NULL;
    }


    return NULL;
}