#pragma once

#include <vector>
#include <cstdint>
#include <fstream>

class BitReader {
public:
    BitReader(std::istream& input_stream);

    std::vector<bool> ReadBits(size_t count_bits);

private:
    char current_byte_ = 0;
    uint8_t count_read_bits_in_current_byte_ = 8;
    std::istream& fin_;

    bool UpdateCurrentByte();
};