#pragma once

#include <vector>
#include <memory>
#include "Node.h"

class PriorityQueue {
public:
    PriorityQueue();

    std::shared_ptr<Node> PopTopElement();
    void PushNewElement(std::shared_ptr<Node> element);
    size_t Size();

private:
    std::vector<std::shared_ptr<Node>> queue_values;

    void ShiftDown(size_t index);
    void ShiftUp(size_t index);
};