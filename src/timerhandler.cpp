#include "../includes/timerhandler.hpp"
#include "../includes/bot.hpp"
#include <fstream>
#include <iostream>
#include <string.h>
#include <chrono>

TimerHandler::TimerHandler(std::string _channel, Bot *_bot) 
    : timer_file{_channel + "_timers.txt"}, channel{_channel}, amount_timers{1}, bot{_bot} {
        std::fstream file;
        file.open(timer_file, std::ios::trunc | std::ios::out);
        if(!file){
            std::cerr << "file cant be opened" << std::endl;
        } else {
            file << "Name:autogen\r\n";
            file << "Interval:30\r\n";
            file << "Last send:0\r\n";
            file << "Message:Check out westlanderz at github for more projects, you can DM him with questions on discord SenpaiR6#1717\r\n";
            file << "\r\n";
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
                    if(!strcmp(line.c_str(), "\r\n") || !strcmp(line.c_str(), "\n\r") || !strcmp(line.c_str(), "\n"))
                        break;
                    timer.append(line);
                }
                std::size_t find_name = timer.find("\r\n");
                if(find_name != std::string::npos){
                    std::string timer_name = timer.substr(5, find_name - 5);
                    timer.erase(0, find_name + 3);
                    std::size_t find_interval = timer.find("\r\n");
                    if(find_interval != std::string::npos) {
                        std::string timer_interval = timer.substr(9, find_interval - 9);
                        timer.erase(0, find_interval + 3);
                        std::size_t find_last_send = timer.find("\r\n");
                        if(find_last_send != std::string::npos) {
                            std::string timer_last_send = timer.substr(10, find_last_send - 10);
                            timer.erase(0, find_last_send + 3);
                            std::size_t find_message = timer.find("\r\n");
                            if(find_message != std::string::npos) {
                                std::string message = timer.substr(8, find_message - 8);
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
                                    timer.clear();
                                    while(!timers.eof()) {
                                        std::getline(timers, line);
                                        std::size_t find_timer = line.find(timer_last_send);
                                        if(find_timer != std::string::npos) {
                                            timer.append("Last send:").append(new_time);
                                        } else {
                                            timer.append(line);
                                        }
                                    }
                                    timers.close();
                                    std::fstream output_file;
                                    output_file.open(timer_file, std::ios::trunc | std::ios::out);
                                    if (!output_file) {
                                        std::cerr << "No timer file for this channel file" << std::endl;
                                    } else {
                                        output_file << timer;
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}

std::string TimerHandler::is_timer_file() {
    return channel + "_timers.txt";
}
