/*
 * Parser of the Oberon-0 compiler.
 *
 * Created by Michael Grossniklaus on 2/2/18.
 */

#ifndef OBERON0C_PARSER_H
#define OBERON0C_PARSER_H

#include <string>
#include "scanner/Scanner.h"
#include "ast/Node.h"

#include "ast/base_blocks/IdentNode.h"
#include "ast/base_blocks/IntNode.h"
#include "ast/base_blocks/SelectorNode.h"

#include "ast/arithmetic/FactorNode.h"
#include "ast/arithmetic/TermNode.h"
#include "ast/arithmetic/SimpleExpressionNode.h"
#include "ast/arithmetic/ExpressionNode.h"

#include "ast/declarations/DeclarationsNode.h"
#include "ast/declarations/TypeNode.h"
#include "ast/declarations/ProcedureDeclarationNode.h"
#include "ast/declarations/IdentListNode.h"
#include "ast/declarations/ProcedureBodyNode.h"
#include "ast/declarations/FormalParameterNode.h"
#include "ast/declarations/FPSectionNode.h"
#include "ast/declarations/FieldListNode.h"
#include "ast/declarations/ArrayTypeNode.h"
#include "parser/ast/statements/ProcedureCallNode.h"
#include "ast/declarations/ProcedureHeadingNode.h"
#include "ast/declarations/ProcedureBodyNode.h"
#include "ast/declarations/RecordTypeNode.h"

#include "ast/statements/StatementSequenceNode.h"
#include "ast/statements/StatementNode.h"
#include "ast/statements/ActualParametersNode.h"
#include "ast/statements/WhileStatementNode.h"
#include "ast/statements/AssignmentNode.h"
#include "ast/statements/IfStatementNode.h"
#include "ast/statements/RepeatStatementNode.h"

#include "ast/ModuleNode.h"

using std::string;

class Parser
{

private:
    Scanner &scanner_;
    Logger &logger_;
    bool has_error;

    std::unique_ptr<Token> expect(TokenType);
    std::unique_ptr<Token> expect_many(std::vector<TokenType>);
    bool if_next(TokenType);

    FilePos pos();

    std::unique_ptr<IdentNode> ident();
    std::unique_ptr<IntNode> integer();
    std::unique_ptr<SelectorNode> selector();
    std::unique_ptr<IntNode> number();
    std::unique_ptr<FactorNode> factor();
    std::unique_ptr<TermNode> term();

    std::unique_ptr<SimpleExpressionNode> simple_expression();
    std::unique_ptr<ExpressionNode> expression();
    std::unique_ptr<AssignmentNode> assignment();

    std::unique_ptr<ActualParametersNode> actual_parameters();
    std::unique_ptr<ProcedureCallNode> procedure_call();

    std::unique_ptr<IfStatementNode> if_statement();
    std::unique_ptr<WhileStatementNode> while_statement();
    std::unique_ptr<RepeatStatementNode> repeat_statement();
    std::unique_ptr<StatementNode> statement();
    std::unique_ptr<StatementSequenceNode> statement_sequence();

    std::unique_ptr<IdentListNode> ident_list();
    std::unique_ptr<FieldListNode> field_list();

    std::unique_ptr<TypeNode> type();
    std::unique_ptr<ArrayTypeNode> array_type();
    std::unique_ptr<RecordTypeNode> record_type();

    std::unique_ptr<FPSectionNode> fp_section();
    std::unique_ptr<FormalParameterNode> formal_parameters();

    std::unique_ptr<ProcedureHeadingNode> procedure_heading();
    std::unique_ptr<ProcedureBodyNode> procedure_body();
    std::unique_ptr<ProcedureDeclarationNode> procedure_declaration();
    std::unique_ptr<DeclarationsNode> declarations();

    std::unique_ptr<ModuleNode> module();

public:
    explicit Parser(Scanner &scanner, Logger &logger) : scanner_(scanner), logger_(logger) {};
    ~Parser() = default;

    std::unique_ptr<ModuleNode> parse();
};

#endif // OBERON0C_PARSER_H
