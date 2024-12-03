// Implementation of the Parser used by the Oberon0 Compiler
//
// Created by Max Galetskiy on 16.11.2024.
//

#include "Parser.h"
#include "../scanner/IdentToken.h"
#include "../util/panic.h"

std::unique_ptr<const Token> Parser::expect(TokenType exprect)
{

    if (scanner_.peek()->type() == exprect)
    {
        auto token = scanner_.next();
        return token;
    }

    std::ostringstream exprected;
    std::ostringstream got;

    got << scanner_.peek()->type();
    exprected << exprect;

    std::ostringstream msg;

    msg << "Syntax Error (Expected ";
    msg << exprected.str();
    msg << " but got ";
    msg << got.str();
    msg << ")";

    logger_.error("", msg.str());
    this->has_error = true;
    return nullptr;
}

std::unique_ptr<const Token> Parser::expect_many(std::vector<TokenType> tokens)
{

    auto token = scanner_.next();
    std::ostringstream exprected;

    exprected << "[";

    for (auto exprect : tokens)
    {
        if (exprect == token->type())
        {
            return token;
        }
        exprected << exprect << ", ";
    }
    exprected << "]";

    std::ostringstream got;

    got << token->type();

    std::ostringstream msg;

    msg << "Syntax Error (Expected ";
    msg << exprected.str();
    msg << " but got ";
    msg << got.str();
    msg << ")";

    logger_.error("", msg.str());
    this->has_error = true;
    return nullptr;
}

bool Parser::if_next(TokenType exprect)
{
    return scanner_.peek()->type() == exprect;
}

// ident -> letter (letter | digit)*  (already recognized by the scanner in full)
std::unique_ptr<IdentNode> Parser::ident()
{
    logger_.info("Ident");
    auto start = scanner_.peek()->start();

    auto token = this->expect(TokenType::const_ident);
    if (!token)
    {
        return nullptr;
    }

    auto ident_token = dynamic_cast<const IdentToken *>(token.get());
    return std::make_unique<IdentNode>(start, ident_token->value()); // success
}

// integer -> digit (digit)*  (already recognized by the scanner in full)
std::unique_ptr<IntNode> Parser::integer()
{
    logger_.info("Integer");

    auto token = this->expect_many({TokenType::int_literal, TokenType::short_literal, TokenType::long_literal});
    if (!token)
    {
        // can not handle otherwise
        return nullptr;
    }

    long int_value;
    switch (token->type())
    {
    case TokenType::int_literal:
        int_value = dynamic_cast<const IntLiteralToken *>(token.get())->value();
        break;
    case TokenType::short_literal:
        int_value = dynamic_cast<const ShortLiteralToken *>(token.get())->value();
        break;
    case TokenType::long_literal:
        int_value = dynamic_cast<const LongLiteralToken *>(token.get())->value();
        break;
    default:
        panic("unreachable");
    }

    return std::make_unique<IntNode>(token->start(), int_value);
}

// number -> integer
std::unique_ptr<IntNode> Parser::number()
{
    return integer();
}

// selector -> ("." ident | "[" expression "]" )*
std::unique_ptr<SelectorNode> Parser::selector()
{
    logger_.info("Selector");
    auto start = scanner_.peek()->start();
    auto selector = std::make_unique<SelectorNode>(start);

    while (true)
    {

        if (this->if_next(TokenType::period))
        {
            this->expect(TokenType::period);
            auto identifier = ident();
            selector->add_field(std::move(identifier));
        }
        else if (this->if_next(TokenType::lbrack))
        {
            this->expect(TokenType::lbrack);
            auto expr = expression();

            this->expect(TokenType::rbrack);
            selector->add_index(std::move(expr));
        }
        else
        {
            break;
        }
    }

    return selector;
}

