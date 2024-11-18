//
// Created by M on 18.11.2024.
//

#ifndef OBERON0C_WHILESTATEMENTNODE_H
#define OBERON0C_WHILESTATEMENTNODE_H

#include "Node.h"

class ExpressionNode;
class StatementSequenceNode;

class WhileStatementNode : Node {

    protected:
        std::unique_ptr<ExpressionNode> condition_;
        std::unique_ptr<StatementSequenceNode> statements_;

    public:

        //WhileStatementNode(const NodeType nodeType, FilePos pos,std::unique_ptr<ExpressionNode> condition,std::unique_ptr<StatementSequenceNode> statements) : Node(nodeType,pos), condition_(condition), statements_(statements) {};

        void accept(NodeVisitor &visitor) override;
        void print(std::ostream &stream) const override;
};


#endif //OBERON0C_WHILESTATEMENTNODE_H
