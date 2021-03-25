#include "../../includes/commands/changeprefix.hpp"

ChangePrefixCommand::ChangePrefixCommand(Bot *_bot) : Command(), bot{_bot} {
    names.push_back("changeprefix");
    names.push_back("change-prefix");
    names.push_back("change_prefix");
}

void ChangePrefixCommand::execute(std::string, std::string original_msg, bool, bool, std::string channel) {
    std::string new_prefix;
    std::size_t find_prefix = original_msg.find(" ");
    if(find_prefix != std::string::npos) {
        std::size_t prefix_end = original_msg.find(" ", find_prefix + 1);
        if(prefix_end != std::string::npos) {
            new_prefix = original_msg.substr(find_prefix + 1, prefix_end - find_prefix);
            bot->new_prefix(new_prefix, channel);
            bot->send_chat_message("Changed the prefix to " + new_prefix, channel);
        } else {
            new_prefix = original_msg.substr(find_prefix + 1);
            bot->new_prefix(new_prefix, channel);
            bot->send_chat_message("Changed the prefix to " + new_prefix, channel);
        }
    } else {
        bot->send_chat_message("Please provide a new prefix to change to", channel);
    }
}

bool ChangePrefixCommand::has_perms_to_run(bool, bool, std::string sender) {
    if(bot->is_channel(sender) || bot->is_owner(sender))
        return true;
    return false; 
}

bool ChangePrefixCommand::find_name(std::string command_name) {
    for(auto it = names.begin(); it != names.end(); it++) {
        if(!strcmp(it->data(), command_name.c_str()))
            return true;
    }
    return false;
}

std::string ChangePrefixCommand::list_command() {
    return names[0];
}

std::string ChangePrefixCommand::generate_help_message(const std::string &channel) {
    return "Use " + bot->is_prefix(channel) + names[0] + " [prefix] to change the prefix of this bot.";
}

void ChangePrefixCommand::new_output(std::string) {}