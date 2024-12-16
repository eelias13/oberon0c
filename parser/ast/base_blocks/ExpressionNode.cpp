//
// Created by M on 21.11.2024.
//

#include "ExpressionNode.h"
#include "parser/ast/base_blocks/IdentNode.h"
#include "parser/ast/base_blocks/SelectorNode.h"

#include "parser/ast/NodeVisitor.h"

ExpressionNode::ExpressionNode(FilePos pos, const NodeType type)  : Node(type,pos) {}

int ExpressionNode::get_precedence() const {
    return precedence_;
}

Operator ExpressionNode::token_to_op(TokenType t) {
    switch(t){
        case TokenType::op_plus:
            return Operator::PLUS;
        case TokenType::op_minus:
            return Operator::MINUS;
        case TokenType::op_or:
            return Operator::OR;
        case TokenType::op_times:
            return Operator::MULT;
        case TokenType::op_div:
            return  Operator::DIV;
        case TokenType::op_mod:
            return Operator::MOD;
        case TokenType::op_and:
            return Operator::AND;
        case TokenType::op_not:
            return Operator::NOT;
        case TokenType::op_eq:
            return Operator::EQ;
        case TokenType::op_lt:
            return Operator::LT;
        case TokenType::op_leq:
            return Operator::LEQ;
        case TokenType::op_gt:
            return Operator::GT;
        case TokenType::op_geq:
            return Operator::GEQ;
        case TokenType::op_neq:
            return Operator::NEQ;
        default:
            return Operator::NO_OPERATOR;
    }
}

void ExpressionNode::print_operator(ostream &stream, Operator op) {
    switch (op) {
        case PLUS:
            stream << "+";
            break;
        case MINUS:
        case NEG:
            stream << "-";
            break;
        case OR:
            stream << "OR";
            break;
        case MULT:
            stream << "*";
            break;
        case DIV:
            stream << "DIV";
            break;
        case MOD:
            stream << "MOD";
            break;
        case AND:
            stream << "&";
            break;
        case NOT:
            stream << "~";
            break;
        case EQ:
            stream << "=";
            break;
        case NEQ:
            stream << "#";
            break;
        case LT:
            stream << "<";
            break;
        case LEQ:
            stream << "<=";
            break;
        case GT:
            stream << ">";
            break;
        case GEQ:
            stream << ">=";
            break;
        case PAREN:
            return;
        case NO_OPERATOR:
            stream << "<ERROR_OP>";
            break;

    }
}

std::ostream &operator<<(ostream &stream, const Operator op) {
    ExpressionNode::print_operator(stream,op);
    return stream;
}

int ExpressionNode::op_to_precedence(Operator op) {
    switch (op) {
        // Lowest Precedence --> Top-level boolean operators like =,#,<,...
        case EQ:
        case NEQ:
        case LT:
        case LEQ:
        case GT:
        case GEQ:
            return 0;
        // Precedence of 1 --> Term Operators like +,- (the operator),OR
        case PLUS:
        case MINUS:
        case OR:
            return 1;
        // Precedence of 2 --> Factor Operators like *,DIV,MOD,...
        case MULT:
        case DIV:
        case MOD:
        case AND:
            return 2;
        // Highest Precedence --> Unary Operators like - (the sign), ~ and also parantheses
        case NEG:
        case NOT:
        case PAREN:
            return 3;
        // Error Precedence
        case NO_OPERATOR:
            return -1;
    }
};


UnaryExpressionNode::UnaryExpressionNode(FilePos pos, std::unique_ptr<ExpressionNode> expr, Operator op) : ExpressionNode(pos,NodeType::unary_expression), expr_(std::move(expr)), op_(op){
    precedence_ = op_to_precedence(op); // Should normally be equal to 2
}

void UnaryExpressionNode::accept(NodeVisitor &visitor) {
}

void UnaryExpressionNode::print(ostream &stream) const {

    if(op_ == Operator::PAREN){
        stream << "(" << *expr_ << ")";

    }else{
        stream << op_ << " " << *(expr_);
    }

}

ExpressionNode *UnaryExpressionNode::get_expr() {
    return expr_.get();
}

Operator UnaryExpressionNode::get_op() {
    return op_;
}

BinaryExpressionNode::BinaryExpressionNode(FilePos pos, std::unique_ptr<ExpressionNode> lhs, Operator op, std::unique_ptr<ExpressionNode> rhs) : ExpressionNode(pos, NodeType::binary_expression), lhs_(std::move(lhs)), op_(op), rhs_(std::move(rhs)) {
    precedence_ = op_to_precedence(op);
}

void BinaryExpressionNode::accept(NodeVisitor &visitor) {

}

void BinaryExpressionNode::print(ostream &stream) const {
    stream << *lhs_ << " " << op_ << " " << *rhs_;
}

BinaryExpressionNode* BinaryExpressionNode::insert_rightmost(Operator op, std::unique_ptr<ExpressionNode> new_rhs) {
    rhs_ = std::make_unique<BinaryExpressionNode>(rhs_->pos(), std::move(rhs_), op, std::move(new_rhs));
    return dynamic_cast<BinaryExpressionNode*>(rhs_.get());
}

ExpressionNode* BinaryExpressionNode::get_rhs() {
    return rhs_.get();
}

ExpressionNode *BinaryExpressionNode::get_lhs() {
    return lhs_.get();
}

Operator BinaryExpressionNode::get_op() {
    return op_;
}


IdentSelectorExpressionNode::IdentSelectorExpressionNode(FilePos pos, std::unique_ptr<IdentNode> ident,std::unique_ptr<SelectorNode> selector) : ExpressionNode(pos, NodeType::ident_selector_expression), ident_(std::move(ident)), selector_(std::move(selector)){}

void IdentSelectorExpressionNode::accept(NodeVisitor &visitor) {
    visitor.visit(*this);
}

void IdentSelectorExpressionNode::print(ostream &stream) const {
    stream << *ident_ << *selector_;
}
