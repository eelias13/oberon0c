//
// Created by M on 18.11.2024.
//

#ifndef OBERON0C_STATEMENTSEQUENCENODE_H
#define OBERON0C_STATEMENTSEQUENCENODE_H

#include "StatementNode.h"
#include <vector>


class StatementSequenceNode : public Node{

    private:
        std::vector<std::unique_ptr<StatementNode>> statements_;

    public:
        StatementSequenceNode(FilePos pos,std::unique_ptr<StatementNode> first_statement) : Node(NodeType::statement_sequence, pos) {statements_.emplace_back(std::move(first_statement));}

        void add_statement(std::unique_ptr<StatementNode> statement){statements_.emplace_back(std::move(statement));};

        void accept(NodeVisitor &visitor) override;
        void print(std::ostream &stream) const override;
};


#endif //OBERON0C_STATEMENTSEQUENCENODE_H
