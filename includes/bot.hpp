#ifndef _bot_h
#define _bot_h

#include <string>
#include <vector>
#include <map>
#include <sockpp/tcp_connector.h>
#include <async/result.hpp>

class Parser;
class CommandHandler;
class TimerHandler;

class Bot {
    public:
        Bot(std::string, std::vector<std::string>, std::string, sockpp::tcp_connector *);
        virtual ~Bot();
        
        void log_in(std::string);
        void log_out();
        void run();
        void send_chat_message(const std::string &, const std::string &);
        void send_server_message(const std::string &);
        async::result<void> process_messages(std::string &);
        async::result<void> check_timers();
        std::string is_username();
        bool is_channel(const std::string &);
        bool is_owner(const std::string &);
        std::string is_prefix(const std::string &);
        std::string is_timer_file(const std::string &);
        void new_prefix(const std::string &, const std::string &);
        CommandHandler * is_commandhandler(const std::string &);
        TimerHandler * is_timerhandler(const std::string &);

    private:
        std::string username;
        std::vector<std::string> channels;
        std::map<std::string, std::string> prefixes;
        std::map<std::string, CommandHandler *> commandhandlers;
        std::map<std::string, TimerHandler *> timerhandlers;
        sockpp::tcp_connector *conn;
        Parser *parser;
        std::string owner;
};

#endif //_bot_h
