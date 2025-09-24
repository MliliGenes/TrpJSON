# TrpJSON Parser - C++98 Implementation

A Comprehensive Guide to Building a TrpJSON Parser in C++98

This guide details a single-pass approach for building a robust TrpJSON parser. The parser directly interacts with the lexer to consume tokens one at a time, building the data structure on the fly. This design is highly memory-efficient, but requires careful attention to error handling and resource management.

## 1. TrpJSON Data Types

Instead of a union, this parser uses a polymorphic class hierarchy for type-safe data representation. The abstract base class `TrpValue` is the foundation, with derived classes for specific types. Tokens are identified by the `TrpTokenType` enum and stored in a `TrpToken` struct.

### Supported TrpJSON Types

| TrpType | Description |
|---------|-------------|
| TRP_NULL | Represents the null value. |
| TRP_BOOL | Represents boolean true or false. |
| TRP_NUMBER | Represents a numeric value (integer or float). |
| TRP_STRING | Represents a sequence of characters enclosed in double quotes. |
| TRP_ARRAY | An ordered list of other TrpJSON values. |
| TRP_OBJECT | An unordered collection of key-value pairs. |

```cpp
// Enum for the different types of TrpJSON values
enum TrpType {
    TRP_NULL,
    TRP_BOOL,
    TRP_NUMBER,
    TRP_STRING,
    TRP_ARRAY,
    TRP_OBJECT
};

// Abstract base class for all TrpJSON values
class TrpValue {
public:
    virtual ~TrpValue() {}
    virtual TrpType getType() const = 0;
};

// Represents a TrpJSON number
class TrpNumber : public TrpValue {
public:
    explicit TrpNumber(double value) : m_value(value) {}
    virtual TrpType getType() const { return TRP_NUMBER; }
    double getValue() const { return m_value; }
private:
    double m_value;
};

// Represents a TrpJSON string
class TrpString : public TrpValue {
public:
    explicit TrpString(const std::string& value) : m_value(value) {}
    virtual TrpType getType() const { return TRP_STRING; }
    const std::string& getValue() const { return m_value; }
private:
    std::string m_value;
};

// Represents a TrpJSON boolean
class TrpBool : public TrpValue {
public:
    explicit TrpBool(bool value) : m_value(value) {}
    virtual TrpType getType() const { return TRP_BOOL; }
    bool getValue() const { return m_value; }
private:
    bool m_value;
};

// Represents a TrpJSON null value
class TrpNull : public TrpValue {
public:
    virtual TrpType getType() const { return TRP_NULL; }
};

// Forward declaration for the container classes
class TrpObject;
class TrpArray;

// Represents a TrpJSON object
class TrpObject : public TrpValue {
public:
    virtual ~TrpObject();
    virtual TrpType getType() const { return TRP_OBJECT; }
    void add(const std::string& key, TrpValue* value);
private:
    std::map<std::string, TrpValue*> m_members;
};

// Represents a TrpJSON array
class TrpArray : public TrpValue {
public:
    virtual ~TrpArray();
    virtual TrpType getType() const { return TRP_ARRAY; }
    void add(TrpValue* value);
private:
    std::vector<TrpValue*> m_elements;
};

// Enum for the different types of tokens recognized by the lexer
enum TrpTokenType {
    T_BRACE_OPEN,       // {
    T_BRACE_CLOSE,      // }
    T_BRACKET_OPEN,     // [
    T_BRACKET_CLOSE,    // ]
    T_COLON,            // :
    T_COMMA,            // ,
    T_STRING,           // "..."
    T_NUMBER,           // 123, 3.14
    T_TRUE,             // true
    T_FALSE,            // false
    T_NULL,             // null
    T_END_OF_FILE,      // EOF
    T_ERROR
};

// Represents a single token from the input string
struct TrpToken {
    TrpTokenType type;
    std::string value; // Stores the string for T_STRING or T_NUMBER
};
```

## 2. The Lexer (TrpLexer)

The `TrpLexer` is a dedicated class that reads the input string and provides the parser with the next token. Its `getNextToken()` function skips whitespace and identifies tokens based on the current character, handling single-character tokens, strings, numbers, and keywords. It advances its internal index after each token, throwing an exception for any invalid characters.

