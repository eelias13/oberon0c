//
// Created by M on 18.11.2024.
//

#include "IfStatementNode.h"

void IfStatementNode::accept(NodeVisitor &visitor) {

}

void IfStatementNode::print(ostream &stream) const {
    stream << "IF " << *condition_ << " THEN " << *then_statements_;

    for(auto itr = else_ifs_.begin(); itr != else_ifs_.end(); itr++){
        stream << "ELSE IF " << *(itr->first) << " THEN " << *(itr->second);
    }

    if(else_statements_){
        stream << "ELSE " << *else_statements_;
    }

    stream << "END";

}


void IfStatementNode::add_else_if(std::unique_ptr<ExpressionNode> expr, std::unique_ptr<StatementSequenceNode> statements) {
    else_ifs_.emplace_back(std::move(expr),std::move(statements));
}

void IfStatementNode::add_else(std::unique_ptr<StatementSequenceNode> else_statements) {
    else_statements_ = std::move(else_statements);
}
