//
// Created by M on 18.11.2024.
//

#ifndef OBERON0C_IFSTATEMENTNODE_H
#define OBERON0C_IFSTATEMENTNODE_H
#include "Node.h"

#include <vector>

class ExpressionNode;
class StatementSequenceNode;

typedef std::pair<std::unique_ptr<ExpressionNode>, std::unique_ptr<StatementSequenceNode>> ElseIfPair;

class IfStatementNode : public Node {

    private:
        std::unique_ptr<ExpressionNode> condition_;
        std::unique_ptr<StatementSequenceNode> then_statements_;
        std::vector<ElseIfPair> else_ifs_ = {};
        std::unique_ptr<StatementSequenceNode> else_statements_ = nullptr;

    public:

        // Commented out until NodeTypes exist

        // IfStatementNode(const NodeType nodeType, FilePos pos, std::unique_ptr<ExpressionNode> condition, std::unique_ptr<StatementSequenceNode> then_statements) : Node(nodeType,pos), condition_(std::move(condition)), then_statements_(std::move(then_statements)) {};

        //void add_else_if(ElseIfPair elsif);
        //void add_else(std::unique_ptr<StatementSequenceNode> else_statements);

        void accept(NodeVisitor &visitor) override;
        void print(std::ostream &stream) const override;

};


#endif //OBERON0C_IFSTATEMENTNODE_H
