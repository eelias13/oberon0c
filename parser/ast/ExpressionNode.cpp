//
// Created by M on 21.11.2024.
//

#include "ExpressionNode.h"
#include "FactorNode.h"

const string ExpressionNode::op_to_string[] = {"=","#","<","<=",">",">=","ERROR"};


void ExpressionNode::accept(NodeVisitor &visitor) {

}

void ExpressionNode::print(ostream &stream) const {
    stream << left_;

    if(right_){
        stream << " " << op_to_string[op_] << " " << right_;
    }
}

ExpressionNode::ExpressionNode(NodeType node_type, FilePos pos, std::unique_ptr<SimpleExpressionNode> left, expr_operator op, std::unique_ptr<SimpleExpressionNode> right) :
        Node(node_type,pos), left_(std::move(left)), op_(op), right_(std::move(right)) {};
