#pragma once

#include <vector>
#include <LetterStatistic.h>
#include <memory>

struct Node {
    LetterStatistic data;
    std::shared_ptr<Node> left = nullptr;
    std::shared_ptr<Node> right = nullptr;

    bool operator<(const Node& node) const;
    Node(LetterStatistic data) : data(data) {}
    Node() = default;
};
