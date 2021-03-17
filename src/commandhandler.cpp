#include "../includes/commandhandler.hpp"
#include "../includes/command.hpp"
#include "../includes/commands/pingcommand.hpp"
#include "../includes/commands/changeprefix.hpp"
#include "../includes/commands/lurkcommand.hpp"
#include "../includes/commands/helpcommand.hpp"
#include "../includes/commands/editresult.hpp"
#include "../includes/commands/addtimer.hpp"

CommandHandler::CommandHandler(Bot *_bot) : bot{_bot} {
    init_command_list();
}

CommandHandler::~CommandHandler() {
    for(auto it = available_commands.begin(); it != available_commands.end(); it++)
        available_commands.erase(it);
}

void CommandHandler::init_command_list() {
    // TODO: add a Help/Commands command, add a ChangePrefix command
    available_commands.push_back(new PingCommand(bot));
    available_commands.push_back(new ChangePrefixCommand(bot));
    available_commands.push_back(new LurkCommand(bot));
    available_commands.push_back(new AddTimerCommand(bot));
    available_commands.push_back(new EditresultCommand(available_commands, bot));
    available_commands.push_back(new HelpCommand(available_commands, bot));
}

void CommandHandler::search_command(std::string command, bool mod, bool sub, std::string sender, std::string original_message, std::string channel) {
    for(auto &_command : available_commands) {
        if(_command->find_name(command)) {
            if(_command->has_perms_to_run(mod, sub, sender))
                _command->execute(sender, original_message, mod, sub, channel);
            else
                bot->send_chat_message(sender + " you dont have the permissions to run this command.", channel);
            return;
        }
    }
    bot->send_chat_message(sender + " I do not recognise this command. Try !help to see a list of the commands.", channel);
}