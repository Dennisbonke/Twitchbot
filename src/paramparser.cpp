#include "../paramparser.hpp"

ParamParser::ParamParser() {
    params.push_back("sender");
    params.push_back("user");
    params.push_back("count");
}

ParamParser::~ParamParser() {}

std::string parse_command(std::string sender) {
    
}