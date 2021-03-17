#include "../../includes/commands/lurkcommand.hpp"

LurkCommand::LurkCommand(Bot *_bot) : Command(), sub_only{false}, mod_only{false}, bot{_bot} {
    names.push_back("lurk");
}

void LurkCommand::execute(std::string sender, std::string original_msg, bool mod, bool sub, std::string channel) {
    bot->send_chat_message("A lurk a day will keep twitch at bay. " + sender + " many thanks for the lurk!", channel);
}

bool LurkCommand::has_perms_to_run(bool mod, bool sub, std::string sender) {
    if(mod_only) {
        if(mod)
            return true;
        else
            return false; 
    } else if(sub_only) {
        if(sub || mod) // || sender == bot->is_channel()
            return true;
        else
            return false; 
    } else
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