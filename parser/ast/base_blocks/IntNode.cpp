//
// Created by M on 21.11.2024.
//

#include "IntNode.h"

void IntNode::accept(NodeVisitor &visitor) {

}

void IntNode::print(ostream &stream) const {
    stream << value_;
}
