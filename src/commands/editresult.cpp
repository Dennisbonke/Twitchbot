#include "../../includes/commands/editresult.hpp"

EditresultCommand::EditresultCommand(std::vector<Command *> _commands, Bot *_bot) : Command(), commands{_commands}, bot{_bot} {
    names.push_back("editcommand");
    names.push_back("edit");
}

void EditresultCommand::execute(std::string sender, std::string original_msg, bool, bool, std::string channel) {
    std::size_t find_command = original_msg.find(" ");
    if(find_command != std::string::npos) {
        std::string commandname{""};
        std::size_t find_change = original_msg.find(" ", find_command + 1);
        if(find_change != std::string::npos) {
            for(auto &_command : commands) {
                if(_command->find_name(original_msg.substr(find_command + 1, find_change - find_command - 1))) {
                    std::size_t find_result = original_msg.find(" ", find_change);
                    if(find_result != std::string::npos) {
                        std::string result = original_msg.substr(find_result + 1);
                        _command->new_output(result);
                        std::string new_result = sender;
                        new_result.append(" i changed the output for ");
                        new_result.append(_command->list_command());
                        new_result.append(" for you.");
                        bot->send_chat_message(new_result, channel);
                    } else {
                        bot->send_chat_message("Please provide a new result to change to.", channel);
                    }
                    return;
                }
            }
            bot->send_chat_message("I dont know that command, so i cant edit it.", channel);
        }
    } else {
        bot->send_chat_message("Please provide a command to change the result from.", channel);
    }
}

bool EditresultCommand::has_perms_to_run(bool mod, bool, std::string sender) {
    if(mod || bot->is_channel(sender) || bot->is_owner(sender))
        return true;
    return false;
}

bool EditresultCommand::find_name(std::string command_name) {
    for(auto it = names.begin(); it != names.end(); it++) {
        if(!strcmp(it->data(), command_name.c_str()))
            return true;
    }
    return false;
}

std::string EditresultCommand::list_command() {
    return names[0];
}

std::string EditresultCommand::generate_help_message(const std::string &channel) {
    return "Use " + bot->is_prefix(channel) + names[0] + " [command] [new-result] to change the output of that command.";
}

void EditresultCommand::new_output(std::string) {}
