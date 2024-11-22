//
// Created by M on 18.11.2024.
//

#ifndef OBERON0C_IFSTATEMENTNODE_H
#define OBERON0C_IFSTATEMENTNODE_H

#include "StatementNode.h"
#include "StatementSequenceNode.h"
#include "ExpressionNode.h"
#include <vector>

typedef std::pair<std::unique_ptr<ExpressionNode>, std::unique_ptr<StatementSequenceNode>> ElseIfPair;

class IfStatementNode : public StatementNode {

    private:
        std::unique_ptr<ExpressionNode> condition_;
        std::unique_ptr<StatementSequenceNode> then_statements_;
        std::vector<ElseIfPair> else_ifs_ = {};
        std::unique_ptr<StatementSequenceNode> else_statements_ = nullptr;

    public:

        IfStatementNode(FilePos pos, std::unique_ptr<ExpressionNode> condition, std::unique_ptr<StatementSequenceNode> then_statements) : StatementNode(NodeType::if_statement,pos), condition_(std::move(condition)), then_statements_(std::move(then_statements)) {};

        void add_else_if(std::unique_ptr<ExpressionNode> expr, std::unique_ptr<StatementSequenceNode> statements);
        void add_else(std::unique_ptr<StatementSequenceNode> else_statements);

        void accept(NodeVisitor &visitor) override;
        void print(std::ostream &stream) const override;

};


#endif //OBERON0C_IFSTATEMENTNODE_H
