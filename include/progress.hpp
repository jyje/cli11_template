#ifndef _PROGRESS_HPP
#define _PROGRESS_HPP

#pragma once

#include <iostream>
#include <chrono>
#include <sstream>
#include <iomanip>
#include <algorithm>

/// Simple progressbar with smart panel in C++
// by jyje (jyjeon@outlook.com)

namespace progress {
    static std::string padLines(std::stringstream& input, int width = 80, char delim = ' ') {
        std::istringstream stream{input.str()};
        std::string token;
        std::stringstream output;
        while(std::getline(stream, token, '\n')) {
            output << " " << token.append(width - token.length(), delim) << std::endl;
        }

        return output.str();
    }

    namespace bar {
        static void display(float current, float total, std::string prefix = "", std::string barfix = "", std::string suffix = "") {
            const char pb[] = ">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>";
            const int count = sizeof(pb) / sizeof(*pb) - 1;

            float rate = current / total;
            int val = (int)(rate * 100);
            int lpad = (int)(rate * count);
            int rpad = count - lpad;

            int lineCount = 1;                
            if (prefix.length() > 0) lineCount += 1 + int(std::count(prefix.begin(), prefix.end(), '\n'));
            if (suffix.length() > 0) lineCount += 1 + int(std::count(suffix.begin(), suffix.end(), '\n'));
            
            if (lineCount > 1 && current != 0) {
                for (int index = 0; index < lineCount; index++) {
                    printf("\x1b[A");
                }
            }

            if (prefix.length() > 0) std::cout << prefix << std::endl;
            printf("\r %3d%% [%.*s%*s] %s\n", val, lpad, pb, rpad, "", barfix.c_str());
            if (suffix.length() > 0) std::cout << suffix << std::endl;
            fflush(stdout);
        }    
    };

    class Time {
        private:
            bool _is_hiding_time = false;
            std::chrono::system_clock::time_point _a_point;
            std::chrono::system_clock::time_point _b_point;
            
        public:
            std::chrono::system_clock::time_point tic_point;
            std::chrono::system_clock::time_point toc_point;

            Time(bool is_hiding_time = false)  {
                this->_is_hiding_time = is_hiding_time;
            }

            ~Time() { }

            static std::string second2time(double input) {
                std::stringstream output;
                double divided;
                double fractional = modf(input, &divided);

                int inSeconds = int(divided);
                int inMinutes = int(floor(inSeconds / 60.0));
                int inHours = int(floor(inMinutes / 60.0));

                if (inHours > 0) {
                    output << inHours << "h ";
                }

                if (inMinutes > 0 || inHours > 0) {
                    output << std::setprecision(2) << std::fixed << int(inMinutes % 60) << "m ";
                    output << std::setfill('0') << std::setw(4);
                }
                
                output << std::setprecision(1) << std::fixed << double(inSeconds % 60) + fractional << "s";

                return output.str();
            }

            static std::string point2string(std::chrono::system_clock::time_point tp) {
                std::time_t time = std::chrono::system_clock::to_time_t(tp);                

                #if (defined(WIN32) || defined(_WIN32) || defined(__WIN32__))
                    struct tm timeinfo;
                    localtime_s(&timeinfo, &time);
                    auto validTime = &timeinfo;
                #else // __linux__
                    auto validTime = std::localtime(&time);
                #endif
                
                std::stringstream ss;
                ss << std::put_time(validTime, "%Y-%m-%d %H:%M:%S");

                return ss.str();
            }

            static double pointDiff(std::chrono::system_clock::time_point a, std::chrono::system_clock::time_point b) {
                auto duration = std::chrono::duration_cast<std::chrono::microseconds>(b - a);
                return duration.count()/1000.0;
            }

            std::chrono::system_clock::time_point tic(bool is_hiding_time = false) {
                this->_a_point = this->_b_point = this->tic_point = this->toc_point = std::chrono::system_clock::now();

                if (!is_hiding_time && !this->_is_hiding_time) {
                    std::cout << "Tic at {" << Time::point2string(this->_a_point) << "}" << std::endl;
                }

                return this->tic_point;
            }

