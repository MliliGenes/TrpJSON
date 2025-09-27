#include <iostream>
#include <fstream>
#include <string>
#include "include/TrpJsonLexer.hpp"

// Helper function to convert token type to string for display
std::string tokenTypeToString(TrpTokenType type) {
    switch (type) {
        case T_BRACE_OPEN: return "BRACE_OPEN";
        case T_BRACE_CLOSE: return "BRACE_CLOSE";
        case T_BRACKET_OPEN: return "BRACKET_OPEN";
        case T_BRACKET_CLOSE: return "BRACKET_CLOSE";
        case T_COLON: return "COLON";
        case T_COMMA: return "COMMA";
        case T_STRING: return "STRING";
        case T_NUMBER: return "NUMBER";
        case T_TRUE: return "TRUE";
        case T_FALSE: return "FALSE";
        case T_NULL: return "NULL";
        case T_END_OF_FILE: return "EOF";
        case T_ERROR: return "ERROR";
        default: return "UNKNOWN";
    }
}

// Test the lexer with the given file
void testLexer(const std::string& filename) {
    std::cout << "\nTesting lexer with file: " << filename << std::endl;
    std::cout << "----------------------------------------\n";
    
    TrpJsonLexer lexer(filename);
    token t;
    
    do {
        t = lexer.getNextToken();
        
        std::cout << tokenTypeToString(t.type) << " (line " << t.line << ", col " << t.col << ")";
        
        if (t.type == T_STRING || t.type == T_NUMBER) {
            std::cout << ": " << t.value;
        } else if (t.type == T_ERROR) {
            std::cout << ": ERROR - " << t.value;
            std::cout << "\n  at position: line " << t.line << ", column " << t.col;
        }
        
        std::cout << std::endl;
        
    } while (t.type != T_END_OF_FILE && t.type != T_ERROR);
}

int main() {
    std::cout << "TrpJSON Lexer Test\n";
    std::cout << "=================\n";
    
    // Create test files
    const std::string validTestFile = "test_valid.json";
    const std::string errorTestFile = "test_errors.json";
    
    std::cout << "\n===== TESTING VALID JSON =====";
    testLexer(validTestFile);
    
    std::cout << "\n===== TESTING ERROR DETECTION =====";
    testLexer(errorTestFile);
    
    std::cout << "\nLexer tests completed.\n";
    return 0;
}
