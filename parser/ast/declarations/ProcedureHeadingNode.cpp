//
// Created by M on 20.11.2024.
//

#include "ProcedureHeadingNode.h"

void ProcedureHeadingNode::accept(NodeVisitor &visitor) {

}

void ProcedureHeadingNode::print(ostream &stream) const {
    stream << "PROCEDURE " << name_;

    if(params_){
        stream << params_;
    }

}
