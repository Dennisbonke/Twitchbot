#ifndef _bot_h
#define _bot_h

#include <string>
#include <sockpp/tcp_connector.h>

class Bot {
    public:
        Bot(std::string, std::string, std::string, sockpp::tcp_connector *);
        virtual ~Bot();
        
        void log_in(std::string);
        void log_out();
        void run();
        void send_chat_message(const std::string &);
        void send_server_message(const std::string &);
        void process_messages(std::string &);
        std::string is_username();
        std::string is_channel();
        std::string is_prefix();
        void new_prefix(const std::string &);

    private:
        std::string username;
        std::string channel;
        std::string prefix;
        sockpp::tcp_connector *conn;
};

#endif //_bot_h
