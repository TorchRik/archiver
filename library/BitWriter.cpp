#include "BitWriter.h"

BitWriter::BitWriter(std::ostream& output_stream) : out_(output_stream) {
}

void BitWriter::WriteBits(const std::vector<bool> &bits) {
    for (size_t index = 0; index < bits.size();) {
        if (current_byte_size_ == 8) {
            out_ << current_byte_;
            current_byte_ = 0;
            current_byte_size_ = 0;
        }
        current_byte_ <<= 1;
        current_byte_ += bits[index++];
        ++current_byte_size_;
    }
}

void BitWriter::WriteBuffer() {
    for (int i = 0; i < 8 - current_byte_size_; i ++) {
        current_byte_ <<= 1;
    }
    current_byte_size_= 8;
    out_ << current_byte_;
}