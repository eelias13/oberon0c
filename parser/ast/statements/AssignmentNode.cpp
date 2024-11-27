//
// Created by M on 21.11.2024.
//

#include "AssignmentNode.h"
#include "parser/ast/base_blocks/IdentNode.h"
#include "parser/ast/base_blocks/SelectorNode.h"
#include "parser/ast/arithmetic/ExpressionNode.h"
#include "parser/ast/arithmetic/SimpleExpressionNode.h"
#include "parser/ast/arithmetic/TermNode.h"
#include "parser/ast/arithmetic/FactorNode.h"

void AssignmentNode::accept(NodeVisitor &visitor) {

}

void AssignmentNode::print(ostream &stream) const {
    stream << *variable_;

    if(selector_){
        stream << *selector_;
    }

    stream << " := " << *expr_;
}

AssignmentNode::AssignmentNode(FilePos pos, std::unique_ptr<IdentNode> variable, std::unique_ptr<SelectorNode> selector, std::unique_ptr<ExpressionNode> expr)
        : StatementNode(NodeType::assignment,pos), variable_(std::move(variable)), selector_(std::move(selector)), expr_(std::move(expr)) {};