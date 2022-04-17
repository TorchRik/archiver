#pragma once

#include <vector>
#include <string>

struct Arguments {
    enum class Command {
        compress,
        decompress,
        info
    };

    Command command;
    std::string archive_name;
    std::vector<std::string> filenames;

    Arguments(std::vector<std::string>& elements);
};