            std::chrono::system_clock::time_point toc(bool is_hiding_time = false) {
                this->_a_point = this->_b_point;
                this->_b_point = this->toc_point = std::chrono::system_clock::now();

                if (!is_hiding_time && !this->_is_hiding_time) {
                    std::cout << "Toc at {" << Time::point2string(this->_b_point) << "}" << std::endl;
                }

                return this->toc_point;
            }

            double toc2toc() {
                return Time::pointDiff(this->_a_point, this->_b_point);
            }

            double tic2toc() {
                return Time::pointDiff(this->tic_point, this->toc_point);
            }

            void check(float current, float total, std::stringstream *frontStream = nullptr) {
                const int statusWidth = 80;
                const int headerIndent = 25;

                std::string front, /*middle,*/ tail;
                std::stringstream /*frontStream,*/ middleStream, tailStream;

                if (frontStream == nullptr) {
                    frontStream = new std::stringstream{" "};
                }
                
                front = progress::padLines(*frontStream, statusWidth);

                double toctoc = this->toc2toc();
                double tictoc = this->tic2toc();

                tailStream << std::endl;
                tailStream << std::setfill(' ') << std::setw(headerIndent) << "Current Processing Time:";
                tailStream << std::setfill(' ') << std::setw(14) << std::setprecision(3) << std::fixed << toctoc << " ms";
                tailStream << " (" << std::setfill(' ') << std::setw(11) << std::setprecision(2) << 1000.0 / (toctoc + std::numeric_limits<double>::epsilon()) << " Hz)";
                tailStream << std::endl;

                double averageTime = tictoc / ((double)current + std::numeric_limits<double>::epsilon());

                tailStream << std::setfill(' ') << std::setw(headerIndent) << "Average Processing Time:";
                tailStream << std::setfill(' ') << std::setw(14) << std::setprecision(3) << std::fixed << averageTime << " ms";
                tailStream << " (" << std::setfill(' ') << std::setw(11) << std::setprecision(2) << 1000.0 / (averageTime + std::numeric_limits<double>::epsilon()) << " Hz)";
                tailStream << std::endl;

                tailStream << std::setfill(' ') << std::setw(headerIndent) << "Elapsed Time:";
                tailStream << std::setfill(' ') << std::setw(17) << Time::second2time(tictoc / 1000.0);
                tailStream << " (" << std::setfill(' ') << std::setw(12) << std::setprecision(3) << tictoc / 1000.0 << " s)";
                tailStream << std::endl;

                double remainingSeconds = averageTime * (total - current) / 1000.0;
                double expectedSeconds = averageTime * total / 1000.0;

                tailStream << std::setfill(' ') << std::setw(headerIndent) << "Remaining Time:";
                tailStream << std::setfill(' ') << std::setw(17) << Time::second2time(remainingSeconds);
                tailStream << " (" << std::setfill(' ') << std::setw(12) << std::setprecision(3) << remainingSeconds << " s)";
                tailStream << std::endl;

                tailStream << std::setfill(' ') << std::setw(headerIndent) << "Expected Time:";
                tailStream << std::setfill(' ') << std::setw(17) << Time::second2time(expectedSeconds);
                tailStream << " (" << std::setfill(' ') << std::setw(12) << std::setprecision(3) << expectedSeconds << " s)";
                tailStream << std::endl;

                tail = progress::padLines(tailStream, statusWidth);

                if (current == 0) {
                    int frontLineCount = int(std::count(front.begin(), front.end(), '\n'));
                    int tailLineCount = int(std::count(tail.begin(), tail.end(), '\n'));

                    std::stringstream frontDummy{" "};
                    if (frontLineCount > 0) frontDummy << std::string(frontLineCount, '\n');
                    
                    std::stringstream tailDummy{" "};
                    if (tailLineCount > 0) tailDummy << std::string(tailLineCount, '\n');

                    progress::bar::display(0, total, frontDummy.str(), "", tailDummy.str());
                    return;
                }

                middleStream << (int)current << " / " << (int)total;

                progress::bar::display(current, total, front, middleStream.str(), tail);
            }
    };
};
#endif
