#include "JsonVisitor.h"

void JsonVisitor::visit(ExpressionNode &node)
{

    // int precedence_ = ;
    // std::optional<long> value_ = std::nullopt;
    // TypeInfo formal_type;
    // TypeInfo actual_type;
    // TypeNode *type_node = nullptr;

    // json_ << "\"ExpressionNode\" : { ";
    // json_ << "\"actualType\" : " << node.get_actual_type() ? node.get_actual_type() : "null";
    switch (node.getNodeType())
    {
    case NodeType::unary_expression:
        visit(dynamic_cast<UnaryExpressionNode &>(node));
        break;
    case NodeType::binary_expression:
        visit(dynamic_cast<BinaryExpressionNode &>(node));
        break;
    case NodeType::ident_selector_expression:
        visit(dynamic_cast<IdentSelectorExpressionNode &>(node));
        break;
    case NodeType::integer:
        visit(dynamic_cast<IntNode &>(node));
        break;
    default:
        return;
    }

    // json_ << "}"
}

std::string sourceOperatorStr(SourceOperator op)
{
    switch (op)
    {
    case SourceOperator::PLUS:
        return "PLUS";

    case SourceOperator::MINUS:
        return "MINUS";

    case SourceOperator::OR:
        return "OR";

    case SourceOperator::MULT:
        return "MULT";

    case SourceOperator::DIV:
        return "DIV";

    case SourceOperator::MOD:
        return "MOD";

    case SourceOperator::AND:
        return "AND";

    case SourceOperator::NEG:
        return "NEG";

    case SourceOperator::NOT:
        return "NOT";

    case SourceOperator::EQ:
        return "EQ";

    case SourceOperator::NEQ:
        return "NEQ";

    case SourceOperator::LT:
        return "LT";

    case SourceOperator::LEQ:
        return "LEQ";

    case SourceOperator::GT:
        return "GT";

    case SourceOperator::GEQ:
        return "GEQ";

    case SourceOperator::NO_OPERATOR:
        return "NO_OPERATOR";

    case SourceOperator::PAREN:
        return "PAREN";
    default:
        return "";
    }
}

void JsonVisitor::visit(BinaryExpressionNode &node)
{
    json_ << "\"BinaryExpressionNode\" : { \"op\" : \"" << sourceOperatorStr(node.get_op()) << "\"";
    json_ << ", \"lhs\" : {";
    visit(*node.get_lhs());
    json_ << "}, \"rhs\" : {";
    visit(*node.get_rhs());
    json_ << "}}";
}
void JsonVisitor::visit(UnaryExpressionNode &node)
{
    json_ << "\"UnaryExpressionNode\" : { \"op\" : \"" << sourceOperatorStr(node.get_op()) << "\"";
    json_ << ", \"expr\" : ";
    visit(*node.get_expr());
    json_ << "}";
}

void JsonVisitor::visit(IdentSelectorExpressionNode &node)
{
    json_ << "\"IdentSelectorExpressionNode\":{ \"ident\" : ";
    visit(*node.get_identifier());
    json_ << ", \"selector\" : ";
    visit(*node.get_selector());
    json_ << "}";
}

void JsonVisitor::visit(IdentNode &node)
{
    json_ << "\"IdentNode\":{ \"ident\" : \"" << node.get_value() << "\" }";
}

void JsonVisitor::visit(IntNode &node)
{
    json_ << "\"IntNode\" : { \"value\" : " << node.get_value() << "}";
}
void JsonVisitor::visit(SelectorNode &node)
{

    json_ << "\"SelectorNode\" : [ ";

    bool is_first = true;
    for (const auto selector : *node.get_selector())
    {

        if (!is_first)
        {
            json_ << ", ";
        }
        json_ << "{";
        if (std::get<0>(selector))
        {
            ExpressionNode *expr = std::get<2>(selector);
            visit(*expr);
        }
        else
        {
            IdentNode *ident = std::get<1>(selector);
            visit(*ident);
        }
        json_ << "}";
    }
}
void JsonVisitor::visit(TypeNode &node)
{
    switch (node.getNodeType())
    {
    case NodeType::ident:
        visit(dynamic_cast<IdentNode &>(node));
        break;
    case NodeType::array_type:
        visit(dynamic_cast<ArrayTypeNode &>(node));
        break;
    case NodeType::record_type:
        visit(dynamic_cast<RecordTypeNode &>(node));
        break;
    default:
        break;
    }
}
void JsonVisitor::visit(ArrayTypeNode &node)
{
    // json_ << "\"ArrayTypeNode\":{\"dim\":{";
    // visit(*node.get_dim_node());
    // json_ << "}, \"type\":{";
    // visit(*node.get_type_node());
    // json_ << "}}";
}
void JsonVisitor::visit(RecordTypeNode &node)
{
}

void JsonVisitor::visit(DeclarationsNode &node)
{

    json_ << "\"DeclarationsNode\":{";
    bool is_first = true;

    json_ << "\"constants\": [";
    for (auto constant : node.get_constants())
    {
        if (!is_first)
        {
            json_ << ",";
        }
        is_first = false;

        json_ << "{ \"name\":{";
        visit(*constant.first);
        json_ << "}, \"expr\":{";
        visit(*constant.second);
        json_ << "}}";
    }
    json_ << "]";

    is_first = true;
    json_ << ",\"variables\": [";
    for (auto variables : node.get_variables())
    {
        if (!is_first)
        {
            json_ << ",";
        }
        is_first = false;
        json_ << "{ \"names\":[";

        bool is_first_1 = true;
        for (auto name : variables.first)
        {
            if (!is_first_1)
            {
                json_ << ",";
            }
            is_first_1 = false;
            json_ << "{";
            visit(*name);
            json_ << "}";
        }

        json_ << "], \"type\":{";
        visit(*variables.second);
        json_ << "}}";
    }
    json_ << "]";

    is_first = true;
    json_ << ",\"typenames\": [";
    for (auto typenames : node.get_typenames())
    {
        if (!is_first)
        {
            json_ << ",";
        }
        json_ << "{ \"name\":{";
        visit(*typenames.first);
        json_ << "}, \"type\":{";
        visit(*typenames.second);
        json_ << "}}";
    }
    json_ << "]";

    is_first = true;
    json_ << ",\"procedures\": [";
    for (auto procedures : node.get_procedures())
    {
        if (!is_first)
        {
            json_ << ",";
        }
        is_first = false;

        json_ << "{";
        visit(*procedures);
        json_ << "}";
    }
    json_ << "]";

    json_ << "}";
}
void JsonVisitor::visit(ProcedureDeclarationNode &node) {}
void JsonVisitor::visit(StatementNode &node) {}
void JsonVisitor::visit(AssignmentNode &node) {}
void JsonVisitor::visit(IfStatementNode &node) {}
void JsonVisitor::visit(ProcedureCallNode &node) {}
void JsonVisitor::visit(RepeatStatementNode &node) {}
void JsonVisitor::visit(StatementSequenceNode &node) {}
void JsonVisitor::visit(WhileStatementNode &node) {}

void JsonVisitor::visit(ModuleNode &node)
{

    json_ << "\"ModuleNode\" : { \"name\" : { \"begin\" : {";

    visit(*node.get_name().first);
    json_ << "}, \"end\":{";
    visit(*node.get_name().second);
    json_ << "}}, \"declaration\": {";
    visit(*node.get_declarations());
    json_ << "}, \"statements\" : {";
    visit(*node.get_statements());
    json_ << "}}";
}

std::string JsonVisitor::getJson()
{
    return json_.str();
}
