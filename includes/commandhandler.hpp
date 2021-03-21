#ifndef _chandler_h
#define _chandler_h

#include "command.hpp"
#include "bot.hpp"

class CommandHandler {
    public:
        CommandHandler(Bot *);
        virtual ~CommandHandler();

        void init_command_list();
        void search_command(std::string, bool, bool, std::string, std::string, std::string);

    private:
        std::vector<Command *> available_commands;
        Bot *bot;
};

#endif //_chandler_h
