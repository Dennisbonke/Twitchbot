#ifndef _bot_h
#define _bot_h

#include <string>
#include <vector>
#include <map>
#include <sockpp/tcp_connector.h>

class Bot {
    public:
        Bot(std::string, std::vector<std::string>, std::string, sockpp::tcp_connector *);
        virtual ~Bot();
        
        void log_in(std::string);
        void log_out();
        void run();
        void send_chat_message(const std::string &, const std::string &);
        void send_server_message(const std::string &);
        void process_messages(std::string &);
        std::string is_username();
        std::vector<std::string> is_channel();
        std::string is_prefix(const std::string &);
        void new_prefix(const std::string &, const std::string &);

    private:
        std::string username;
        std::vector<std::string> channels;
        std::map<std::string, std::string> prefixes;
        sockpp::tcp_connector *conn;
};

#endif //_bot_h
