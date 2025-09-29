#include "../../include/parser/TrpJsonParser.hpp"

TrpJsonParser::TrpJsonParser( const std::string _file_name ) : lexer(_file_name), parsed(false) {
    if (!lexer.isOpen()) {
        std::cerr << "Error: Failed to open file: " << _file_name << std::endl;
    }
}

TrpJsonParser::~TrpJsonParser( void ) {
    // idk
}

ITrpJsonValue* TrpJsonParser::parse( void ) {
    if (!lexer.isOpen()) {
        std::cerr << "Error: No file provided." << std::endl;
        return NULL;
    }

    AutoPointer<ITrpJsonValue> auto_ptr;
    // performe a check before parsing

    token t = lexer.getNextToken();
    switch (t.type)
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
            std::cerr << "Error: Empty JSON file" << std::endl;
            return NULL;
        
        case T_ERROR:
            last_err = t.value; 
            std::cerr << "Error:\nline: " << t.line << " col: " << t.col << std::endl;
            std::cerr << "\t" << last_err << std::endl;

        default:
            break;
    }


    return NULL;
}