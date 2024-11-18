//
// Created by M on 18.11.2024.
//

#ifndef OBERON0C_REPEATSTATEMENT_H
#define OBERON0C_REPEATSTATEMENT_H

#include "WhileStatementNode.h"


class RepeatStatement : public WhileStatementNode {

    private:

    public:

        //RepeatStatement(const NodeType nodeType, FilePos pos,std::unique_ptr<StatementSequenceNode> statements, std::unique_ptr<ExpressionNode> condition) : WhileStatementNode(nodeType,pos,condition,statements){};

        void print(std::ostream &stream) const override;

};


#endif //OBERON0C_REPEATSTATEMENT_H
