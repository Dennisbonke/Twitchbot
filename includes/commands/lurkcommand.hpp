#ifndef _lurk_h
#define _lurk_h

#include "../command.hpp"

class LurkCommand : public Command {
    public:
        LurkCommand(Bot *);

        void execute(std::string, std::string, bool, bool, std::string) override;
        bool has_perms_to_run(bool, bool, std::string) override;
        bool find_name(std::string) override;
        std::string list_command() override;
        std::string generate_help_message(const std::string &) override;
        void new_output(std::string) override;

    private:
        std::vector<std::string> names;
        Bot *bot;
        std::string result;
};

#endif //_lurk_h