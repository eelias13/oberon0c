//
// Created by M on 19.11.2024.
//

#ifndef OBERON0C_IDENTLISTNODE_H
#define OBERON0C_IDENTLISTNODE_H

#include "IdentNode.h"
#include <vector>


class IdentListNode : Node {

    private:
        std::vector<std::unique_ptr<IdentNode>> identifier_;

    public:

        IdentListNode(FilePos pos, std::unique_ptr<IdentNode> first_identifier) : Node(NodeType::ident_list,pos) { identifier_.emplace_back(std::move(first_identifier));};

        void accept(NodeVisitor &visitor) override;
        void print(std::ostream &stream) const override;

};


#endif //OBERON0C_IDENTLISTNODE_H
