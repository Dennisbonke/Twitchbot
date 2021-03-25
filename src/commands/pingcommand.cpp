#include "../../includes/commands/pingcommand.hpp"

PingCommand::PingCommand(Bot *_bot) : Command(), bot{_bot} {
    names.push_back("ping");
    names.push_back("pong");
    result = "This is the ping command ${sender}";
}

void PingCommand::execute(std::string sender, std::string, bool, bool, std::string channel) {
    std::string new_result{""};
    std::size_t find_param_result = result.find("${sender}");
    if(find_param_result != std::string::npos) {
        new_result.append(result.substr(0, find_param_result));
        new_result.append(sender);
    } else {
        new_result = result;
    }
    bot->send_chat_message(new_result, channel);
}

bool PingCommand::has_perms_to_run(bool, bool, std::string) {
    return true;
}

bool PingCommand::find_name(std::string command_name) {
    for(auto it = names.begin(); it != names.end(); it++) {
        if(!strcmp(it->data(), command_name.c_str()))
            return true;
    }
    return false;
}

std::string PingCommand::list_command() {
    return names[0];
}

std::string PingCommand::generate_help_message(const std::string &channel) {
    return "Use " + bot->is_prefix(channel) + names[0] + " test stuff out hahhaha, dont use normally.";
}

void PingCommand::new_output(std::string _result) {
    result = _result;
}
