#ifndef _change_prefix_h
#define _change_prefix_h

#include "../command.hpp"

class ChangePrefixCommand : public Command {
    public:
        ChangePrefixCommand(Bot *);

        void execute(std::string, std::string, bool, bool) override;
        bool has_perms_to_run(bool, bool, std::string) override;
        bool find_name(std::string) override;
        std::string list_command() override;
        std::string generate_help_message() override;

    private:
        std::vector<std::string> names;
        bool sub_only;
        bool mod_only;
        Bot *bot;
};

#endif
