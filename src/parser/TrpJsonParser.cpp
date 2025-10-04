#include "../../include/parser/TrpJsonParser.hpp"

TrpJsonParser::TrpJsonParser( const std::string _file_name ) : parsed(false) {
    head = NULL;
    lexer = new TrpJsonLexer(_file_name);
}

TrpJsonParser::TrpJsonParser( void ) : parsed(false) {
    head = NULL;
    lexer = NULL;
}

// we can create class with no lexer and later inject the lexer
void TrpJsonParser::setLexer( TrpJsonLexer* _lexer ) {
    resetLexer( _lexer );
}

void TrpJsonParser::resetLexer( TrpJsonLexer* new_lexer ) {
    if ( !new_lexer || !new_lexer->isOpen() ) return;
    if ( lexer ) delete lexer;
    lexer = new_lexer;
}

ITrpJsonValue* TrpJsonParser::getAST( void ) const { return head; }
bool TrpJsonParser::isParsed( void ) const { return parsed; }
const token& TrpJsonParser::getLastError( void ) const { return last_err; }

void TrpJsonParser::lastError( token t ) {
    if ( t.type != T_ERROR ) t.value = "Expected token";
    std::cerr << lexer->getFileName() << ":"
    << t.line << ":"
    << t.col << " "
    << "Error: " << t.value << std::endl;
    last_err = t;
}

void TrpJsonParser::clearAST( void ) {
    delete head;
    head = NULL;
}

void TrpJsonParser::reset( void ) {
    clearAST();
    parsed = false;
    last_err.type = T_ERROR;
    last_err.value.clear();
    last_err.col = 0;
    last_err.line = 0;
}

ITrpJsonValue* TrpJsonParser::release( void ) {
    ITrpJsonValue* tmp = head;
    head = NULL;
    return tmp;
}

TrpJsonParser::~TrpJsonParser( void ) {
    clearAST();
    delete lexer;
    lexer = NULL;
}

ITrpJsonValue* TrpJsonParser::parseValue( token& current_token ) {

    switch (current_token.type)
    {
        case T_BRACE_OPEN:
            return parseObject( current_token );
        case T_BRACKET_OPEN:
            return parseArray( current_token );
        case T_STRING:
            return parseString( current_token );
        case T_NUMBER:
            return parseNumber( current_token );
        case T_TRUE: case T_FALSE: case T_NULL:
            return parseLiteral( current_token );
        case T_END_OF_FILE:
            break;
        case T_ERROR:
            lastError( current_token );
            break;
        default:
            break;
    }

    return NULL;
}

bool TrpJsonParser::parse( void ) {
    if ( !lexer ) {
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

ITrpJsonValue* TrpJsonParser::parseArray( token& current_token ) {
    if ( current_token.type != T_BRACKET_OPEN ) return NULL;

    AutoPointer<TrpJsonArray> arr_ptr(new TrpJsonArray());

    token t = lexer->getNextToken();
    if ( t.type == T_BRACKET_CLOSE ) return arr_ptr.release();

    while ( true ) {
        ITrpJsonValue* tmp_value = parseValue(t);
        if ( !tmp_value ) return NULL;

        arr_ptr->add(tmp_value);

        t = lexer->getNextToken();
        if ( t.type == T_BRACKET_CLOSE ) {
            break;
        } else if ( t.type == T_COMMA ) {
            t = lexer->getNextToken();
            continue;
        } else {
            lastError( t );
            return NULL;
        }
    }

    return arr_ptr.release();
}

ITrpJsonValue* TrpJsonParser::parseObject( token& current_token ) {
    if ( current_token.type != T_BRACE_OPEN ) return NULL;

    AutoPointer<TrpJsonObject> obj_ptr( new TrpJsonObject() );

    token t = lexer->getNextToken();
    if ( t.type == T_BRACE_CLOSE ) return obj_ptr.release();

    while ( true ) {
        if ( t.type != T_STRING ) {
            lastError( t );
            return NULL;
        }

        std::string key = t.value;

        t = lexer->getNextToken();
        if ( t.type != T_COLON ) {
            lastError( t );
            return NULL;
        };

        t = lexer->getNextToken();
        ITrpJsonValue* tmp_value = parseValue( t );
        if ( !tmp_value ) {
            return NULL;
        }
        obj_ptr->add(key, tmp_value);

        t = lexer->getNextToken();
        if ( t.type == T_BRACE_CLOSE ) {
            break;
        } else if ( t.type == T_COMMA ) {
            t = lexer->getNextToken();
            continue;
        } else {
            lastError( t );
            return NULL;
        }
    }

    return obj_ptr.release();
}

ITrpJsonValue* TrpJsonParser::parseString( token& current_token ) {
    if ( current_token.type != T_STRING ) return NULL;

    return new TrpJsonString(current_token.value);
}

ITrpJsonValue* TrpJsonParser::parseNumber( token& current_token ) {
    if ( current_token.type != T_NUMBER ) return NULL;

    double value = std::atof(current_token.value.c_str());
    return new TrpJsonNumber(value);
}

ITrpJsonValue* TrpJsonParser::parseLiteral( token& current_token ) {
    if ( !(current_token.type == T_NULL || current_token.type == T_TRUE || current_token.type == T_FALSE) )
        return NULL;
    
    switch (current_token.type) {
        case T_NULL:
            return new TrpJsonNull();
        case T_TRUE:
            return new TrpJsonBool(true);
        case T_FALSE:
            return new TrpJsonBool(false);
        default:
            return NULL;
    }
}