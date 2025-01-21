//
// Manages all Identifiers within a scope of a Oberon0 Program
// Created by M on 15.12.2024.
//

#ifndef OBERON0C_SYMBOLTABLE_H
#define OBERON0C_SYMBOLTABLE_H

#include <vector>
#include <utility>
#include <unordered_map>
#include "parser/ast/Node.h"

enum Kind
{
    PROCEDURE,
    CONSTANT,
    VARIABLE,
    TYPENAME,
    ERROR_KIND
};

enum GeneralType
{
    INTEGER,
    BOOLEAN,
    ERROR_TYPE,
    RECORD,
    ARRAY,
    ALIAS
};

struct Type{
    GeneralType general;
    string name;
    int array_dim = -1;
    std::shared_ptr<Type> element_type;
    bool operator!=(Type other);
    bool operator==(Type other);
};



/*
 *   --> Kind = What is this identifier used as (Procedure, Constant, Variable, Name of a type)
 *   --> Node in AST
 *              Kind        |   Stored Node
 *              ------------------------------------------
 *              Procedure   |   ProcedureDeclarationNode
 *              Constant    |   ExpressionNode
 *              Variable    |   TypeNode
 *              Typename    |   TypeNode
 *
 *   --> Type = Type of this variable as a string (empty for procedures)
 *
 *   A note on types:
 *         * Prebuilt types are "INTEGER", "BOOLEAN" and "_ERROR"
 *         * Array-Types are stored as "_ARRAY_[TYPE]_[DIMENSION]"
 *         * Record-Types are simply stored as "_RECORD"
 */
struct IdentInfo
{
    string name; // Useful since sometimes the name of the identifier may be "lost" along the way, e.g. when tracing
    Kind kind;
    Node *node;
    Type type;
};

class SymbolTable
{

private:
    std::unordered_map<string, IdentInfo> table_;
    std::unordered_map<string, std::unordered_map<string, Type>> records_;

public:
    explicit SymbolTable() = default;

    void insert(const string &name, Kind k, Node *node, GeneralType general_type, string type = "");
    void insert(const string &name, Kind k, Node *node, const Type& type);
    void insert_array_type(const string &name, Node* node,Type* element_type, int dimension);
    void insert_record(const string &record_name, std::vector<std::pair<string, Type>> fields);

    IdentInfo *lookup(const std::string &name);
    Type* lookup_field(const string &record_name, const string &field_name);
};

#endif // OBERON0C_SYMBOLTABLE_H
