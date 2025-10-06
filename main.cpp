#include <string>
#include "lib/TrpJson.hpp"

void testParser(const std::string& filename) {
    TrpJsonParser parser(filename);
    
    if (parser.parse())
        parser.prettyPrint();
}

int main(int ac, char **av) {
    if (ac != 2) return 1;

    const std::string validTestFile = av[1];
    testParser(validTestFile);
    
    return 0;
}
