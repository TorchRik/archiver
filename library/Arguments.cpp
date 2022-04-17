#include "Arguments.h"

Arguments::Arguments(std::vector<std::string>& arguments) {
    std::string last_command;
    for (std::string argument : arguments) {
        if (argument.empty()) {
            continue;
        }
        if (argument[0] == '-') {
            if (!last_command.empty()) {
                throw std::runtime_error("You could choose only on option (decode or encode)");
            }
            last_command = argument;
            if (argument == "-c") {
                command = Command::compress;
            } else if (argument == "-d") {
                command = Command::decompress;
            } else if (argument == "-h") {
                command = Command::info;
            } else {
                throw std::runtime_error("Incorrect command");
            }
            continue;
        }
        if (archive_name.empty()) {
            archive_name = argument;
        } else {
            filenames.push_back(argument);
        }
    }
}