// factor -> ident selector | number | "(" expression ")" | "~" factor
std::unique_ptr<FactorNode> Parser::factor()
{
    logger_.info("Factor");
    auto start = scanner_.peek()->start();

    if (this->if_next(TokenType::const_ident))
    {
        auto id = ident();
        auto sel = selector();
        return std::make_unique<IdentSelectorFactorNode>(start, std::move(id), std::move(sel));
    }
    else if (this->if_next(TokenType::lparen))
    {
        scanner_.next();
        auto expr = expression();
        this->expect(TokenType::rparen);
        return std::make_unique<ExpressionInFactorNode>(start, std::move(expr));
    }
    else if (this->if_next(TokenType::op_not))
    {
        auto factor_token = scanner_.next();
        auto fact = factor();
        return std::make_unique<NegatedFactorNode>(factor_token->start(), std::move(fact));
    }
    else
    {
        return number();
    }
}

// term -> factor (("*" | "DIV" | "MOD" | "&") factor)*
std::unique_ptr<TermNode> Parser::term()
{
    logger_.info("Term");

    auto start = scanner_.peek()->start();
    auto fact = factor();
    auto token_type = scanner_.peek()->type();

    auto term_node = std::make_unique<TermNode>(start, std::move(fact));

    while (token_type == TokenType::op_times || token_type == TokenType::op_div ||
           token_type == TokenType::op_mod || token_type == TokenType::op_and)
    {

        term_ops op;
        switch (token_type)
        {
        case TokenType::op_times:
            op = term_ops::MULT;
            break;
        case TokenType::op_div:
            op = term_ops::DIV;
            break;
        case TokenType::op_mod:
            op = term_ops::MOD;
            break;
        case TokenType::op_and:
            op = term_ops::AND;
            break;
        default:
            panic("unreachable");
        }

        scanner_.next();
        term_node->addFactor(op, factor());
        token_type = scanner_.peek()->type();
    }

    return term_node;
}

// SimpleExpression -> ("+" | "-" | €) term (("+"|"-"|"OR") term)*
std::unique_ptr<SimpleExpressionNode> Parser::simple_expression()
{

    logger_.info("Simple Expression");

    auto start = scanner_.peek()->start();
    auto token = scanner_.peek();
    auto op = simpexpr_op::NONE;

    if (token->type() == TokenType::op_plus || token->type() == TokenType::op_minus)
    {
        op = (token->type() == TokenType::op_plus) ? simpexpr_op::PLUS : simpexpr_op::OR;
        scanner_.next();
    }
    auto fst_term = term();

    auto simp_expr = std::make_unique<SimpleExpressionNode>(start, std::move(fst_term), op);

    token = scanner_.peek();
    while (token->type() == TokenType::op_plus || token->type() == TokenType::op_minus || token->type() == TokenType::op_or)
    {

        simpexpr_op op;
        switch (token->type())
        {
        case TokenType::op_plus:
            op = simpexpr_op::PLUS;
            break;
        case TokenType::op_minus:
            op = simpexpr_op::MINUS;
            break;
        case TokenType::op_or:
            op = simpexpr_op::OR;
            break;
        default:
            panic("unreachable");
        }

        scanner_.next();
        auto trm = term();
        simp_expr->addTerm(std::move(trm), op);

        token = scanner_.peek();
    }

    return simp_expr;
}

// Expression -> SimpleExpression (("="|"#"|"<"|"<="|">"|">=") SimpleExpression)?
std::unique_ptr<ExpressionNode> Parser::expression()
{
    logger_.info("Expression");
    auto start = scanner_.peek()->start();
    auto fst_expr = simple_expression();

    auto token_type = scanner_.peek()->type();
    if (token_type == TokenType::op_eq || token_type == TokenType::op_neq ||
        token_type == TokenType::op_lt || token_type == TokenType::op_leq ||
        token_type == TokenType::op_gt || token_type == TokenType::op_geq)
    {

        expr_operator op;
        switch (token_type)
        {
        case TokenType::op_eq:
            op = expr_operator::EQ;
            break;
        case TokenType::op_neq:
            op = expr_operator::NEQ;
            break;
        case TokenType::op_lt:
            op = expr_operator::LT;
            break;
        case TokenType::op_leq:
            op = expr_operator::LEQ;
            break;
        case TokenType::op_gt:
            op = expr_operator::GT;
            break;
        case TokenType::op_geq:
            op = expr_operator::GEQ;
            break;
        default:
            panic("unreachable");
        }

        scanner_.next();
        auto snd_expr = simple_expression();

        return std::make_unique<ExpressionNode>(start, std::move(fst_expr), op, std::move(snd_expr));
    }
    else
    {
        return std::make_unique<ExpressionNode>(start, std::move(fst_expr));
    }
}

