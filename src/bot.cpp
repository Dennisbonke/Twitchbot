#include "../includes/bot.hpp"
#include "../includes/parser.hpp"
#include "../includes/commandhandler.hpp"
#include "../includes/timerhandler.hpp"
#include <iostream>

Bot::Bot(std::string _username, std::vector<std::string> _channels, std::string prefix, sockpp::tcp_connector *_conn) 
    : username{_username}, channels{_channels}, conn{_conn}, owner{_username} {
        for(auto &__channels : _channels) {
            prefixes.insert(std::pair<std::string, std::string>(__channels, prefix));
            commandhandlers.insert(std::pair<std::string, CommandHandler *>(__channels, new CommandHandler(this)));
            timerhandlers.insert(std::pair<std::string, TimerHandler *>(__channels, new TimerHandler(__channels, this)));
        }
        parser = new Parser(this);
    }

Bot::~Bot() {
    for(auto &channel : channels) {
        delete commandhandlers.at(channel);
        delete timerhandlers.at(channel);
    }
    prefixes.clear();
    commandhandlers.clear();
    timerhandlers.clear();
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
        process_messages(message_buffer);
        check_timers();
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
            parser->parse_server_message(currLine);
        } else
            break;
    }
}

void Bot::check_timers() {
    for(auto const &[channel, handler] : timerhandlers) {
        handler->calc_timer();
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

std::string Bot::is_timer_file(const std::string &channel) {
    return timerhandlers.at(channel)->is_timer_file();
}

void Bot::new_prefix(const std::string &new_prefix, const std::string &channel) {
    prefixes.at(channel) = new_prefix;
}

CommandHandler * Bot::is_commandhandler(const std::string &_channel) {
    for(auto const &[channel, handler] : commandhandlers) {
        if(!strcmp(channel.c_str(), _channel.c_str()))
            return handler;
    }
    return nullptr;
}
