//
// Created by M on 20.11.2024.
//

#ifndef OBERON0C_PROCEDUREHEADINGNODE_H
#define OBERON0C_PROCEDUREHEADINGNODE_H

#include "parser/ast/Node.h"

class IdentNode;
class FormalParameterNode;


class ProcedureHeadingNode : public Node {

    private:
        std::unique_ptr<IdentNode> name_;
        std::unique_ptr<FormalParameterNode> params_;

    public:

        ProcedureHeadingNode(FilePos pos, std::unique_ptr<IdentNode> name, std::unique_ptr<FormalParameterNode> params = nullptr);

        void accept(NodeVisitor &visitor) override;
        void print(std::ostream &stream) const override;

};


#endif //OBERON0C_PROCEDUREHEADINGNODE_H
