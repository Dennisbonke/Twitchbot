#include "../../includes/commands/edittimer.hpp"
#include <fstream>
#include <chrono>

EditTimerCommand::EditTimerCommand(Bot *_bot) : bot{_bot} {
    names.push_back("edittimer");
}

void EditTimerCommand::execute(std::string sender, std::string original_msg, bool, bool, std::string channel) {
    std::string file = bot->is_timer_file(channel);
    std::fstream timers;
    std::string tmp_result{""};
    std::vector<std::string> timers_string;
    std::vector<std::string> timers_to_write;
    bool found{false};
    timers.open(file, std::ios::in);
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
            std::size_t find_new_msg = original_msg.find(" ", find_timer + 1);
            if(find_new_msg != std::string::npos) {
                std::string timer_to_edit = original_msg.substr(find_timer + 1, find_new_msg - find_timer - 1);
                std::string message = original_msg.substr(find_new_msg + 1);
                for(auto &_timer : timers_string) {
                    std::size_t find_name = _timer.find(":");
                    std::size_t end_name = _timer.find(" ");
                    if(find_name != std::string::npos) {
                        std::size_t find_interval = _timer.find(":", find_name + 1);
                        if(find_interval != std::string::npos) {
                            std::string timer_name = _timer.substr(find_name + 1, end_name - find_name - 1);
                            std::size_t find_last_send = _timer.find(":", find_interval + 1);
                            if(!strcmp(timer_to_edit.c_str(), timer_name.c_str())) {
                                if(find_last_send != std::string::npos) {
                                    std::size_t find_message = _timer.find(":", find_last_send + 5);
                                    if(find_message != std::string::npos) {
                                        auto now = std::chrono::system_clock::now();
                                        time_t ttime = std::chrono::system_clock::to_time_t(now);
                                        tm *local_time = localtime(&ttime);
                                        std::string new_hours = std::to_string(local_time->tm_hour);
                                        std::string new_mins = std::to_string(local_time->tm_min);
                                        std::string new_time = new_hours.append(":").append(new_mins);
                                        tmp_result.append(_timer.substr(0, _timer.find("Last send:") + 10));
                                        tmp_result.append(new_time)
                                            .append(" Message:")
                                            .append(message)
                                            .append("\n\n");
                                    }
                                }
                                found = true;
                            } else {
                                tmp_result.append(_timer + "\n\n");
                            }
                        }
                    }
                    timers_to_write.push_back(tmp_result);
                    tmp_result.clear();
                }
            }
        }
    }
    if(timers.is_open())
        timers.close();

    std::fstream output;
    output.open(file, std::ios::out);
    if(!output) {
        std::cerr << "No file excists" << std::endl;
    } else {
        for(auto &_timer : timers_to_write) {
            output << _timer;
        }
    }
    output.close();

    if(found)
        bot->send_chat_message("Editted the timer for you " + sender, channel);
    else
        bot->send_chat_message("Could not find the timer you were looking for " + sender + " please make sure you have written the name correctly", channel);
}

bool EditTimerCommand::has_perms_to_run(bool mod, bool, std::string sender) {
    if(mod || bot->is_channel(sender) || bot->is_owner(sender))
        return true;
    return false;
}

bool EditTimerCommand::find_name(std::string command_name) {
    for(auto it = names.begin(); it != names.end(); it++) {
        if(!strcmp(it->data(), command_name.c_str()))
            return true;
    }
    return false;
}

std::string EditTimerCommand::list_command() {
    return names[0];
}

std::string EditTimerCommand::generate_help_message(const std::string &channel) {
    return "Use " + bot->is_prefix(channel) + names[0] + " [name] [message] to edit a timed message from this channel.";
}

void EditTimerCommand::new_output(std::string) {}

