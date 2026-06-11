#include <string>
#include "lib/TrpJson.hpp"

void testParser(const std::string& filename) {
    TrpJsonParser parser(filename);
    
    if (parser.parse())
        parser.prettyPrint();
}

void testJsonString(const char *json_string) {

    TrpJsonLexer* lexer = new TrpJsonLexer(json_string, true );
    TrpJsonParser parser;

    parser.setLexer(lexer);
    if (parser.parse())
        parser.prettyPrint();

}

int main() {
    // if (ac != 2) return 1;

    // const std::string validTestFile = av[1];
    // testParser(validTestFile);

    const char* json =
"{\n"
"    \"name\": \"John\",\n"
"    \"age\": 42,\n"
"    \"active\": true,\n"
"    \"address\": {\n"
"        \"city\": \"Casablanca\",\n"
"        \"country\": \"Morocco\"\n"
"    },\n"
"    \"skills\": [\n"
"        \"C++\",\n"
"        \"JSON\",\n"
"        \"Parsing\"\n"
"    ]\n"
"}";


    testJsonString(json);
    
    return 0;
}
