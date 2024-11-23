//
// Created by M on 18.11.2024.
//

#include "WhileStatementNode.h"

void WhileStatementNode::accept(NodeVisitor &visitor) {

}

void WhileStatementNode::print(ostream &stream) const {

    stream << "WHILE " << *condition_ << " DO " << *statements_ << " END";

}
