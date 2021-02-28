#include "../includes/bot.hpp"
#include "../includes/parser.hpp"
#include <iostream>

Bot::Bot(std::string _username, std::vector<std::string> _channels, std::string _prefix, sockpp::tcp_connector *_conn) 
    : username{_username}, channels{_channels}, prefix{_prefix}, conn{_conn} {}

Bot::~Bot() {}

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
        process_messages(message_buffer);
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

void Bot::process_messages(std::string &msg) {
    while (true) {
        size_t lineBreakPos = msg.find("\r\n");
        if (lineBreakPos != std::string::npos) {
            std::string currLine(msg.substr(0, lineBreakPos));
            std::cout << currLine << std::endl;
            msg.erase(0, lineBreakPos + 2);
            Parser *parser = new Parser(currLine, this);
            parser->parse_server_message(prefix);
            std::string cmd = parser->server_command();
            if(!strcmp(cmd.c_str(), "PING")) {
                send_server_message("PONG :tmi.twitch.tv");
            }
            delete parser;
        } else
            break;
    }
}

std::string Bot::is_username() {
    return username;
}

std::vector<std::string> Bot::is_channel() {
    return channels;
}

std::string Bot::is_prefix() {
    return prefix;
}

void Bot::new_prefix(const std::string &new_prefix) {
    prefix = new_prefix;
}
