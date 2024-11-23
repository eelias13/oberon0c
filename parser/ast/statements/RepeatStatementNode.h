//
// Created by M on 18.11.2024.
//

#ifndef OBERON0C_REPEATSTATEMENT_H
#define OBERON0C_REPEATSTATEMENT_H

#include "parser/ast/arithmetic/ExpressionNode.h"
#include "StatementSequenceNode.h"

class RepeatStatementNode : public StatementNode {

    private:
        std::unique_ptr<ExpressionNode> condition_;
        std::unique_ptr<StatementSequenceNode> statements_;

    public:
        RepeatStatementNode(FilePos pos, std::unique_ptr<ExpressionNode> condition, std::unique_ptr<StatementSequenceNode> statements) : StatementNode(NodeType::repeat_statement,pos), condition_(std::move(condition)), statements_(std::move(statements)){};

        void print(std::ostream &stream) const override;
        void accept(NodeVisitor &visitor) override;

};


#endif //OBERON0C_REPEATSTATEMENT_H
