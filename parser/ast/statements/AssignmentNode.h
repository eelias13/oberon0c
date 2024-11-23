//
// Created by M on 21.11.2024.
//

#ifndef OBERON0C_ASSIGNMENTNODE_H
#define OBERON0C_ASSIGNMENTNODE_H

#include "StatementNode.h"
#include "parser/ast/base_blocks/IdentNode.h"
#include "parser/ast/base_blocks/SelectorNode.h"
#include "parser/ast/arithmetic/ExpressionNode.h"

class AssignmentNode : public StatementNode{
    private:
        std::unique_ptr<IdentNode> variable_;
        std::unique_ptr<SelectorNode> selector_;
        std::unique_ptr<ExpressionNode> expr_;

    public:
        AssignmentNode(FilePos pos, std::unique_ptr<IdentNode> variable,std::unique_ptr<SelectorNode> selector, std::unique_ptr<ExpressionNode> expr) : StatementNode(NodeType::assignment,pos), variable_(std::move(variable)), selector_(std::move(selector)), expr_(std::move(expr)) {};

        void accept(NodeVisitor &visitor) override;
        void print(std::ostream &stream) const override;
};


#endif //OBERON0C_ASSIGNMENTNODE_H
