//
// Created by M on 20.11.2024.
//

#ifndef OBERON0C_MODULENODE_H
#define OBERON0C_MODULENODE_H

#include "parser/ast/statements/StatementSequenceNode.h"
#include "parser/ast/base_blocks/IdentNode.h"
#include "parser/ast/declarations/DeclarationsNode.h"

class ModuleNode : Node {

    private:
    std::unique_ptr<IdentNode> module_name_begin_;
    std::unique_ptr<DeclarationsNode> declarations_;
    std::unique_ptr<StatementSequenceNode> statements_;
    std::unique_ptr<IdentNode> module_name_end_;

    public:

    ModuleNode(FilePos pos, std::unique_ptr<IdentNode> name_start, std::unique_ptr<DeclarationsNode> declarations, std::unique_ptr<StatementSequenceNode> statements, std::unique_ptr<IdentNode> name_end)
            : Node(NodeType::module,pos),module_name_begin_(std::move(name_start)), declarations_(std::move(declarations)), statements_(std::move(statements)), module_name_end_(std::move(name_end)) {};


    void accept(NodeVisitor &visitor) override;
    void print(std::ostream &stream) const override;

};


#endif //OBERON0C_MODULENODE_H
