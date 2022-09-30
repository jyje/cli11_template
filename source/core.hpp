#ifndef _CORE_HPP
#define _CORE_HPP

#include <thread>
#include "progress.hpp"

namespace Core {
    static int doProcess(int number, int duration, std::string message) {

        progress::Time time;
        time.tic();
        std::cout << std::endl;
        time.check(0.0f, float(number));

        std::stringstream stream{message};

        for (int index = 0; index < number; index ++) {            
            std::this_thread::sleep_for(std::chrono::milliseconds(duration)); // 'thread' included
            
            time.toc(true);
            time.check(float(index + 1), float(number), &stream);
        }
        time.toc();

        return EXIT_SUCCESS;
    }
};
#endif
