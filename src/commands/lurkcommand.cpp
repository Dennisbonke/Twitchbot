#include "../../includes/commands/lurkcommand.hpp"

LurkCommand::LurkCommand(Bot *_bot) : Command(), bot{_bot} {
    names.push_back("lurk");
}

void LurkCommand::execute(std::string sender, std::string original_msg, bool mod, bool sub, std::string channel) {
    bot->send_chat_message("A lurk a day will keep twitch at bay. " + sender + " many thanks for the lurk!", channel);
}

bool LurkCommand::has_perms_to_run(bool mod, bool sub, std::string sender) {
    return true;
}

bool LurkCommand::find_name(std::string command_name) {
    for(auto it = names.begin(); it != names.end(); it++) {
        if(!strcmp(it->data(), command_name.c_str()))
            return true;
    }
    return false;
}

std::string LurkCommand::list_command() {
    return names[0];
}

std::string LurkCommand::generate_help_message() {
    return "Use !" + names[0] + " let the streamer know that you are lurking.";
}

void LurkCommand::new_output(std::string _result) {
    result = _result;
}