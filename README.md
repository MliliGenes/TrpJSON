# TrpJSON

A lightweight, C++98 compatible JSON parsing library with a clean object-oriented design.

## Overview

TrpJSON is a JSON parsing library designed for C++98 compatibility, featuring a lexer-parser architecture with proper memory management through RAII smart pointers. The library provides a complete implementation of JSON parsing with support for all JSON data types.

## Features

- **C++98 Compatible**: Works with legacy compilers and systems
- **Memory Safe**: RAII-based memory management with AutoPointer template
- **Complete JSON Support**: Objects, arrays, strings, numbers, booleans, and null values
- **Error Reporting**: Detailed error messages with line and column information
- **Lexer-Parser Architecture**: Clean separation of tokenization and parsing logic
- **Static Library**: Can be compiled as a static library for easy integration

## Architecture

The library follows a modular design with three main components:

### Core Components

- **ITrpJsonValue**: Base interface for all JSON value types
- **TrpJsonType**: Enumeration defining JSON value types
- **TrpJsonLexer**: Tokenizes JSON input from files
- **AutoPointer<T>**: RAII smart pointer template for C++98

### Value Types

- **TrpJsonObject**: JSON object implementation using std::map
- **TrpJsonArray**: JSON array implementation using std::vector
- **TrpJsonString**: JSON string value
- **TrpJsonNumber**: JSON number value (double precision)
- **TrpJsonBool**: JSON boolean value
- **TrpJsonNull**: JSON null value

### Parser

- **TrpJsonParser**: Main parser class that generates Abstract Syntax Tree (AST)
## API Reference

### TrpJsonParser

The main parser class for processing JSON files.

#### Constructor
```cpp
TrpJsonParser();                           // Default constructor
TrpJsonParser(const std::string _file_nmae); // Constructor with file
```

#### Core Methods
```cpp
bool parse();                              // Parse loaded JSON file
ITrpJsonValue* getAST() const;             // Get parsed Abstract Syntax Tree
ITrpJsonValue* release();                  // Release ownership of AST
void reset();                              // Reset parser state
```

#### State Management
```cpp
bool isParsed() const;                     // Check if parsing completed
const token& getLastError() const;         // Get last parsing error
void lastError() const;                    // Print last error details
void clearAST();                           // Clear current AST
```

#### Output Methods
```cpp
std::string astToString() const;           // Convert AST to string
void prettyPrint() const;                  // Pretty-print parsed JSON
```

#### Lexer Management
```cpp
void setLexer(TrpJsonLexer* _lexer);       // Set lexer instance (calls resetLexer)
void resetLexer(TrpJsonLexer* new_lexer);  // Replace lexer instance
```

### TrpJsonLexer

Low-level tokenizer for JSON input.

#### Constructor
```cpp
TrpJsonLexer(std::string file_name);       // Constructor with file
```

#### Methods
```cpp
token getNextToken();                      // Get next token from input
bool isOpen();                             // Check if file is open
const std::string getFileName() const;     // Get current filename
void reset();                              // Reset lexer state
```

### Value Types API

#### TrpJsonObject
```cpp
void add(std::string key, ITrpJsonValue* value);  // Add key-value pair
ITrpJsonValue* find(std::string key);             // Find value by key
JsonObjectMap::const_iterator begin() const;     // Iterator begin
JsonObjectMap::const_iterator end() const;       // Iterator end
size_t size() const;                              // Get object size
```

#### TrpJsonArray
```cpp
void add(ITrpJsonValue* value);            // Add element to array
ITrpJsonValue* at(size_t index);           // Access element by index
size_t size() const;                       // Get array size
```

#### TrpJsonString
```cpp
const std::string& getValue() const;       // Get string value
```

#### TrpJsonNumber
```cpp
const double& getValue() const;            // Get numeric value
```

#### TrpJsonBool
```cpp
const bool& getValue() const;              // Get boolean value
```

### AutoPointer<T>

RAII smart pointer for automatic memory management.

```cpp
AutoPointer(T* ptr = NULL);                // Constructor
bool isNULL() const;                       // Check if pointer is null
T* get() const;                            // Get raw pointer
T* release();                              // Release ownership
void reset(T* ptr);                        // Reset with new pointer
T& operator*() const;                      // Dereference operator
T* operator->() const;                     // Arrow operator
```

