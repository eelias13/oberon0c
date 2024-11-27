//
// Created by M on 20.11.2024.
//

#ifndef OBERON0C_PROCEDUREDECLARATIONNODE_H
#define OBERON0C_PROCEDUREDECLARATIONNODE_H

#include "parser/ast/Node.h"

class ProcedureHeadingNode;
class ProcedureBodyNode;

class ProcedureDeclarationNode : public Node {

    private:
        std::unique_ptr<ProcedureHeadingNode> heading_;
        std::unique_ptr<ProcedureBodyNode> body_;

    public:
        ProcedureDeclarationNode(FilePos pos, std::unique_ptr<ProcedureHeadingNode> heading, std::unique_ptr<ProcedureBodyNode> body);

        void accept(NodeVisitor &visitor) override;
        void print(std::ostream &stream) const override;


};


#endif //OBERON0C_PROCEDUREDECLARATIONNODE_H
