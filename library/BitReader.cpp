#include "BitReader.h"
#include "iostream"

BitReader::BitReader(std::istream& input_stream) : fin_(input_stream) {
}

bool BitReader::UpdateCurrentByte() {
    count_read_bits_in_current_byte_ = 0;
    if (fin_ >> std::noskipws >> current_byte_) {
        return true;
    }
    return false;
}


std::vector<bool> BitReader::ReadBits(size_t count_bits) {
    std::vector<bool> bits;
    while (bits.size() < count_bits) {
        if (count_read_bits_in_current_byte_ == 8) {
            if (!UpdateCurrentByte()) {
                throw std::runtime_error("index out of range");
            }
        }
        bool next_bit = (current_byte_ >> (7 - count_read_bits_in_current_byte_++)) % 2;
        bits.push_back(next_bit);
    }
    return bits;
}