## 3. The Parser (TrpParser)

The `TrpParser` is responsible for building the data structure. It uses a recursive descent approach, with the main `parse()` function calling a series of methods like `parseValue()`, `parseObject()`, and `parseArray()`. These functions consume tokens from the `TrpLexer` one at a time to construct the data tree. A custom smart pointer is used to manage dynamically allocated `TrpValue` objects, preventing memory leaks if an error occurs during parsing.

## 4. The Main Logic

The program's entry point instantiates a `TrpLexer` with the input string and a `TrpParser` that uses that lexer. The `parse()` method is called within a try-catch block to handle any parsing errors. The root object returned from the parse function holds the complete data tree.

## 5. Parser Workflow

The parser operates as a single-pass system, with control flowing between the lexer and parser. The process is as follows:

1. **Input String**: The `TrpLexer` takes the raw JSON string as input.
2. **Tokenization**: The `TrpLexer` reads the string character by character, identifying and returning the next token (`TrpToken`) to the parser.
3. **Recursive Parsing**: The `TrpParser`'s `parseValue()` method inspects the token to determine its type (`{`, `[`, `"` etc.).
   - If a complex type (like `{` or `[`) is found, `parseValue()` calls the appropriate parsing method (`parseObject()` or `parseArray()`).
   - These methods then loop, calling `parseValue()` for each nested element, creating the recursive flow.
4. **Data Tree Construction**: As the parser "descends" and then "ascends" the tree, it builds the `TrpValue` object hierarchy.
5. **Final Output**: The main `parse()` function returns a pointer to the root of the completed `TrpValue` data tree.

## 6. Robust Error Handling and Memory Management

A critical aspect of this C++98 parser is its robust memory management. When an exception is thrown, the stack unwinds, and any resources (like objects allocated with `new`) that have not yet been assigned to a container will be leaked. The key to solving this is the Resource Acquisition Is Initialization (RAII) idiom.

The solution is a simple RAII-style wrapper to manage a pointer in C++98. Since C++98 lacks `std::unique_ptr` and `std::shared_ptr`, this custom "smart pointer" class ensures that dynamically allocated objects are automatically deleted if an exception is thrown during parsing, preventing memory leaks. Ownership of an object is only "released" from the smart pointer when it is successfully added to its parent container (an array or object).

```cpp
// A simple RAII wrapper to manage a pointer in C++98
template <class T>
class AutoPtr {
public:
    explicit AutoPtr(T* ptr = 0) : m_ptr(ptr) {}
    ~AutoPtr() { delete m_ptr; }
    
    // Disable copying (C++98 style)
private:
    AutoPtr(const AutoPtr& other);
    AutoPtr& operator=(const AutoPtr& other);
public:

    // Operator to access the raw pointer
    T* operator->() const { return m_ptr; }
    T& operator*() const { return *m_ptr; }

    // Release ownership to the caller
    T* release() {
        T* temp = m_ptr;
        m_ptr = 0;
        return temp;
    }
private:
    T* m_ptr;
};
```

Now, you would use this `AutoPtr` in your parsing functions. The moment you `new` a `TrpValue`, you place it inside an `AutoPtr`. If an exception is thrown, the `AutoPtr` will be destroyed, and its destructor will delete the managed pointer, preventing the leak.

### Updated Pseudocode using AutoPtr:

```cpp
// ... inside TrpParser class ...
TrpValue* parseObject() {
    // ...
    // Create the object container with an AutoPtr for safety
    AutoPtr<TrpObject> object(new TrpObject());
    
    // ...
    // LOOP ...
    AutoPtr<TrpValue> value(parseValue()); // 'value' is now managed
    
    // If the next line throws, 'value' is automatically deleted
    object->add(key, value.release()); // Release ownership to the container
    
    // ...
    return object.release(); // Return ownership to the caller
}
```

This pattern ensures that every object created with `new` is immediately placed under the management of a stack-based RAII object. The ownership is only "released" when the object is successfully added to its parent container. This is a very powerful and robust technique for C++98.