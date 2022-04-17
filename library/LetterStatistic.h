#pragma once

#include <vector>

struct LetterStatistic {
    std::vector<bool> letter_code;
    size_t count_of_letters = 0;

    bool operator<(const LetterStatistic& node) const;
};