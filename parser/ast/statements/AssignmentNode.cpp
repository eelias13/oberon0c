//
// Created by M on 21.11.2024.
//

#include "AssignmentNode.h"

void AssignmentNode::accept(NodeVisitor &visitor) {

}

void AssignmentNode::print(ostream &stream) const {
    stream << variable_;

    if(selector_){
        stream << selector_;
    }

    stream << " := " << expr_;
}
