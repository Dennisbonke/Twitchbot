#include "../includes/bot.hpp"
#include "../includes/parser.hpp"
#include "../includes/commandhandler.hpp"
#include <iostream>

Bot::Bot(std::string _username, std::vector<std::string> _channels, std::string prefix, sockpp::tcp_connector *_conn) 
    : username{_username}, channels{_channels}, conn{_conn}, owner{_username} {
        for(auto &__channels : _channels) {
            prefixes.insert(std::pair<std::string, std::string>(__channels, prefix));
            commandhandlers.insert(std::pair<std::string, CommandHandler *>(__channels, new CommandHandler(this)));
        }
        parser = new Parser(this);
    }

Bot::~Bot() {
    delete parser;
}

void Bot::log_in(std::string password) {
    std::string pass_msg, nick_msg, user_msg, join_msg;

    pass_msg = "PASS ";
    pass_msg.append(password);
    send_server_message(pass_msg);

    nick_msg = "NICK ";
    nick_msg.append(username);
    send_server_message(nick_msg);

    user_msg = "USER ";
    user_msg.append(username);
    send_server_message(user_msg);

    send_server_message("CAP REQ : twitch.tv/membership");
    send_server_message("CAP REQ : twitch.tv/tags");
    send_server_message("CAP REQ : twitch.tv/commands");

    for(auto &channel : channels) {
        join_msg = "JOIN #";
        join_msg.append(channel);
        send_server_message(join_msg);
        send_chat_message("Joined this channel to help the streamer out", channel);
    }
}

void Bot::log_out() {
    for(auto &channel : channels) {
        std::string part_msg = "PART #";
        part_msg.append(channel);
        send_server_message(part_msg);
    }
    send_server_message("QUIT :Bot is shutting down");
}

void Bot::run() {
    std::string message_buffer;

    // send_chat_message("Hi Chat, its Westlanderz chatbot right here talking....");
    while(true) {
        char buffer[513] = {};
        conn->read(buffer, sizeof(buffer) - 1);
        message_buffer.append(buffer);
        // TODO: make this a async call
        process_messages(&message_buffer);
        message_buffer.erase();
    }
}

void Bot::send_chat_message(const std::string &msg, const std::string &channel) {
    std::string send_msg{"PRIVMSG #"};
    send_msg.append(channel);
    send_msg.append(" :");
    send_msg.append(msg);
    send_server_message(send_msg);
}

void Bot::send_server_message(const std::string &msg) {
    std::string server_msg{msg};
    server_msg.append("\r\n");

    if(conn->write_n(server_msg.c_str(), server_msg.length()) == -1) {
        std::cerr << "sending message to the server failed\r\n"
        << server_msg << std::endl;
    }
}

void Bot::process_messages(std::string *msg) {
    while (true) {
        std::size_t lineBreakPos = msg->find("\r\n");
        if (lineBreakPos != std::string::npos) {
            std::string currLine(msg->substr(0, lineBreakPos));
            std::cout << currLine << std::endl;
            msg->erase(0, lineBreakPos + 2);
            parser->parse_server_message(currLine);
        } else
            break;
    }
}

std::string Bot::is_username() {
    return username;
}

bool Bot::is_channel(const std::string &channel) {
    // TODO: better check
    for(auto &_channel : channels) {
        if(!strcmp(_channel.c_str(), channel.c_str()))
            return  true;
    }
    return false;
}

bool Bot::is_owner(const std::string &_owner) {
    if(!strcmp(_owner.c_str(), owner.c_str()))
        return true;
    return false;
}

std::string Bot::is_prefix(const std::string &channel) {
    return prefixes.at(channel);
}

void Bot::new_prefix(const std::string &new_prefix, const std::string &channel) {
    prefixes.at(channel) = new_prefix;
}

CommandHandler * Bot::is_commandhandler(const std::string &channel) {
    for(auto const& [key, val] : commandhandlers) {
        if(!strcmp(key.c_str(), channel.c_str()))
            return val;
    }
    return nullptr;
}
