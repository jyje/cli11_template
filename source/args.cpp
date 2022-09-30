#include "args.hpp"
#include "core.hpp"

Args* Args::_instance = nullptr;

Args& Args::getInstance() {
    if (Args::_instance == nullptr) {
        Args::_instance = new Args();
    }
    
    return *Args::_instance;
}

Args::Args() {
    if (this->_set_version_flag) {
        this->app.set_version_flag("-v, --version", Args::_program_version);
    }

    this->app.description(Args::_program_description);
}

Args::~Args() {
    //// TODO: delete all arrays
    delete[] this->argv;
}

int Args::parseArgs(int argc, char* argv[]) {
    this->argc = argc;
    this->argv = argv;

    this->app.callback([&]() {
        return this->doEmptyWork();
    });

    this->doMain();

    CLI11_PARSE(app, argc, argv);

    return EXIT_SUCCESS;
}

int Args::doEmptyWork() {
    if (this->_showing_empty_help) {
        throw CLI::CallForHelp();
    }

    std::cout << "Hello, World!" << std::endl;

    return EXIT_SUCCESS;
}

void Args::doMain() {
    std::string name = "process";
    std::string description = "Do dummy process with progress display.";

    auto sub = this->app.add_subcommand(name, description);

    sub->add_option("-n, --number", this->process_number, "Set a process number.")
        ->default_val<int>(500)
        ->check(CLI::PositiveNumber);
    sub->add_option("-d, --duration", this->processing_duration, "Set a expected processing duration [milliseconds].")
        ->default_val<int>(200)
        ->check(CLI::NonNegativeNumber);
    sub->add_option("-m, --message", this->some_message, "Set some message to display in process panel.")
        ->default_val<std::string>("in progress...");
    
    sub->final_callback([&]() {
        exit(Core::doProcess(this->process_number, this->processing_duration, this->some_message));
    });
}
