#include "../includes/parser.hpp"
#include "../includes/commandhandler.hpp"
#include <cstring>

/**
 * @brief Construct a new Parser:: Parser object
 * 
 * @param _bot a bot instance to send chat messages
 */
Parser::Parser(Bot *_bot) : bot{_bot} {}

/**
 * @brief Destroy the Parser:: Parser object
 * 
 */
Parser::~Parser() {}

/**
 * @brief Parse the message that was recieved by the server at the conn
 * 
 * @param server_message the messaage recieved by the server and needed to parse
 */
void Parser::parse_server_message(std::string server_message) {
    std::size_t find_ping = server_message.find("PING");
    if(find_ping != std::string::npos) {
        bot->send_server_message("PONG :tmi.twitch.tv");
        return;
    }
    std::size_t find_msg = server_message.find("PRIVMSG");
    if(find_msg != std::string::npos) {
        std::size_t find_mod = server_message.find("mod=");
        // TODO: fix because founders are not subs now
        std::size_t find_sub = server_message.find("subscriber/");
        std::size_t find_founder = server_message.find("founder/");
        std::size_t find_sender = server_message.find("display-name=");
        if(find_mod != std::string::npos) {
            if(!strcmp(server_message.substr(find_mod, find_mod + 5).c_str(), "mod=1"))
                mod = true;
            else
                mod = false;
        }
        if(find_sub != std::string::npos) {
            if(!strcmp(server_message.substr(find_sub, find_sub + 12).c_str(), "subscriber/0"))
                sub = false;
            else
                sub = true;
        }
        if(find_founder != std::string::npos) {
            if(!strcmp(server_message.substr(find_founder, find_founder + 9).c_str(), "founder/0"))
                sub = false;
            else
                sub = true;
        }
        if(find_sender != std::string::npos) {
            sender = server_message.substr(find_sender + 13, server_message.find(";", find_sender) - (find_sender + 13));
        }
        std::size_t find_channel_name = server_message.find_first_of(":", find_msg);
        std::string channel = server_message.substr(find_msg + 9, find_channel_name - find_msg - 10);
        message = server_message.substr(find_channel_name + 1);
        if(message.starts_with(bot->is_prefix(channel))){
            bot->is_commandhandler(channel)->search_command(message.substr(1, message.find_first_of(" ") - 1), mod, sub, sender, message, channel);
        } else if(!strcmp(message.c_str(), "prefix"))
            bot->send_chat_message("The prefix for this bot is " + bot->is_prefix(channel), channel);
    }
}
