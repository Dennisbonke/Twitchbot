#include "../paramparser.hpp"
#include <cstring>

ParamParser::ParamParser() {
    params.push_back("sender");
    params.push_back("user");
    params.push_back("count");
}

ParamParser::~ParamParser() {}

std::string ParamParser::parse_command(std::string result, std::string sender) {
    std::string new_result{""};
    std::size_t find_param_result = result.find("${sender}");
    if(find_param_result != std::string::npos) {
        new_result.append(result.substr(0, find_param_result));
        new_result.append(sender);
    }
}

bool ParamParser::check_good_param(std::string param) {
    for(auto &_param : params) {
        if(!strcmp(_param.c_str(), param.c_str()))
            return true;
    }
    return false;
}
