//
// Created by M on 21.11.2024.
//

#ifndef OBERON0C_IDENTNODE_H
#define OBERON0C_IDENTNODE_H

#include "parser/ast/declarations/TypeNode.h"
#include "semantic_checker/ScopeTable.h"

class IdentNode : public TypeNode {

    private:
        const string name_;
        Type formal_type_;
        Type actual_type_;
        TypeNode* type_node_;

    public:
        IdentNode(FilePos pos, const string name) : TypeNode(NodeType::ident,pos),name_(name) {};

        void accept(NodeVisitor &visitor) override;
        void print(std::ostream &stream) const override;

        string get_value();
        void set_types(Type formal, Type actual, TypeNode*node);
        Type get_formal_type();
        Type get_actual_type();
        TypeNode* get_type_node();

};


#endif //OBERON0C_IDENTNODE_H
