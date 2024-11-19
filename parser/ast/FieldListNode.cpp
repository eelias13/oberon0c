//
// Created by M on 19.11.2024.
//

#include "FieldListNode.h"

void FieldListNode::accept(NodeVisitor &visitor) {

}

void FieldListNode::print(ostream &stream) const {

    stream << fields_ << " : " << type_;

}
