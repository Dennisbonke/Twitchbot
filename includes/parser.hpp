#ifndef _parser_h
#define _parser_h

#include "bot.hpp"

class Parser {
    public:
        Parser(std::string, Bot *);
        virtual ~Parser();

        bool is_command();
        bool is_ping_message();
        void parse_server_message(std::string);
        std::string is_sender();
        std::string server_command();

    private:
        std::string server_message;
        std::string sender;
        bool mod;
        bool sub;
        std::string message;
        std::string command;
        Bot *bot;
};

#endif //_parser_h
