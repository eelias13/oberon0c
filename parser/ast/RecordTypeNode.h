//
// Created by M on 20.11.2024.
//

#ifndef OBERON0C_RECORDTYPENODE_H
#define OBERON0C_RECORDTYPENODE_H

#include "FieldListNode.h"

class RecordTypeNode : Node{

    private:
        std::vector<std::unique_ptr<FieldListNode>> fields_;

    public:

        RecordTypeNode(const NodeType node_type, FilePos pos, std::unique_ptr<FieldListNode> first_field) : Node(node_type,pos) {fields_.emplace_back(std::move(first_field));};

        void add_field_list(std::unique_ptr<FieldListNode> field_list){ fields_.emplace_back(std::move(field_list));};

        void accept(NodeVisitor &visitor) override;
        void print(std::ostream &stream) const override;

};


#endif //OBERON0C_RECORDTYPENODE_H
