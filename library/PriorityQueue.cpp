#include "PriorityQueue.h"

PriorityQueue::PriorityQueue() {}

void PriorityQueue::ShiftDown(size_t index) {
    while (2 * index + 1 < queue_values.size()) {
        size_t left = 2 * index + 1, right = 2 * index + 2;
        size_t next_index = left;
        if (right < queue_values.size() and *queue_values[right] < *queue_values[left]) {
            next_index = right;
        }
        if (*queue_values[index] < *queue_values[next_index]) {
            break;
        }
        else {
            std::swap(queue_values[index], queue_values[next_index]);
            index = next_index;
        }
    }
}

void PriorityQueue::ShiftUp(size_t index) {
    while (index) {
        size_t up_index = (index - 1) / 2;
        if (*queue_values[up_index] < *queue_values[index]) {
            break;
        }
        std::swap(queue_values[up_index], queue_values[index]);
        index = up_index;
    }
}

std::shared_ptr<Node> PriorityQueue::PopTopElement() {
    if (!queue_values.size()) {
        throw std::overflow_error("Queue is empty");
    }
    std::shared_ptr<Node> first_node = move(queue_values[0]);
    std::swap(queue_values[0], queue_values[queue_values.size() - 1]);
    queue_values.pop_back();
    ShiftDown(0);
    return first_node;
}

void PriorityQueue::PushNewElement(std::shared_ptr<Node> element) {
    queue_values.push_back(element);
    ShiftUp(queue_values.size() - 1);
}

size_t PriorityQueue::Size() {
    return queue_values.size();
}