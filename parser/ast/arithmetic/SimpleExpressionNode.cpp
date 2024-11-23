//
// Created by M on 22.11.2024.
//

#include "SimpleExpressionNode.h"
#include "TermNode.h"

const string SimpleExpressionNode::op_to_string[] = {"+","-","OR","<NONE>"};

void SimpleExpressionNode::accept(NodeVisitor &visitor) {

}

void SimpleExpressionNode::print(ostream &stream) const {

    for(auto itr = expressions_.begin(); itr != expressions_.end(); itr++){

        if(itr->first != NONE){
            stream << " " << op_to_string[itr->first] << " ";
        }

        stream << *(itr->second);

    }

}

SimpleExpressionNode::SimpleExpressionNode(FilePos pos, std::unique_ptr<TermNode> initial_expr, simpexpr_op initial_op) : Node(NodeType::simple_expression,pos) {expressions_.emplace_back(initial_op, std::move(initial_expr));}

void SimpleExpressionNode::addTerm(std::unique_ptr<TermNode> term, simpexpr_op op) {expressions_.emplace_back(op,std::move(term));};