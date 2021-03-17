#include "../../includes/commands/helpcommand.hpp"

HelpCommand::HelpCommand(std::vector<Command *> _commands, Bot *_bot) : Command(), sub_only{false}, mod_only{false}, commands{_commands}, bot{_bot} {
    names.push_back("commands");
    names.push_back("help");
}

void HelpCommand::execute(std::string sender, std::string original_msg, bool mod, bool sub, std::string channel) {
    std::vector<Command *> allowed_commands;
    for(auto &command : commands) {
        if(command->has_perms_to_run(mod, sub, sender))
            allowed_commands.push_back(command);
    }
    std::size_t find_args = original_msg.find_first_of(" ");
    std::size_t find_command = original_msg.find_first_of(" ", find_args + 1);
    std::string command_name = "";
    if(find_args != std::string::npos) {
        if(find_command != std::string::npos) {
            command_name = original_msg.substr(find_args, find_command - find_args);
        } else {
            command_name = original_msg.substr(find_args);
        }
        for(auto &command : allowed_commands) {
            if(!strcmp(command->list_command().c_str(), command_name.c_str()))
                bot->send_chat_message(command->generate_help_message(), channel);
        }
    } else {
        std::string help_msg = "here is a list of commands you can run: ";
        for(auto &command : allowed_commands) {
            help_msg.append(command->list_command());
            help_msg.append(", ");
        }
        help_msg.append(". Try !help " + names[0] + " to learn more about one of them. Version: 1.0.0 https://github.com/Westlanderz/TwitchBot");
        bot->send_chat_message(help_msg, channel);
    }
}

bool HelpCommand::has_perms_to_run(bool mod, bool sub, std::string sender) {
    if(mod_only) {
        if(mod)
            return true;
        else
            return false; 
    } else if(sub_only) {
        if(sub || mod)
            return true;
        else
            return false; 
    } else
        return true;
}

bool HelpCommand::find_name(std::string command_name) {
    for(auto it = names.begin(); it != names.end(); it++) {
        if(!strcmp(it->data(), command_name.c_str()))
            return true;
    }
    return false;
}

std::string HelpCommand::list_command() {
    return names[0];
}

std::string HelpCommand::generate_help_message() {
    return "I think you know how to use this command.....";
}