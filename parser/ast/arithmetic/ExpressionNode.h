//
// Created by M on 21.11.2024.
//

#ifndef OBERON0C_EXPRESSIONNODE_H
#define OBERON0C_EXPRESSIONNODE_H

#include "SimpleExpressionNode.h"

enum expr_operator {EQ = 0, NEQ, LT, LEQ, GT, GEQ, ERR};

class ExpressionNode : Node {

    private:
        std::unique_ptr<SimpleExpressionNode> left_;
        expr_operator op_;
        std::unique_ptr<SimpleExpressionNode> right_;

        const static string op_to_string[7];
    public:
        ExpressionNode(FilePos pos, std::unique_ptr<SimpleExpressionNode> left, expr_operator op = ERR, std::unique_ptr<SimpleExpressionNode> right = nullptr);

        void accept(NodeVisitor &visitor) override;
        void print(std::ostream &stream) const override;

};


#endif //OBERON0C_EXPRESSIONNODE_H
