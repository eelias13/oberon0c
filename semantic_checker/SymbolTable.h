//
// Manages all Identifiers within a scope of a Oberon0 Program
// Created by M on 15.12.2024.
//

#ifndef OBERON0C_SYMBOLTABLE_H
#define OBERON0C_SYMBOLTABLE_H

#include <vector>
#include <utility>
#include <unordered_map>
#include <map>
#include <optional>
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

struct TypeInfo
{

    bool operator!=(TypeInfo other);
    bool operator==(TypeInfo other);

    GeneralType general;
    std::string name;
    int array_dim;
    std::shared_ptr<TypeInfo> element_type;

    // Constructor that matches the parameters used in SymbolTable.cpp
    TypeInfo(GeneralType general, std::string name, int array_dim = 0, std::shared_ptr<TypeInfo> element_type = nullptr)
        : general(general), name(std::move(name)), array_dim(array_dim), element_type(std::move(element_type)) {}

    TypeInfo() : general(ERROR_TYPE), name(""), array_dim(0), element_type(nullptr) {}
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
    TypeInfo type;

    // Correct constructor (fixes the error)
    IdentInfo(std::string name, Kind kind, Node *node, TypeInfo type)
        : name(std::move(name)), kind(kind), node(node), type(std::move(type)) {}
        
    IdentInfo() : name(""), kind(ERROR_KIND), node(nullptr), type() {}
};

class SymbolTable
{

private:
    std::unordered_map<string, IdentInfo> table_;
    std::unordered_map<string, std::map<string, TypeInfo>> records_;

public:
    explicit SymbolTable() = default;

    void insert(const string &name, Kind k, Node *node, GeneralType general_type, string type = "");
    void insert(const string &name, Kind k, Node *node, const TypeInfo &type);
    void insert_array_type(const string &name, Node *node, TypeInfo *element_type, int dimension);
    void insert_record(const string &record_name, std::vector<std::pair<string, TypeInfo>> fields);

    IdentInfo *lookup(const std::string &name);
    TypeInfo *lookup_field(const string &record_name, const string &field_name);
    std::optional<std::map<string, TypeInfo>> lookup_record(const string &record_name);
};

#endif // OBERON0C_SYMBOLTABLE_H
