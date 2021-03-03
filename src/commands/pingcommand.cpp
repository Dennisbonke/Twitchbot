#include "../../includes/commands/pingcommand.hpp"

PingCommand::PingCommand(Bot *_bot) : Command(), bot{_bot} {
    names.push_back("ping");
    names.push_back("pong");
}

void PingCommand::execute(std::string sender, std::string original_msg, bool mod, bool sub, std::string channel) {
    bot->send_chat_message("This is the ping command", channel);
}

bool PingCommand::has_perms_to_run(bool mod, bool sub, std::string sender) {
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

std::string PingCommand::generate_help_message() {
    return "Use !" + names[0] + " test stuff out hahhaha, dont use normally.";
}

void PingCommand::new_output(std::string _result) {
    result = _result;
}