// Assignment -> Ident Selector ":=" Expression
std::unique_ptr<AssignmentNode> Parser::assignment()
{
    logger_.info("Assignment");

    auto start = scanner_.peek()->start();
    auto id = ident();
    auto sel = selector();

    this->expect(TokenType::op_becomes); // Syntax Error (Expected ":=", but got...) (Special Cases: Did you put "=" instead ":=" ? Did you forget the "=" after the ":")

    scanner_.next();
    auto expr = expression();
    return std::make_unique<AssignmentNode>(start, std::move(id), std::move(sel), std::move(expr));
}

// ActualParameters -> "(" (expression ("," expression)*)? ")"
std::unique_ptr<ActualParametersNode> Parser::actual_parameters()
{
    logger_.info("Actual Parameters");
    auto token = scanner_.peek();

    auto actual_params = std::make_unique<ActualParametersNode>(token->start());

    this->expect(TokenType::lparen);

    // Case: No expression
    if (this->if_next(TokenType::rparen))
    {
        scanner_.next();
        return actual_params; // Success
    }

    // Case: (At Least) One Expression
    actual_params->add_expression(expression());

    // Multiple Expressions
    while (this->if_next(TokenType::comma))
    {
        scanner_.next();
        actual_params->add_expression(expression());
    }

    this->expect(TokenType::rparen);
    return actual_params;
}

// ProcedureCall -> Ident Selector (ActualParameters)?
std::unique_ptr<ProcedureCallNode> Parser::procedure_call()
{
    logger_.info("Procedure Call");

    auto start = scanner_.peek()->start();
    auto id = ident();
    auto sel = selector();

    // Check for parenthesis to see whether ActualParameters term exists
    if (this->if_next(TokenType::lparen))
    {
        auto params = actual_parameters();
        return std::make_unique<ProcedureCallNode>(start, std::move(id), std::move(sel), std::move(params));
    }

    return std::make_unique<ProcedureCallNode>(start, std::move(id), std::move(sel));
}

// IfStatement -> "IF" expression "THEN" StatementSequence ("ELSIF" expression "THEN" StatementSequence)* ("ELSE" StatementSequence)? "END"
std::unique_ptr<IfStatementNode> Parser::if_statement()
{
    logger_.info("If Statement");

    auto start = scanner_.peek()->start();
    this->expect(TokenType::kw_if);
    auto condition = expression();
    this->expect(TokenType::kw_then);
    auto then = statement_sequence();

    // CONSTRUCT NODE
    auto if_node = std::make_unique<IfStatementNode>(start, std::move(condition), std::move(then));

    // ELSE IF
    while (this->if_next(TokenType::kw_elsif))
    {
        scanner_.next();
        auto elif_cond = expression();
        std::unique_ptr<StatementSequenceNode> elif_then;
        this->expect(TokenType::kw_then);
        elif_then = statement_sequence();
        if_node->add_else_if(std::move(elif_cond), std::move(elif_then));
    }

    // ELSE
    if (this->if_next(TokenType::kw_else))
    {
        scanner_.next();
        if_node->add_else(statement_sequence());
    }

    this->expect(TokenType::kw_end);
    return if_node; // All successful
}

// WhileStatement -> "WHILE" expression "DO" StatementSequence "END"
std::unique_ptr<WhileStatementNode> Parser::while_statement()
{
    logger_.info("While Statement");
    auto start = scanner_.peek()->start();
    this->expect(TokenType::kw_while);
    auto cond = expression();
    this->expect(TokenType::kw_do);
    auto statements = statement_sequence();
    this->expect(TokenType::kw_end);
    return std::make_unique<WhileStatementNode>(start, std::move(cond), std::move(statements)); // successful;
}

