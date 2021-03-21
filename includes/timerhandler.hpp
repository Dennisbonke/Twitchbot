#ifndef _timerhandler_h
#define _timerhandler_h

#include <string>

class Bot;

class TimerHandler {
    public:
        TimerHandler(std::string, Bot *);
        virtual ~TimerHandler();

        void calc_timer();
        void add_timer();
        void remove_timer();
        std::string is_timer_file();

    private:
        std::string timer_file;
        std::string channel;
        int amount_timers;
        Bot *bot;
};

#endif //_timerhandler_h
