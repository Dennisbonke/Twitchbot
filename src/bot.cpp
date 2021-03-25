#include "../includes/bot.hpp"
#include "../includes/parser.hpp"
#include "../includes/commandhandler.hpp"
#include "../includes/timerhandler.hpp"
#include <iostream>

/**
 * @brief Construct a new Bot:: Bot object
 * 
 * @param _username the username the bot will be having in chat
 * @param _channels a list full of channels the bot is connecting to
 * @param prefix the default prefix for all the channels
 * @param _conn the connection to the server
 */
Bot::Bot(std::string _username, std::vector<std::string> _channels, std::string prefix, sockpp::tcp_connector *_conn) 
    : username{_username}, channels{_channels}, conn{_conn}, owner{_username} {
        for(auto &__channels : _channels) {
            prefixes.insert(std::pair<std::string, std::string>(__channels, prefix));
            commandhandlers.insert(std::pair<std::string, CommandHandler *>(__channels, new CommandHandler(this)));
            timerhandlers.insert(std::pair<std::string, TimerHandler *>(__channels, new TimerHandler(__channels, this)));
        }
        parser = new Parser(this);
    }

/**
 * @brief Destroy the Bot:: Bot object
 * 
 */
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

/**
 * @brief Lets the bot log in to the TMI servers with a given oauth code
 * 
 * @param password the oauth code used to log in to the TMI-servers
 */
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

/**
 * @brief Lets the bot log out at the TMI servers
 * 
 */
void Bot::log_out() {
    for(auto &channel : channels) {
        std::string part_msg = "PART #";
        part_msg.append(channel);
        send_server_message(part_msg);
    }
    send_server_message("QUIT :Bot is shutting down");
}

/**
 * @brief the main program loop with async handling of incoming messages and timers
 * 
 */
void Bot::run() {
    std::string message_buffer;
    async::run_queue rq;
	async::queue_scope qs{&rq};

    // send_chat_message("Hi Chat, its Westlanderz chatbot right here talking....");
    while(true) {
        char buffer[513] = {};
        conn->read(buffer, sizeof(buffer) - 1);
        message_buffer.append(buffer);
        async::run(process_messages(message_buffer), async::current_queue);
        async::run(check_timers(), async::current_queue);
        message_buffer.erase();
    }
}

/**
 * @brief sends a message to the chat of the given channel
 * 
 * @param msg the message it needs to send to the chat
 * @param channel the channel it needs to send the message to
 */
void Bot::send_chat_message(const std::string &msg, const std::string &channel) {
    std::string send_msg{"PRIVMSG #"};
    send_msg.append(channel);
    send_msg.append(" :");
    send_msg.append(msg);
    send_server_message(send_msg);
}

/**
 * @brief sends a message to the TMI server, recommended to use send_chat_message to send a message in chat
 * 
 * @param msg the full message that will be send to the server
 */
void Bot::send_server_message(const std::string &msg) {
    std::string server_msg{msg};
    server_msg.append("\r\n");

    if(conn->write_n(server_msg.c_str(), server_msg.length()) == -1) {
        std::cerr << "sending message to the server failed\r\n"
        << server_msg << std::endl;
    }
}

/**
 * @brief this is an async version to process messages that were recieved by the bot
 * 
 * @param msg the message that was recieved by the server
 * @return async::result<void> its void but async from libasync
 */
async::result<void> Bot::process_messages(std::string &msg) {
    while (true) {
        std::size_t lineBreakPos = msg.find("\r\n");
        if (lineBreakPos != std::string::npos) {
            std::string currLine(msg.substr(0, lineBreakPos));
            std::cout << currLine << std::endl;
            msg.erase(0, lineBreakPos + 2);
            parser->parse_server_message(currLine);
        } else
            break;
    }
    co_return;
}

/**
 * @brief handles the timers of each channel
 * 
 * @return async::result<void> its void but async from libasync
 */
async::result<void> Bot::check_timers() {
    for(auto const &[channel, handler] : timerhandlers) {
        handler->calc_timer();
    }
    co_return;
}

/**
 * @brief just to give back the username of this bot
 * 
 * @return std::string the username of this bot
 */
std::string Bot::is_username() {
    return username;
}

/**
 * @brief checks if the given user is a channel in the list
 * 
 * @param channel the channel to compare to the connected channels
 * @return true 
 * @return false 
 */
bool Bot::is_channel(const std::string &channel) {
    // TODO: better check
    for(auto &_channel : channels) {
        if(!strcmp(_channel.c_str(), channel.c_str()))
            return  true;
    }
    return false;
}

/**
 * @brief checks if the user is the bot owner
 * 
 * @param _owner the sender of the command to check if its the bot owner
 * @return true 
 * @return false 
 */
bool Bot::is_owner(const std::string &_owner) {
    if(!strcmp(_owner.c_str(), owner.c_str()))
        return true;
    return false;
}

/**
 * @brief returns the prefix for a channel
 * 
 * @param channel the channel to check the prefix of
 * @return std::string the prefix on that channel
 */
std::string Bot::is_prefix(const std::string &channel) {
    return prefixes.at(channel);
}

/**
 * @brief gets the file location for a certain channel
 * 
 * @param channel the channel to get the timer file of
 * @return std::string the file location for the channel
 */
std::string Bot::is_timer_file(const std::string &channel) {
    return timerhandlers.at(channel)->is_timer_file();
}

/**
 * @brief changes the prefix of a channel
 * 
 * @param new_prefix the prefix to change to
 * @param channel the channel to change the prefix of
 */
void Bot::new_prefix(const std::string &new_prefix, const std::string &channel) {
    prefixes.at(channel) = new_prefix;
}

/**
 * @brief gets the commandhandler of a given channel
 * 
 * @param _channel the channel to get the handler of
 * @return CommandHandler * the commandhandler to return
 */
CommandHandler * Bot::is_commandhandler(const std::string &_channel) {
    for(auto const &[channel, handler] : commandhandlers) {
        if(!strcmp(channel.c_str(), _channel.c_str()))
            return handler;
    }
    return nullptr;
}

/**
 * @brief gets the timerhandler of a given channel
 * 
 * @param _channel the channel to get the handler of
 * @return TimerHandler * the timerhandler to return
 */
TimerHandler * Bot::is_timerhandler(const std::string &_channel) {
    for(auto const &[channel, handler] : timerhandlers) {
        if(!strcmp(channel.c_str(), _channel.c_str()))
            return handler;
    }
    return nullptr;
}