// RepeatStatement -> "REPEAT" StatementSequence "UNTIL" expression
std::unique_ptr<RepeatStatementNode> Parser::repeat_statement()
{
    logger_.info("Repeat Statement");
    auto start = scanner_.peek()->start();
    this->expect(TokenType::kw_repeat);
    auto statements = statement_sequence();
    this->expect(TokenType::kw_until);
    auto cond = expression();
    return std::make_unique<RepeatStatementNode>(start, std::move(cond), std::move(statements));
}

// Statement -> (assignment | Procedure Call | IfStatement | WhileStatement)
// Note: While not included in the CompilerConstruction book, this should contain "RepeatStatement" as well
std::unique_ptr<StatementNode> Parser::statement()
{

    logger_.info("Statement");

    auto start = scanner_.peek()->start();

    // IfStatement
    if (this->if_next(TokenType::kw_if))
    {
        return if_statement();
    }

    // WhileStatement
    else if (this->if_next(TokenType::kw_while))
    {
        return while_statement();
    }

    // RepeatStatement
    else if (this->if_next(TokenType::kw_repeat))
    {
        return repeat_statement();
    }
    // Check the next token to decide which Non-terminal "Statement" is derived into
    else
    {

        // If the token is an identifier then it could either correspond to an assignment or a procedure call
        // Thus Oberon-0 is not "really" an LL(1) language, but for the parser this makes little difference

        // Recall:
        //  assignment -> ident selector ":=" expression
        //  procedureCall -> ident selector [ActualParameters]?

        auto id = ident();
        auto sel = selector();

        // Assignment
        if (this->if_next(TokenType::op_becomes))
        {
            scanner_.next();
            auto expr = expression();
            return std::make_unique<AssignmentNode>(start, std::move(id), std::move(sel), std::move(expr));
        }

        // Procedure Call
        // Check for "(" to see whether the (ActualParameters) term exists
        if (this->if_next(TokenType::lparen))
        {
            auto params = actual_parameters();
            return std::make_unique<ProcedureCallNode>(start, std::move(id), std::move(sel), std::move(params));
        }
        return std::make_unique<ProcedureCallNode>(start, std::move(id), std::move(sel));
    }
}

// StatementSequence -> Statement (";" statement)*
std::unique_ptr<StatementSequenceNode> Parser::statement_sequence()
{
    logger_.info("Statement Sequence");
    auto start = scanner_.peek()->start();
    auto first = statement();
    auto sequence = std::make_unique<StatementSequenceNode>(start, std::move(first));
    while (this->if_next(TokenType::semicolon))
    {
        scanner_.next();
        sequence->add_statement(statement());
    }

    return sequence;
}

// IdentList -> Ident ("," ident)*
std::unique_ptr<std::vector<std::unique_ptr<IdentNode>>> Parser::ident_list()
{
    logger_.info("Ident List");
    auto start = scanner_.peek()->start();
    auto id_list = std::make_unique<std::vector<std::unique_ptr<IdentNode>>>();
    id_list->emplace_back(ident());

    while (this->if_next(TokenType::comma))
    {
        scanner_.next();
        id_list->emplace_back(ident());
    }

    return id_list;
}

// FieldList -> (IdentList ":" type)?
std::unique_ptr<FieldListNode> Parser::field_list()
{
    logger_.info("Field List");
    auto start = scanner_.peek()->start();
    // Check Follows(FieldList) to decide whether this FieldList is empty or not
    // Follows(FieldList) = ; END
    if (this->if_next(TokenType::semicolon) || this->if_next(TokenType::kw_end))
    {
        return std::make_unique<FieldListNode>(start); // Field List empty, continue parsing in upper layer
    }

    auto idents = ident_list();
    this->expect(TokenType::colon);
    auto idents_type = type();
    return std::make_unique<FieldListNode>(start, std::move(idents), std::move(idents_type));
}

