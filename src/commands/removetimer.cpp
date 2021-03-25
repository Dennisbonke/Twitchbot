#include "../../includes/commands/removetimer.hpp"
#include "../../includes/timerhandler.hpp"
#include <fstream>
#include <iostream>

RemoveTimerCommand::RemoveTimerCommand(Bot *_bot) : bot{_bot} {
    names.push_back("removetimer");
    names.push_back("rmtimer");
}

void RemoveTimerCommand::execute(std::string, std::string original_msg, bool, bool, std::string channel) {
    std::string file = bot->is_timer_file(channel);
    std::fstream timers;
    std::string tmp_result{""};
    std::vector<std::string> timers_string;
    timers.open(file, std::fstream::in);
    if (!timers) {
		std::cerr << "No timer file for this channel file" << std::endl;
	} else {
        std::string timer;
        std::string line;
        while(std::getline(timers, line)) {
            if(line.empty()) {
                timers_string.push_back(timer);
                timer.clear();
            } else {
                timer.append(line);
            }
        }
        timers.close();
        std::size_t find_timer = original_msg.find(" ");
        if(find_timer != std::string::npos) {
            std::string timer_to_edit = original_msg.substr(find_timer + 1);
            std::vector<std::string>::iterator rmtimer;
            for(auto it = timers_string.begin(); it != timers_string.end(); ++it) {
                std::string _timer = it->data();
                std::size_t find_name = _timer.find(":");
                std::size_t end_name = _timer.find(" ");
                if(find_name != std::string::npos) {
                    std::string timer_name = _timer.substr(find_name + 1, end_name - find_name - 1);
                    if(!strcmp(timer_to_edit.c_str(), timer_name.c_str())) {
                        bot->send_chat_message("Removed the timer with name " + timer_name, channel);
                        bot->is_timerhandler(channel)->remove_timer();
                        rmtimer = it;
                    }
                }
            }
            timers_string.erase(rmtimer);
        }
    }

    std::fstream output;
    output.open(file, std::fstream::out);
    if(!output) {
        std::cerr << "No file excists" << std::endl;
    } else {
        for(auto &_timer : timers_string) {
            _timer.append("\n\n");
            output << _timer;
        }   
    }
    output.close();
}

bool RemoveTimerCommand::has_perms_to_run(bool mod, bool, std::string sender) {
    if(mod || bot->is_channel(sender) || bot->is_owner(sender))
        return true;
    return false;
}

bool RemoveTimerCommand::find_name(std::string command_name) {
    for(auto it = names.begin(); it != names.end(); it++) {
        if(!strcmp(it->data(), command_name.c_str()))
            return true;
    }
    return false;
}

std::string RemoveTimerCommand::list_command() {
    return names[0];
}

std::string RemoveTimerCommand::generate_help_message(const std::string &channel) {
    return "Use " + bot->is_prefix(channel) + names[0] + " [name] to remove a timed message from this channel.";
}

void RemoveTimerCommand::new_output(std::string) {}