## Usage Example

### Basic Parsing

```cpp
#include "include/parser/TrpJsonParser.hpp"
#include "include/core/TrpAutoPointer.hpp"

int main() {
    // Create parser with JSON file
    TrpJsonParser parser("data.json");
    
    // Parse the file
    if (parser.parse()) {
        // Get the AST
        ITrpJsonValue* ast = parser.getAST();
        
        // Pretty print the result
        parser.prettyPrint();
        
        // Convert to string
        std::string jsonString = parser.astToString();
        std::cout << jsonString << std::endl;
    } else {
        // Handle parsing error
        parser.lastError();
    }
    
    return 0;
}
```

### Working with Parsed Values

```cpp
// Assuming we have a parsed object
ITrpJsonValue* ast = parser.getAST();

// Check type and cast
if (ast->getType() == TRP_OBJECT) {
    TrpJsonObject* obj = static_cast<TrpJsonObject*>(ast);
    
    // Find a value
    ITrpJsonValue* nameValue = obj->find("name");
    if (nameValue && nameValue->getType() == TRP_STRING) {
        TrpJsonString* nameStr = static_cast<TrpJsonString*>(nameValue);
        std::cout << "Name: " << nameStr->getValue() << std::endl;
    }
}
```

### Using AutoPointer for Memory Management

```cpp
#include "include/core/TrpAutoPointer.hpp"

void safeParsingExample() {
    TrpJsonParser parser("data.json");
    
    if (parser.parse()) {
        // Use AutoPointer for automatic cleanup
        AutoPointer<ITrpJsonValue> ast(parser.release());
        
        // Use ast safely - memory will be cleaned up automatically
        if (!ast.isNULL()) {
            // Process the AST
            processJsonValue(ast.get());
        }
        // AutoPointer destructor automatically cleans up memory
    }
}
```

## Building

### Compile as Library

```bash
make lib
```

This creates `libtrpjson.a` static library.

### Using the Library

Include the library in your project:

```bash
g++ -std=c++98 -Iinclude your_file.cpp -L. -ltrpjson
```

### Manual Compilation

```bash
g++ -std=c++98 -Wall -Wextra -Iinclude src/*.cpp your_main.cpp
```

## JSON Type Mapping

| JSON Type | C++ Class      | Enum Value  |
|-----------|----------------|-------------|
| Object    | TrpJsonObject  | TRP_OBJECT  |
| Array     | TrpJsonArray   | TRP_ARRAY   |
| String    | TrpJsonString  | TRP_STRING  |
| Number    | TrpJsonNumber  | TRP_NUMBER  |
| Boolean   | TrpJsonBool    | TRP_BOOL    |
| Null      | TrpJsonNull    | TRP_NULL    |

## Token Types

The lexer recognizes the following token types:

- `T_BRACE_OPEN`, `T_BRACE_CLOSE`: `{` and `}`
- `T_BRACKET_OPEN`, `T_BRACKET_CLOSE`: `[` and `]`
- `T_COLON`, `T_COMMA`: `:` and `,`
- `T_STRING`, `T_NUMBER`: JSON string and number literals
- `T_TRUE`, `T_FALSE`, `T_NULL`: JSON literals
- `T_END_OF_FILE`, `T_ERROR`: End of input and error states

## Error Handling

The library provides detailed error information:

```cpp
if (!parser.parse()) {
    const token& error = parser.getLastError();
    std::cout << "Parse error at line " << error.line 
              << ", column " << error.col 
              << ": " << error.value << std::endl;
}
```

## Memory Management

The library uses RAII principles:

- All dynamically allocated objects are managed through the `AutoPointer<T>` class
- The parser automatically cleans up the AST when destroyed
- Use `parser.release()` to transfer ownership of the AST
- Always use `AutoPointer<T>` when taking ownership of parser results

## Requirements

- C++98 compatible compiler
- Standard C++ library (iostream, string, vector, map, fstream)
- Make (for building)

## License

This project uses the MIT License. See LICENSE file for details.