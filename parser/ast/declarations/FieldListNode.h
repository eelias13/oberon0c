//
// Created by M on 19.11.2024.
//

#ifndef OBERON0C_FIELDLISTNODE_H
#define OBERON0C_FIELDLISTNODE_H

#include "parser/ast/Node.h"
#include <memory>
#include <vector>

class IdentNode;
class TypeNode;

class FieldListNode : Node {
    private:

        std::unique_ptr<std::vector<std::unique_ptr<IdentNode>>> fields_;
        std::unique_ptr<TypeNode> type_;

    public:

    explicit FieldListNode(FilePos pos);
    explicit FieldListNode(FilePos pos, std::unique_ptr<std::vector<std::unique_ptr<IdentNode>>> fields, std::unique_ptr<TypeNode> type);

    void accept(NodeVisitor &visitor) override;
    void print(std::ostream &stream) const override;

};


#endif //OBERON0C_FIELDLISTNODE_H
