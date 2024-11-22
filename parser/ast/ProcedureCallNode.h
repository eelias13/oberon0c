//
// Created by M on 19.11.2024.
//

#ifndef OBERON0C_PROCEDURECALLNODE_H
#define OBERON0C_PROCEDURECALLNODE_H

#include "StatementNode.h"
#include "ActualParametersNode.h"
#include "IdentNode.h"
#include "SelectorNode.h"

class ProcedureCallNode : StatementNode {

    private:
        std::unique_ptr<IdentNode> name_;
        std::unique_ptr<SelectorNode> selector_;

        std::unique_ptr<ActualParametersNode> parameters_;

    public:

        explicit ProcedureCallNode(FilePos pos, std::unique_ptr<IdentNode> name, std::unique_ptr<SelectorNode> selector, std::unique_ptr<ActualParametersNode> parameters) : StatementNode(NodeType::procedure_call,pos), name_(std::move(name)), selector_(std::move(selector)), parameters_(std::move(parameters)) { };

        void accept(NodeVisitor &visitor) override;
        void print(std::ostream &stream) const override;


};


#endif //OBERON0C_PROCEDURECALLNODE_H
