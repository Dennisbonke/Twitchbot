#include <exception>
#include <iostream>
#include <string>
#include <sockpp/tcp_connector.h>
#include <fstream>
#include <vector>
#include "includes/bot.hpp"

int main(void) {
    std::string host = "irc.chat.twitch.tv";
    in_port_t port = 6667;
    sockpp::tcp_connector *conn = new sockpp::tcp_connector(sockpp::inet_address(host, port));
    
    std::cout << "connected to " << sockpp::inet_address(host, port) << std::endl;

    std::fstream config_file, channel_file;
    std::string oauthcode;
    std::vector<std::string> channels;
    config_file.open("config.txt", std::ios::in);
	if (!config_file) {
		std::cerr << "No such file" << std::endl;
	} else {
		char ch;

		while (1) {
			config_file >> ch;
			if (config_file.eof())
				break;
            oauthcode.push_back(ch);
		}
	}
	config_file.close();

    channels.push_back("westlanderz");
    channels.push_back("smurfingisbae");

    Bot *bot = new Bot("westlanderz", channels, "!", conn);

    try{
        bot->log_in(oauthcode);
        bot->run();
    } catch(std::exception &e) {
        std::cerr << "Exception thrown in main\r\n" << e.what() << std::endl;
        for(auto &channel : channels) {
            bot->send_chat_message("Leaving channel", channel);
        }
        bot->log_out();
        exit(1);
    }
}
