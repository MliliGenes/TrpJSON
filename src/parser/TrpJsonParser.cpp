#include "../../include/parser/TrpJsonParser.hpp"

TrpJsonParser::TrpJsonParser( const std::string _file_name ) : parsed(false) {
    head = NULL;
    lexer = new TrpJsonLexer(_file_name);
    if (!lexer || (lexer && !lexer->isOpen())) {
        std::cerr << "Error: Failed to open file: " << _file_name << std::endl;
    }
}

TrpJsonParser::TrpJsonParser( void ) : parsed(false) {
    head = NULL;
    lexer = NULL;
}

TrpJsonParser::~TrpJsonParser( void ) {
    // idk
}

ITrpJsonValue* TrpJsonParser::parseValue( token& current_token ) {

    switch (current_token.type)
    {
        case T_BRACE_OPEN:
            //return parseObject();

        case T_BRACKET_OPEN:
            //return parseArray();

        case T_STRING:
            //return parseString();

        case T_NUMBER:
            //return parseNumbr();

        case T_TRUE: case T_FALSE: case T_NULL:
            //return parseLiteral();

        case T_END_OF_FILE:
            
        
        case T_ERROR:
            last_err = current_token.value; 
            std::cerr << "Error:\nline: " << current_token.line << " col: " << current_token.col << std::endl;
            std::cerr << "\t" << last_err << std::endl;

        default:
            std::cerr << "Error: Buy some memory storage brp!" << std::endl;
    }
    
    return NULL;
}

bool TrpJsonParser::parse( void ) {
    if (!lexer->isOpen()) {
        std::cerr << "Error: No file provided." << std::endl;
        return false;
    }

    token t = lexer->getNextToken();
    head = parseValue(t);
    
    if ( head != NULL ) {
        parsed = true;
        return true;
    }
    return false;
}   