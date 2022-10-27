#ifndef _METHOD_HPP
#define _METHOD_HPP

#include <thread>
#include <sstream>
#include <vector>
#include "../include/progress.hpp"
#include "../source/args.hpp"

namespace Method {
    static int doProcess(int number, int duration, std::string message) {

        progress::Time time;
        time.tic();
        std::cout << std::endl;
        time.check(0.0f, float(number));

        std::stringstream stream{message};

        for (int index = 0; index < number; index ++) {            
            std::this_thread::sleep_for(std::chrono::milliseconds(duration));
            
            time.toc(true);
            time.check(float(index + 1), float(number), &stream);
        }
        time.toc();

        return EXIT_SUCCESS;
    }
};
#endif
