//
// Created by M on 21.11.2024.
//

#ifndef OBERON0C_DECLARATIONSNODE_H
#define OBERON0C_DECLARATIONSNODE_H

#include "IdentListNode.h"
#include "ProcedureDeclarationNode.h"
#include "parser/ast/base_blocks/IdentNode.h"
#include "parser/ast/arithmetic/ExpressionNode.h"

typedef std::pair<std::unique_ptr<IdentNode>,std::unique_ptr<ExpressionNode>> id_expr_pair;
typedef std::pair<std::unique_ptr<IdentNode>,std::unique_ptr<TypeNode>> id_type_pair;
typedef std::pair<std::unique_ptr<IdentListNode>,std::unique_ptr<TypeNode>> idlist_type_pair;

class DeclarationsNode : Node {

    private:
        std::vector<id_expr_pair> const_list_;
        std::vector<id_type_pair> type_list_;
        std::vector<idlist_type_pair> var_list;

        std::vector<std::unique_ptr<ProcedureDeclarationNode>> procedure_list;

    public:

        explicit DeclarationsNode(FilePos pos) : Node(NodeType::declarations,pos){};

        void add_constant(std::unique_ptr<IdentNode> ident, std::unique_ptr<ExpressionNode> expr){const_list_.emplace_back(std::move(ident),std::move(expr));};
        void add_type(std::unique_ptr<IdentNode> ident, std::unique_ptr<TypeNode> type){type_list_.emplace_back(std::move(ident),std::move(type));};
        void add_var(std::unique_ptr<IdentListNode> idents, std::unique_ptr<TypeNode> type) {var_list.emplace_back(std::move(idents), std::move(type));};
        void add_procedure(std::unique_ptr<ProcedureDeclarationNode> procedure){procedure_list.emplace_back(std::move(procedure));};

        void accept(NodeVisitor &visitor) override;
        void print(std::ostream &stream) const override;


};


#endif //OBERON0C_DECLARATIONSNODE_H
