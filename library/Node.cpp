#include "Node.h"

bool Node::operator<(const Node &node) const {
    return data < node.data;
}
