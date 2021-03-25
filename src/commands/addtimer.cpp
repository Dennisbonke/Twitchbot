#include "../../includes/commands/addtimer.hpp"
#include "../../includes/timerhandler.hpp"
#include <fstream>
#include <chrono>

AddTimerCommand::AddTimerCommand(Bot *_bot) : bot{_bot} {
    names.push_back("addtimer");
}

void AddTimerCommand::execute(std::string, std::string original_msg, bool, bool, std::string channel) {
    std::string file = bot->is_timer_file(channel);
    std::fstream timer_file;
    std::string new_timer{""};
    timer_file.open(file, std::ios::app);
    if(!timer_file) {
        std::cerr << "No such file excists" << std::endl;
    } else {
        std::string name{"Name:"}, interval{" Interval:"}, message{" Message:"};
        std::size_t find_name, find_interval, find_message;
        find_name = original_msg.find(" ");
        if(find_name != std::string::npos) {
            find_interval = original_msg.find(" ", find_name + 1);
            if(find_interval != std::string::npos) {
                find_message = original_msg.find(" ", find_interval + 1);
                if(find_message != std::string::npos) {
                    message.append(original_msg.substr(find_message + 1));
                    interval.append(original_msg.substr(find_interval + 1, find_message - find_interval - 1));
                    name.append(original_msg.substr(find_name + 1, find_interval - find_name - 1));
                    auto now = std::chrono::system_clock::now();
                    time_t times = std::chrono::system_clock::to_time_t(now);
                    tm *local_times = localtime(&times);
                    std::string new_hours = std::to_string(local_times->tm_hour);
                    std::string new_mins = std::to_string(local_times->tm_min);
                    std::string new_time = new_hours.append(":").append(new_mins);
                    new_timer = name.append(interval).append(" Last send:").append(new_time).append(message).append("\n\n");
                    timer_file << new_timer;
                    timer_file.close();
                    bot->is_timerhandler(channel)->add_timer();
                    bot->send_chat_message("Added the timer to the listed timers", channel);
                    return;
                }
            }
        }
    }
    bot->send_chat_message("Please provide all the arguments to the timer. For more info use !help addtimer", channel);
}

bool AddTimerCommand::has_perms_to_run(bool mod, bool, std::string sender) {
    if(mod || bot->is_channel(sender) || bot->is_owner(sender))
        return true;
    return false;
}

bool AddTimerCommand::find_name(std::string command_name) {
    for(auto it = names.begin(); it != names.end(); it++) {
        if(!strcmp(it->data(), command_name.c_str()))
            return true;
    }
    return false;
}

std::string AddTimerCommand::list_command() {
    return names[0];
}

std::string AddTimerCommand::generate_help_message(const std::string &channel) {
    return "Use " + bot->is_prefix(channel) + names[0] + " [name] [interval (min)] [message] to add a timed message to this channel.";
}

void AddTimerCommand::new_output(std::string) {}
