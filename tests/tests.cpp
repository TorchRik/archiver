#include "BitReader.h"
#include "BitWriter.h"
#include "PriorityQueue.h"
#include <sstream>
#include "Encoder.h"
#include "Decoder.h"
#include <filesystem>
#include "catch.hpp"
#include "iostream"

#define CATCH_CONFIG_MAIN

void CheckOutput(const std::vector<bool>& bits, const std::vector<char>& data) {
    size_t bits_index = 0, data_index = 0;
    while (bits_index < bits.size()) {
        unsigned char next_byte = data[data_index++];
        for (int i = 0; i < 8 && bits_index < bits.size(); i ++) {
            REQUIRE(bits[bits_index++] == (next_byte >> (7 - i)) % 2);
        }
    }
}

TEST_CASE("TestBitReader") {
    std::vector<char> data = {0x0f, 0x0a};
    std::stringstream stream;
    stream << data[0] << data[1];
    BitReader reader(stream);
    auto bits = reader.ReadBits(9);
    REQUIRE(bits.size() == 9);
    CheckOutput(bits, data);
}

TEST_CASE("TestBitWriter") {
    std::vector<bool> data = {0, 1, 0, 1, 0, 1, 1, 0, 1, 0, 1};
    std::stringstream stream;
    BitWriter writer(stream);
    writer.WriteBits(data);
    writer.WriteBuffer();
    BitReader reader(stream);
    std::vector<bool> bits = reader.ReadBits(11);
    for (size_t index = 0; index < 11; ++index) {
        REQUIRE(data[index] == bits[index]);
    }
}

TEST_CASE("PriorityQueue") {
    PriorityQueue my_queue;
    std::vector<LetterStatistic> start_elements{{GetLetterCodeFromChar('a'), 10}, {GetLetterCodeFromChar('b'), 20},
                                     {GetLetterCodeFromChar('c'), 30}};
    for (auto& element : start_elements) {
        my_queue.PushNewElement( std::make_shared<Node>(element));
    }
    REQUIRE(my_queue.PopTopElement()->data.count_of_letters == 10);
    REQUIRE(my_queue.PopTopElement()->data.count_of_letters == 20);
    LetterStatistic next_stat {GetLetterCodeFromChar('d'), 1};
    my_queue.PushNewElement(std::make_shared<Node>(next_stat));
    REQUIRE(my_queue.PopTopElement()->data.count_of_letters == 1);
}

TEST_CASE("EncodeAndDecode") {
    std::cout << std::filesystem::current_path();
    std::stringstream file, archive_file;
    std::string text = "Hello world!";
    file << text;
    std::string file_name = "../tasks/archiver/tests/output_files/abacaba.txt";
    BitWriter writer(archive_file);
    Encode(file_name, file, writer, true);
    archive_file.clear();
    archive_file.seekg(0);
    BitReader reader(archive_file);
    Decode(reader);
    std::fstream output_file(file_name);
    char current_char;
    size_t index = 0;
    for (; output_file >> std::noskipws >> current_char; ++index) {
        REQUIRE(current_char == text[index]);
    }
    REQUIRE(index == text.size());
}

TEST_CASE("ArchiveSize") {
    for (std::string file_name : {"../tasks/archiver/tests/input1.txt",
                                  "../tasks/archiver/tests/input2.txt",
                                  "../tasks/archiver/tests/input3.txt",}) {
        std::stringstream archive_file;
        BitWriter writer(archive_file);
        std::fstream fin(file_name);
        Encode(file_name, fin, writer, true);
        fin.clear();
        fin.seekg(0);
        size_t size_fin = 0;
        char ch;
        while (fin >> ch) {
            ++size_fin;
        }
        double compress_percent = static_cast<double>(archive_file.str().size()) / size_fin;
        std::cout << compress_percent << std::endl;
        if (file_name == "../tasks/archiver/tests/input1.txt") {
            REQUIRE(compress_percent < 0.5);
        } else if (file_name == "../tasks/archiver/tests/input2.txt") {
            REQUIRE(compress_percent < 1);
        } else {
            REQUIRE(compress_percent < 0.6);
        }
    }
}