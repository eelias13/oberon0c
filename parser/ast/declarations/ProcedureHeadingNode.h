//
// Created by M on 20.11.2024.
//

#ifndef OBERON0C_PROCEDUREHEADINGNODE_H
#define OBERON0C_PROCEDUREHEADINGNODE_H

#include "parser/ast/declarations/FormalParameterNode.h"

class ProcedureHeadingNode : Node {

    private:
        std::unique_ptr<IdentNode> name_;
        std::unique_ptr<FormalParameterNode> params_;

    public:

        ProcedureHeadingNode(FilePos pos, std::unique_ptr<IdentNode> name, std::unique_ptr<FormalParameterNode> params = nullptr) : Node(NodeType::procedure_heading, pos), name_(std::move(name)), params_(std::move(params)) {};

        void accept(NodeVisitor &visitor) override;
        void print(std::ostream &stream) const override;

};


#endif //OBERON0C_PROCEDUREHEADINGNODE_H
