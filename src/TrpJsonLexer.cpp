#include "../include/TrpJsonLexer.hpp"

TrpJsonLexer::TrpJsonLexer( std::string _file_name ) : file_name(_file_name), line(1), col(1) {
    json_file.open(file_name);
    opened = json_file.is_open();
}