// Type -> Ident | ArrayType | RecordType
std::unique_ptr<TypeNode> Parser::type()
{
    logger_.info("Type");

    // We decide the according production by the next token

    // ArrayType
    if (this->if_next(TokenType::kw_array))
    {
        return array_type();
    }
    // RecordType
    else if (this->if_next(TokenType::kw_record))
    {
        return record_type();
    }
    else
    {
        // Identifier
        return ident();
    }
}

// ArrayType -> "ARRAY" expression "OF" type
std::unique_ptr<ArrayTypeNode> Parser::array_type()
{
    logger_.info("Array Type");
    auto start = scanner_.peek()->start();
    this->expect(TokenType::kw_array);
    auto expr = expression();
    this->expect(TokenType::kw_of);
    auto arr_type = type();
    return std::make_unique<ArrayTypeNode>(start, std::move(expr), std::move(arr_type));
}

// RecordType = "RECORD" FieldList (";" FieldList)* "END"
std::unique_ptr<RecordTypeNode> Parser::record_type()
{
    logger_.info("Record Type");
    auto start = scanner_.peek()->start();
    this->expect(TokenType::kw_record);
    auto rec_type = std::make_unique<RecordTypeNode>(start, field_list());
    while (this->if_next(TokenType::semicolon))
    {
        scanner_.next();
        rec_type->add_field_list(field_list());
    }

    this->expect(TokenType::kw_end);
    return rec_type; // Success
}

// FPSection -> ("VAR")? IdentList ":" type
std::unique_ptr<fp_section> Parser::fp_section()
{
    logger_.info("FPSection");
    auto start = scanner_.peek()->start();
    bool var_included = false;

    if (this->if_next(TokenType::kw_var))
    {
        scanner_.next();
        var_included = true;
    }

    auto id_list = ident_list();
    this->expect(TokenType::colon);
    auto var_type = type();
    return std::make_unique<std::tuple<bool, unique_ptr<std::vector<unique_ptr<IdentNode>>>, unique_ptr<TypeNode>>>(var_included, std::move(id_list), std::move(var_type));
}

// FormalParameters -> "(" (FPSection (";" FPSection)*  )? ")"
std::unique_ptr<formal_parameters> Parser::formal_parameters()
{
    logger_.info("Formal Parameters");
    auto start = scanner_.peek()->start();


    auto formal_params = std::make_unique<std::vector<std::unique_ptr<std::tuple<bool,std::unique_ptr<std::vector<std::unique_ptr<IdentNode>>>,std::unique_ptr<TypeNode>>>>>();
    this->expect(TokenType::lparen);

    // Empty Parameters
    if (this->if_next(TokenType::rparen))
    {
        scanner_.next();
        return formal_params;
    }

    // FPSections
    formal_params->emplace_back(fp_section());

    while (this->if_next(TokenType::semicolon))
    {
        scanner_.next();
        formal_params->emplace_back(fp_section());
    }

    this->expect(TokenType::rparen);
    return formal_params;
}

// ProcedureHeadingNode -> "PROCEDURE" Ident (FormalParameters)?
std::unique_ptr<ProcedureHeadingNode> Parser::procedure_heading()
{
    logger_.info("Procedure Heading");
    auto start = scanner_.peek()->start();
    this->expect(TokenType::kw_procedure);
    auto id = ident();
    // To see whether FormalParameters follow, we check if an "(" follows
    if (this->if_next(TokenType::lparen))
    {
        return std::make_unique<ProcedureHeadingNode>(start, std::move(id), formal_parameters());
    }

    return std::make_unique<ProcedureHeadingNode>(start, std::move(id));
}

