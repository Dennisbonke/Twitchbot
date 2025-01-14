#ifndef _help_h
#define _help_h

#include "../command.hpp"

class HelpCommand : public Command {
    public:
        HelpCommand(std::vector<Command *>, Bot *);

        void execute(std::string, std::string, bool, bool, std::string) override;
        bool has_perms_to_run(bool, bool, std::string) override;
        bool find_name(std::string) override;
        std::string list_command() override;
        std::string generate_help_message() override;

    private:
        std::vector<std::string> names;
        bool sub_only;
        bool mod_only;
        std::vector<Command *> commands;
        Bot *bot;
};

#endif
