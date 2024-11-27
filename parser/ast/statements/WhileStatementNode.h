//
// Created by M on 18.11.2024.
//

#ifndef OBERON0C_WHILESTATEMENTNODE_H
#define OBERON0C_WHILESTATEMENTNODE_H

#include "parser/ast/statements/StatementNode.h"

class ExpressionNode;
class StatementSequenceNode;

class WhileStatementNode : public StatementNode {

    protected:
        std::unique_ptr<ExpressionNode> condition_;
        std::unique_ptr<StatementSequenceNode> statements_;

    public:

        WhileStatementNode(FilePos pos,std::unique_ptr<ExpressionNode> condition,std::unique_ptr<StatementSequenceNode> statements);
        void accept(NodeVisitor &visitor) override;
        void print(std::ostream &stream) const override;
};


#endif //OBERON0C_WHILESTATEMENTNODE_H
