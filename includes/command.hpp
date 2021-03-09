#ifndef _command_h
#define _command_h

#include "bot.hpp"

class Command {
    public:
        Command() = default;
        virtual ~Command() = default;

        virtual void execute(std::string, std::string, bool, bool, std::string) = 0;
        virtual bool has_perms_to_run(bool, bool, std::string) = 0;
        virtual bool find_name(std::string) = 0;
        virtual std::string list_command() = 0;
        virtual std::string generate_help_message(const std::string &) = 0;
        virtual void new_output(std::string) = 0;
};

#endif //_command_h
