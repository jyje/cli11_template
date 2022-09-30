#ifndef _ARGS_HPP
#define _ARGS_HPP

#include <iostream>
#include <CLI/CLI.hpp>
#include "core.hpp"

class Args {
    private:
        static Args* _instance;

        const std::string _program_name{"cli11_template"};
        const std::string _program_version{"0.0.1"};
        const std::string _program_description{"My Template for CLI11"};
        const bool _showing_empty_help{false};
        const int _column_width{40};
        const bool _set_version_flag{true};
        
        CLI::App _app{Args::_program_name};

        Args();
        ~Args();

        int doEmptyWork();
        void doMain();
    public:
        static Args& getInstance();

        int process_number;
        int processing_duration;
        std::string some_message;

        int parseArgs(int argc, char* argv[]);
};
#endif
