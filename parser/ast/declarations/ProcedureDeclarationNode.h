//
// Created by M on 20.11.2024.
//

#ifndef OBERON0C_PROCEDUREDECLARATIONNODE_H
#define OBERON0C_PROCEDUREDECLARATIONNODE_H

#include "parser/ast/Node.h"
#include <vector>

class IdentNode;
class TypeNode;
class DeclarationsNode;
class StatementSequenceNode;

typedef std::tuple<bool,std::unique_ptr<std::vector<std::unique_ptr<IdentNode>>>,std::unique_ptr<TypeNode>> fp_section_t;

typedef std::vector<std::unique_ptr<fp_section_t>> parameters;

class ProcedureDeclarationNode : public Node {

    private:
        std::unique_ptr<IdentNode> begin_name_;
        std::unique_ptr<parameters> params_;

        std::unique_ptr<DeclarationsNode> declarations_;
        std::unique_ptr<StatementSequenceNode> statements_;
        std::unique_ptr<IdentNode> end_name_;


    public:
        ProcedureDeclarationNode(FilePos pos, std::unique_ptr<IdentNode> name, std::unique_ptr<parameters> params, std::unique_ptr<DeclarationsNode> declarations, std::unique_ptr<IdentNode> end_name, std::unique_ptr<StatementSequenceNode> statements = nullptr);

        void accept(NodeVisitor &visitor) override;
        void print(std::ostream &stream) const override;

        std::pair<IdentNode*,IdentNode*> get_names();
        parameters* get_parameters();
        DeclarationsNode* get_declarations();
        StatementSequenceNode* get_statements();

};


#endif //OBERON0C_PROCEDUREDECLARATIONNODE_H
