//
// Created by M on 20.11.2024.
//

#ifndef OBERON0C_RECORDTYPENODE_H
#define OBERON0C_RECORDTYPENODE_H

#include "parser/ast/declarations/FieldListNode.h"
#include "TypeNode.h"

class RecordTypeNode : TypeNode{

    private:
        std::vector<std::unique_ptr<FieldListNode>> fields_;

    public:

        RecordTypeNode(FilePos pos, std::unique_ptr<FieldListNode> first_field) : TypeNode(NodeType::record_type,pos) {fields_.emplace_back(std::move(first_field));};

        void add_field_list(std::unique_ptr<FieldListNode> field_list){ fields_.emplace_back(std::move(field_list));};

        void accept(NodeVisitor &visitor) override;
        void print(std::ostream &stream) const override;

};


#endif //OBERON0C_RECORDTYPENODE_H
