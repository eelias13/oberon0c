//
// Created by M on 19.11.2024.
//

#include "parser/ast/statements/ProcedureCallNode.h"

void ProcedureCallNode::accept(NodeVisitor &visitor) {

}

void ProcedureCallNode::print(ostream &stream) const {
    stream << *name_ << *selector_;

    if(parameters_) {
        stream << *parameters_;
    }
}
