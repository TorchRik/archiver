#pragma once

#include <vector>
#include <fstream>

class BitWriter {
public:
    BitWriter(std::ostream& output_stream);

    void WriteBits(const std::vector<bool>& bits);
    void WriteBuffer();
private:
    std::ostream& out_;
    unsigned char current_byte_ = 0;
    uint8_t current_byte_size_ = 0;
};