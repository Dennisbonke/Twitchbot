#ifndef _parser_h
#define _parser_h

#include "bot.hpp"

class Parser {
    public:
        Parser(Bot *);
        virtual ~Parser();

        void parse_server_message(std::string);

    private:
        std::string sender;
        bool mod;
        bool sub;
        std::string message;
        std::string command;
        Bot *bot;
};

#endif //_parser_h
