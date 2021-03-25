#include "../includes/timerhandler.hpp"
#include "../includes/bot.hpp"
#include <fstream>
#include <iostream>
#include <string.h>
#include <chrono>

/**
 * @brief Construct a new Timer Handler:: Timer Handler object
 * 
 * @param _channel the channel for the timerhandler
 * @param _bot the bot instance so it can send messages from here
 */
TimerHandler::TimerHandler(std::string _channel, Bot *_bot) 
    : timer_file{"../files/timers/" + _channel + "_timers.txt"}, channel{_channel}, amount_timers{1}, bot{_bot} {
        std::fstream file;
        file.open(timer_file, std::ios::trunc | std::ios::out);
        if(!file){
            std::cerr << "file cant be opened" << std::endl;
        } else {
            std::string tmp = "Name:autogen Interval:1 Last send:";
            auto now = std::chrono::system_clock::now();
            time_t times = std::chrono::system_clock::to_time_t(now);
            tm *local_times = localtime(&times);
            std::string new_hours = std::to_string(local_times->tm_hour);
            std::string new_mins = std::to_string(local_times->tm_min);
            std::string new_time = new_hours.append(":").append(new_mins);
            tmp.append(new_time)
                .append(" Message:Check out westlanderz at github for more projects, you can DM him with questions on discord SenpaiR6#1717 \n\n");
            file << tmp;
        }
        file.close();
    }

/**
 * @brief Destroy the Timer Handler:: Timer Handler object
 * 
 */
TimerHandler::~TimerHandler() {}

/**
 * @brief goes through the tiemrs in the timer file and checks if it needs to be executed
 * 
 */
void TimerHandler::calc_timer() {
    std::fstream timers;
    std::string tmp_result{""};
    std::vector<std::string> timers_string;
    std::vector<std::string> timers_to_write;
    timers.open(timer_file, std::ios::in);
    if (!timers) {
		std::cerr << "No timer file for this channel file" << std::endl;
	} else {
        if(amount_timers != 0) {
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
            for(auto &_timer : timers_string) {
                std::size_t find_name = _timer.find(":");
                std::size_t end_name = _timer.find(" ");
                if(find_name != std::string::npos) {
                    std::size_t find_interval = _timer.find(":", find_name + 1);
                    std::size_t end_interval = _timer.find(" ", find_interval + 1);
                    if(find_interval != std::string::npos) {
                        std::string timer_name = _timer.substr(find_name + 1, end_name - find_name - 1);
                        std::size_t find_last_send = _timer.find(":", find_interval + 1);
                        if(find_last_send != std::string::npos) {
                            std::string timer_interval = _timer.substr(find_interval + 1, end_interval - find_interval - 1);
                            std::string timer_last_send = _timer.substr(find_last_send + 1, 5);
                            std::size_t find_message = _timer.find(":", find_last_send + 5);
                            if(find_message != std::string::npos) {
                                std::string message = _timer.substr(find_message + 1);
                                int interval = std::stoi(timer_interval);
                                auto now = std::chrono::system_clock::now();
                                time_t ttime = std::chrono::system_clock::to_time_t(now);
                                tm *local_time = localtime(&ttime);
                                int min = local_time->tm_min;
                                int hour = local_time->tm_hour;
                                int mins = std::stoi(timer_last_send.substr(timer_last_send.find(":") + 1, 2));
                                int hours = std::stoi(timer_last_send.substr(timer_last_send.find(":") - 2, 2));
                                if((hour * 60 + min) > (hours * 60 + mins + interval) || (hour == 0 && min < 5)) {
                                    bot->send_chat_message(message, channel);
                                    now = std::chrono::system_clock::now();
                                    time_t times = std::chrono::system_clock::to_time_t(now);
                                    tm *local_times = localtime(&times);
                                    std::string new_hours = std::to_string(local_times->tm_hour);
                                    std::string new_mins = std::to_string(local_times->tm_min);
                                    std::string new_time = new_hours.append(":").append(new_mins);
                                    tmp_result.append(_timer.substr(0, _timer.find("Last send:") + 10));
                                    tmp_result.append(new_time)
                                        .append(" Message:")
                                        .append(message)
                                        .append("\n\n");
                                } else {
                                    tmp_result.append(_timer + "\n\n");
                                }
                            }
                        }
                    }
                }
                timers_to_write.push_back(tmp_result);
                tmp_result.clear();
            }
        }
    }
    if(timers.is_open())
        timers.close();

    std::fstream output;
    output.open(timer_file, std::ios::out);
    if(!output) {
        std::cerr << "No file excists" << std::endl;
    } else {
        for(auto &_timer : timers_to_write) {
            output << _timer;
        }
    }
    output.close();
}

void TimerHandler::add_timer() {
    amount_timers++;
}

void TimerHandler::remove_timer() {
    amount_timers--;
}

/**
 * @brief Gives back the file that excists on this handler
 * 
 * @return std::string the file for the timerhandler
 */
std::string TimerHandler::is_timer_file() {
    return timer_file;
}
