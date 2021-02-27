#include <exception>
#include <iostream>
#include <string>
#include <sockpp/tcp_connector.h>
#include "includes/bot.hpp"

int main(void) {
    std::string host = "irc.chat.twitch.tv";
    in_port_t port = 6667;
    sockpp::tcp_connector *conn = new sockpp::tcp_connector(sockpp::inet_address(host, port));
    
    std::cout << "connected to " << sockpp::inet_address(host, port) << std::endl;
    
    Bot *bot = new Bot("westlanderz", "westlanderz", "!", conn);
    try{
        bot->log_in("oauth:76z1a4yp3xwt1rvf1x6yxysejwtjmb");
        bot->run();
    } catch(std::exception &e) {
        std::cerr << "Exception thrown in main\r\n" << e.what() << std::endl;
        bot->send_chat_message("Leaving channel");
        bot->log_out();
        exit(1);
    }
}
