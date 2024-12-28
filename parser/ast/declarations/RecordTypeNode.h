//
// Created by M on 20.11.2024.
//

#ifndef OBERON0C_RECORDTYPENODE_H
#define OBERON0C_RECORDTYPENODE_H

#include <vector>
#include "parser/ast/declarations/TypeNode.h"

class IdentNode;

typedef std::unique_ptr<std::vector<std::unique_ptr<IdentNode>>> ident_list;

typedef std::pair<ident_list, std::unique_ptr<TypeNode>> field;

class RecordTypeNode : public TypeNode{

    private:
        std::vector<std::unique_ptr<field>> fields_;

    public:

        RecordTypeNode(FilePos pos, std::unique_ptr<field> first_field);

        void add_field_list(std::unique_ptr<field> field_list);

        void accept(NodeVisitor &visitor) override;
        void print(std::ostream &stream) const override;
};


#endif //OBERON0C_RECORDTYPENODE_H
