#ifndef _addtimer_h
#define _addtimer_h

#include "../command.hpp"

class AddTimerCommand : public Command {
    public:
        AddTimerCommand(Bot *);

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

#endif //_addtimer_h
