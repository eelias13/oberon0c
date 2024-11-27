//
// Created by M on 20.11.2024.
//

#include "ProcedureHeadingNode.h"
#include "parser/ast/declarations/FormalParameterNode.h"
#include "FPSectionNode.h"
#include "IdentListNode.h"
#include "parser/ast/base_blocks/IdentNode.h"


void ProcedureHeadingNode::accept(NodeVisitor &visitor) {

}

void ProcedureHeadingNode::print(ostream &stream) const {
    stream << "PROCEDURE " << *name_;

    if(params_){
        stream << *params_;
    }

}

ProcedureHeadingNode::ProcedureHeadingNode(FilePos pos, std::unique_ptr<IdentNode> name, std::unique_ptr<FormalParameterNode> params)  : Node(NodeType::procedure_heading, pos), name_(std::move(name)), params_(std::move(params)) {};