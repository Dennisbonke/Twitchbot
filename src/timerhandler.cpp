#include "../includes/timerhandler.hpp"
#include "../includes/bot.hpp"
#include <fstream>
#include <iostream>
#include <string.h>
#include <chrono>

TimerHandler::TimerHandler(std::string _channel, Bot *_bot) 
    : timer_file{"files/timers/" + _channel + "_timers.txt"}, channel{_channel}, amount_timers{1}, bot{_bot} {
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
            tmp.append(new_time).append(" ")
            .append("Message:Check out westlanderz at github for more projects, you can DM him with questions on discord SenpaiR6#1717 \n\n");
            file << tmp;
        }
        file.close();
    }

TimerHandler::~TimerHandler() {}

void TimerHandler::calc_timer() {
    std::fstream timers;
    timers.open(timer_file, std::ios::in);
    if (!timers) {
		std::cerr << "No timer file for this channel file" << std::endl;
	} else {
        if(amount_timers != 0) {
            for(int i = 1; i <= amount_timers; i++){
                std::string timer;
                std::string line;
                while(std::getline(timers, line)) {
                    if(line.empty()) {
                        break;
                    }
                    timer.append(line);
                }
                std::size_t find_name = timer.find(":");
                std::size_t end_name = timer.find(" ");
                if(find_name != std::string::npos){
                    std::size_t find_interval = timer.find(":", find_name + 1);
                    std::size_t end_interval = timer.find(" ", find_interval + 1);
                    if(find_interval != std::string::npos) {
                        std::string timer_name = timer.substr(find_name + 1, end_name - find_name - 1);
                        std::size_t find_last_send = timer.find(":", find_interval + 1);
                        if(find_last_send != std::string::npos) {
                            std::string timer_interval = timer.substr(find_interval + 1, end_interval - find_interval - 1);
                            std::string timer_last_send = timer.substr(find_last_send + 1, 5);
                            std::size_t find_message = timer.find(":", find_last_send + 5);
                            if(find_message != std::string::npos) {
                                std::string message = timer.substr(find_message + 1);
                                int interval = std::stoi(timer_interval);
                                auto now = std::chrono::system_clock::now();
                                time_t ttime = std::chrono::system_clock::to_time_t(now);
                                tm *local_time = localtime(&ttime);
                                int min = local_time->tm_min;
                                int hour = local_time->tm_hour;
                                int mins = std::stoi(timer_last_send.substr(timer_last_send.find(":") + 1, 2));
                                int hours = std::stoi(timer_last_send.substr(timer_last_send.find(":") - 2, 2));
                                if((hour * 60 + min) > (hours * 60 + mins + interval)) {
                                    bot->send_chat_message(message, channel);
                                    auto now = std::chrono::system_clock::now();
                                    time_t times = std::chrono::system_clock::to_time_t(now);
                                    tm *local_times = localtime(&times);
                                    std::string new_hours = std::to_string(local_times->tm_hour);
                                    std::string new_mins = std::to_string(local_times->tm_min);
                                    std::string new_time = new_hours.append(":").append(new_mins);
                                    std::string tmp_result = timer.substr(0, timer.find("Last send:") + 10);
                                    tmp_result.append(new_time).append(" Message:").append(message);
                                    timers.close();
                                    std::fstream output_file;
                                    output_file.open(timer_file, std::ios::out);
                                    if (!output_file) {
                                        std::cerr << "No timer file for this channel file" << std::endl;
                                    } else {
                                        output_file << tmp_result;
                                    }
                                    output_file.close();
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    if(timers.is_open())
        timers.close();
}

std::string TimerHandler::is_timer_file() {
    return timer_file;
}