// ProcedureBodyNode -> declarations ("BEGIN" StatementSequence)? "END" ident
std::unique_ptr<ProcedureBodyNode> Parser::procedure_body()
{
    logger_.info("Procedure Body");
    auto start = scanner_.peek()->start();
    auto declars = declarations();

    // Statement Sequence
    std::unique_ptr<StatementSequenceNode> statements = nullptr;
    if (this->if_next(TokenType::kw_begin))
    {
        scanner_.next();
        statements = statement_sequence();
    }
    this->expect(TokenType::kw_end);
    return std::make_unique<ProcedureBodyNode>(start, std::move(declars), ident(), std::move(statements));
}

// ProcedureDeclaration = ProcedureHeadingNode ";" ProcedureBodyNode
std::unique_ptr<ProcedureDeclarationNode> Parser::procedure_declaration()
{
    logger_.info("Procedure Declaration");
    auto start = scanner_.peek()->start();
    auto heading = procedure_heading();
    this->expect(TokenType::semicolon);
    auto body = procedure_body();
    return std::make_unique<ProcedureDeclarationNode>(start, std::move(heading), std::move(body));
}

//  Declarations ->    ("CONST" (ident "=" expression ";")* )?
//                     ("TYPE"  (ident "=" type ";") *      )?
//                     ("VAR"   (identList ":" type ";")*   )?
//                     (ProcedureDeclaration ";"            )*
std::unique_ptr<DeclarationsNode> Parser::declarations()
{
    logger_.info("Declarations");
    auto start = scanner_.peek()->start();

    auto declarations_node = std::make_unique<DeclarationsNode>(start);

    // CONST declarations
    if (this->if_next(TokenType::kw_const))
    {
        scanner_.next();

        // (ident "=" expression ";")*
        while (this->if_next(TokenType::const_ident))
        {
            auto id = ident();
            this->expect(TokenType::op_eq);
            auto expr = expression();
            this->expect(TokenType::semicolon);
            declarations_node->add_constant(std::move(id), std::move(expr));
        }
    }

    // TYPE declarations
    if (this->if_next(TokenType::kw_type))
    {
        scanner_.next();

        // (ident "=" type ";")*
        while (this->if_next(TokenType::const_ident))
        {
            auto id = ident();
            this->expect(TokenType::op_eq);
            auto id_type = type();
            this->expect(TokenType::semicolon);
            declarations_node->add_type(std::move(id), std::move(id_type));
        }
    }

    // VAR declarations
    if (this->if_next(TokenType::kw_var))
    {
        scanner_.next();

        // (identList ":" type ";")*
        while (this->if_next(TokenType::const_ident))
        {
            auto id_list = ident_list();
            this->expect(TokenType::colon);
            auto id_type = type();
            this->expect(TokenType::semicolon);
            declarations_node->add_var(std::move(id_list), std::move(id_type));
        }
    }

    // Procedure Declarations
    while (this->if_next(TokenType::kw_procedure))
    {
        auto procedure = procedure_declaration();
        this->expect(TokenType::semicolon);
        declarations_node->add_procedure(std::move(procedure));
    }

    return declarations_node;
}

// Module -> "Module" ident ";" declarations ("BEGIN" StatementSequence)? "END" ident "."
std::unique_ptr<ModuleNode> Parser::module()
{
    logger_.info("Module");
    auto start = scanner_.peek()->start();

    this->expect(TokenType::kw_module);
    auto module_name_begin = ident();
    this->expect(TokenType::semicolon);

    auto declars = declarations();

    // Statement Sequence
    std::unique_ptr<StatementSequenceNode> statements = nullptr;
    if (this->if_next(TokenType::kw_begin))
    {
        scanner_.next();
        statements = statement_sequence();
    }

    // "END" ident "."
    this->expect(TokenType::kw_end);
    auto module_name_end = ident();
    this->expect(TokenType::period);
    if (!this->if_next(TokenType::eof))
    {
        logger_.error("", "Not all tokens consumed");
        this->has_error = true;
    }

    return std::make_unique<ModuleNode>(start, std::move(module_name_begin), std::move(declars), std::move(statements), std::move(module_name_end));
}

std::unique_ptr<ModuleNode> Parser::parse()
{
    return module();
}
