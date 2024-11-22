//
// Created by M on 18.11.2024.
//

#include "RepeatStatementNode.h"

void RepeatStatementNode::accept(NodeVisitor &visitor) {

}

void RepeatStatementNode::print(ostream &stream) const {
    stream << "REPEAT " << statements_ << " UNTIL " << condition_;
}
