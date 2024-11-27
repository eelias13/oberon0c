//
// Created by M on 19.11.2024.
//

#ifndef OBERON0C_IDENTLISTNODE_H
#define OBERON0C_IDENTLISTNODE_H

#include <vector>
#include "parser/ast/Node.h"

class IdentNode;

class IdentListNode : public Node {

    private:
        std::vector<std::unique_ptr<IdentNode>> identifier_;

    public:

        IdentListNode(FilePos pos, std::unique_ptr<IdentNode> first_identifier);

        void add_identifier(std::unique_ptr<IdentNode> ident);

        void accept(NodeVisitor &visitor) override;
        void print(std::ostream &stream) const override;

};


#endif //OBERON0C_IDENTLISTNODE_H
