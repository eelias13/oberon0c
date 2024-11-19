//
// Created by M on 19.11.2024.
//

#ifndef OBERON0C_FIELDLISTNODE_H
#define OBERON0C_FIELDLISTNODE_H

#include "Node.h"
#include "IdentListNode.h"

class TypeNode;

class FieldListNode : Node {
    private:

        std::unique_ptr<IdentListNode> fields_;
        std::unique_ptr<TypeNode> type_;

    public:

    explicit FieldListNode(const NodeType nodeType, FilePos pos, std::unique_ptr<IdentListNode> fields, std::unique_ptr<TypeNode> type) : Node(nodeType,pos),fields_(std::move(fields)),type_(std::move(type)) {};

    void accept(NodeVisitor &visitor) override;
    void print(std::ostream &stream) const override;

};


#endif //OBERON0C_FIELDLISTNODE_H
