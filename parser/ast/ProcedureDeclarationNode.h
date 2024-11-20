//
// Created by M on 20.11.2024.
//

#ifndef OBERON0C_PROCEDUREDECLARATIONNODE_H
#define OBERON0C_PROCEDUREDECLARATIONNODE_H

#include "ProcedureHeadingNode.h"
#include "ProcedureBodyNode.h"

class ProcedureDeclarationNode : Node {

    private:
        std::unique_ptr<ProcedureHeadingNode> heading_;
        std::unique_ptr<ProcedureBodyNode> body_;

    public:
        ProcedureDeclarationNode(const NodeType node_type, FilePos pos, std::unique_ptr<ProcedureHeadingNode> heading, std::unique_ptr<ProcedureBodyNode> body) : Node(node_type,pos),heading_(std::move(heading)),body_(std::move(body)) {};

        void accept(NodeVisitor &visitor) override;
        void print(std::ostream &stream) const override;


};


#endif //OBERON0C_PROCEDUREDECLARATIONNODE_H
