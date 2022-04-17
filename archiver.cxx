#include <iostream>
#include "Encoder.h"
#include "Decoder.h"
#include "Arguments.h"
#include "ArchiverConfig.h"

int main(int argc, char* argv[]) {
    if (argc < 2) {
        // report version
        std::cout << argv[0] << " Version " << archiver_VERSION_MAJOR << "."
                  << archiver_VERSION_MINOR << std::endl;
        std::cout << "Arguments:\n"
            "-c archive_name file1 [file2 ...]  Encode yours files to archive_name\n"
            "-d archive_name   decode your archive";
        return 1;
    }

    std::vector<std::string> commands(argc);
    for (int i = 1; i < argc; ++i) {
        commands[i] = argv[i];
    }
    Arguments arguments(commands);

    if (arguments.command == Arguments::Command::info) {
        std::string output = "Arguments:\n"
            "-c archive_name file1 [file2 ...]  Encode yours files to archive_name\n"
            "-d archive_name   decode your archive";
        std::cout << output;
    } else if (arguments.command == Arguments::Command::compress) {
        if (arguments.archive_name.empty() || arguments.filenames.empty()) {
            throw std::runtime_error("You didn't specify an archive or files");
        }
        std::ofstream out(arguments.archive_name);
        BitWriter writer(out);
        for (size_t index = 0; index < arguments.filenames.size(); ++index) {
            std::string path_to_file = arguments.filenames[index];
            std::fstream fin(path_to_file);
            if (!fin.is_open()) {
                throw std::runtime_error(path_to_file + " is incorrect path");
            }
            Encode(path_to_file, fin, writer, (index == arguments.filenames.size() - 1));
        }
    } else if (arguments.command == Arguments::Command::decompress) {
        if (!arguments.filenames.empty()) {
            throw std::runtime_error("You can decode only one file per once");
        }
        std::fstream archive_stream(arguments.archive_name);
        if (!archive_stream.is_open()) {
            throw std::runtime_error("Can not open archive file");
        }
        BitReader archive_reader(archive_stream);
        while (Decode(archive_reader)) {}
    }
}