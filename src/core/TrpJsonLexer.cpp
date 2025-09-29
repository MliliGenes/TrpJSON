#include "../../include/core/TrpJsonLexer.hpp"

TrpJsonLexer::TrpJsonLexer(std::string _file_name) 
    : file_name(_file_name), has_next_line(false), current_line(""), line(0), col(0) {
    json_file.open(file_name.c_str(), std::ios::in);
    if (!json_file.is_open()) {
        std::cerr << "Error: Failed to open file: " << file_name << std::endl;
        return;
    }
    std::getline(json_file, current_line);
    current = current_line.begin();
    line_end = current_line.end();
    has_next_line = static_cast<bool>(std::getline(json_file, next_line));
}

TrpJsonLexer::~TrpJsonLexer(void) {
    if (json_file.is_open()) {
        json_file.close();
    }
}

bool TrpJsonLexer::isOpen( void ) {
    return json_file.is_open();
}

bool TrpJsonLexer::loadNextLineIfNeeded() {
    if (isAtEndOfLine()) {
        if (has_next_line) {
            current_line = next_line;
            has_next_line = static_cast<bool>(std::getline(json_file, next_line));
            line++;
            col = 0;
            current = current_line.begin();
            line_end = current_line.end();
            return true;
        } else {
            return false;
        }
    }
    return true;
}

void TrpJsonLexer::skipWhitespace() {
    while (true) {
        if (!loadNextLineIfNeeded()) {
            return;
        }
        
        while (current != line_end) {
            char c = *current;
            if (c == ' ' || c == '\t' || c == '\r') {
                advanceLexer();
            } else {
                return;
            }
        }
    }
}

char TrpJsonLexer::peekChar() const {
    if (current == line_end) {
        if (has_next_line) {
            return '\n';
        } else {
            return '\0';
        }
    }
    return *current;
}

char TrpJsonLexer::getChar() {
    char c = peekChar();
    if (c != '\0') {
        advanceLexer();
    }
    return c;
}

void TrpJsonLexer::advanceLexer() {
    if (current != line_end) {
        ++current;
        ++col;
    }
}

void TrpJsonLexer::pushBackLexer() {
    if (current != current_line.begin()) {
        --current;
        --col;
    }
}

bool TrpJsonLexer::isAtEndOfLine() const {
    return current == line_end;
}

bool TrpJsonLexer::isAtEnd() const {
    return !has_next_line && isAtEndOfLine();
}

token TrpJsonLexer::createErrorToken(const std::string& message) {
    token t;
    t.type = T_ERROR;
    t.value = message;
    t.line = line;
    t.col = col;
    return t;
}

token TrpJsonLexer::readString() {
    token t;
    t.type = T_STRING;
    t.line = line;
    t.col = col - 1;
    
    std::string value;
    bool escaped = false;
    
    while (true) {
        if (isAtEndOfLine()) {
                return createErrorToken("Invalid unescaped newline in string");
        }
        
        char c = getChar();
        
        if (c == '\0') {
            return createErrorToken("Unterminated string at end of file");
        }
        
        if (escaped) {
            switch (c) {
                case '"': value += '"'; break;
                case '\\': value += '\\'; break;
                case '/': value += '/'; break;
                case 'b': value += '\b'; break;
                case 'f': value += '\f'; break;
                case 'n': value += '\n'; break;
                case 'r': value += '\r'; break;
                case 't': value += '\t'; break;
                default:
                    std::string error_msg = "Invalid escape sequence: \\";
                    error_msg += c;
                    return createErrorToken(error_msg);
            }
            escaped = false;
        } else if (c == '\\') {
            escaped = true;
        } else if (c == '"') {
            break;
        } else {
            value += c;
        }
    }
    
    t.value = value;
    return t;
}

token TrpJsonLexer::readNumber() {
    token t;
    t.type = T_NUMBER;
    t.line = line;
    t.col = col - 1;
    
    std::string value;
    pushBackLexer();
    char first_digit = *current;
    advanceLexer();
    value += first_digit;
    
    if (first_digit == '0' && !isAtEndOfLine() && isdigit(peekChar())) {
        return createErrorToken("Invalid number format: leading zeros are not allowed");
    }
    
    bool has_decimal = false;
    bool has_exponent = false;
    bool has_digit_after_decimal = false;
    bool has_digit_after_exponent = false;
    
    while (!isAtEndOfLine()) {
        char c = peekChar();
        
        if (isdigit(c)) {
            value += getChar();
            if (has_decimal && !has_digit_after_decimal) {
                has_digit_after_decimal = true;
            }
            if (has_exponent && !has_digit_after_exponent) {
                has_digit_after_exponent = true;
            }
        } else if (c == '.' && !has_decimal && !has_exponent) {
            has_decimal = true;
            value += getChar();
        } else if ((c == 'e' || c == 'E') && !has_exponent) {
            has_exponent = true;
            value += getChar();
            
            if (!isAtEndOfLine()) {
                c = peekChar();
                if (c == '+' || c == '-') {
                    value += getChar();
                }
            }
        } else {
            break;
        }
    }
    
    if (has_decimal && !has_digit_after_decimal) {
        return createErrorToken("Invalid number format: decimal point must be followed by at least one digit");
    }
    
    if (has_exponent && !has_digit_after_exponent) {
        return createErrorToken("Invalid number format: exponent must be followed by at least one digit");
    }
    
    t.value = value;
    return t;
}

token TrpJsonLexer::readLiteral() {
    size_t start_col = col - 1;
    pushBackLexer();
    
    std::string literal;
    literal += *current;
    advanceLexer();
    
    while (!isAtEndOfLine() && isalpha(peekChar())) {
        literal += getChar();
    }
    
    token t;
    t.line = line;
    t.col = start_col;
    
    if (literal == "true") {
        t.type = T_TRUE;
    } else if (literal == "false") {
        t.type = T_FALSE;
    } else if (literal == "null") {
        t.type = T_NULL;
    } else {
        std::string error_msg = "Invalid literal: ";
        error_msg += literal;
        return createErrorToken(error_msg);
    }
    
    if (t.type != T_ERROR && !isAtEnd()) {
        char next = peekChar();
        if (isalnum(next) || next == '_') {
            std::string error_msg = "Invalid token: ";
            error_msg += literal;
            error_msg += next;
            return createErrorToken(error_msg);
        }
    }
    
    return t;
}

token TrpJsonLexer::getNextToken() {
    skipWhitespace();
    
    token t;
    t.line = line;
    t.col = col;
    
    if (isAtEnd()) {
        t.type = T_END_OF_FILE;
        return t;
    }
    
    char c = getChar();
    
    switch (c) {
        case '\0':
            t.type = T_END_OF_FILE;
            break;
            
        case '{':
            t.type = T_BRACE_OPEN;
            break;
            
        case '}':
            t.type = T_BRACE_CLOSE;
            break;
            
        case '[':
            t.type = T_BRACKET_OPEN;
            break;
            
        case ']':
            t.type = T_BRACKET_CLOSE;
            break;
            
        case ':':
            t.type = T_COLON;
            break;
            
        case ',':
            t.type = T_COMMA;
            break;
            
        case '"':
            return readString();
            
        case '-':
        case '0': case '1': case '2': case '3': case '4':
        case '5': case '6': case '7': case '8': case '9':
            return readNumber();
            
        case 't': case 'f': case 'n':
            return readLiteral();
            
        default:
            // Handle unexpected characters
            std::string error_msg = "Unexpected character: ";
            error_msg += c;
            return createErrorToken(error_msg);
    }
    
    return t;
}

