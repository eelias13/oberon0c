/*
 * Tokens returned by the scanner of the Oberon-0 compiler.
 *
 * Created by Michael Grossniklaus on 2/23/18.
 */

#include "Token.h"

Token::~Token() = default;

TokenType Token::type() const
{
    return type_;
}

FilePos Token::start() const
{
    return start_;
}

FilePos Token::end() const
{
    return end_;
}

void Token::print(std::ostream &stream) const
{
    stream << type_;
}

std::ostream &operator<<(std::ostream &stream, const Token &token)
{
    token.print(stream);
    return stream;
}

std::ostream &operator<<(std::ostream &stream, const TokenType &type)
{
    std::string result;
    switch (type)
    {
    case TokenType::eof:
        result = "<eof>";
        break;
    case TokenType::undef:
        result = "<undefined>";
        break;
    case TokenType::boolean_literal:
        result = "BOOLEAN literal";
        break;
    case TokenType::byte_literal:
        result = "BYTE literal";
        break;
    case TokenType::char_literal:
        result = "CHAR literal";
        break;
    case TokenType::short_literal:
        result = "SHORTINT literal";
        break;
    case TokenType::int_literal:
        result = "INTEGER literal";
        break;
    case TokenType::long_literal:
        result = "LONGINT literal";
        break;
    case TokenType::float_literal:
        result = "REAL literal";
        break;
    case TokenType::double_literal:
        result = "LONGREAL literal";
        break;
    case TokenType::string_literal:
        result = "STRING literal";
        break;
    case TokenType::const_ident:
        result = "identifier";
        break;
    case TokenType::period:
        result = ".";
        break;
    case TokenType::comma:
        result = ",";
        break;
    case TokenType::colon:
        result = ":";
        break;
    case TokenType::semicolon:
        result = ";";
        break;
    case TokenType::lparen:
        result = "(";
        break;
    case TokenType::rparen:
        result = ")";
        break;
    case TokenType::lbrack:
        result = "[";
        break;
    case TokenType::rbrack:
        result = "]";
        break;
    case TokenType::lbrace:
        result = "{";
        break;
    case TokenType::rbrace:
        result = "}";
        break;
    case TokenType::varargs:
        result = "...";
        break;
    case TokenType::pipe:
        result = "|";
        break;
    case TokenType::op_times:
        result = "*";
        break;
    case TokenType::op_div:
        result = "DIV";
        break;
    case TokenType::op_mod:
        result = "MOD";
        break;
    case TokenType::op_plus:
        result = "+";
        break;
    case TokenType::op_minus:
        result = "-";
        break;
    case TokenType::op_and:
        result = "&";
        break;
    case TokenType::op_or:
        result = "OR";
        break;
    case TokenType::op_not:
        result = "~";
        break;
    case TokenType::op_eq:
        result = "=";
        break;
    case TokenType::op_neq:
        result = "#";
        break;
    case TokenType::op_lt:
        result = "<";
        break;
    case TokenType::op_gt:
        result = ">";
        break;
    case TokenType::op_leq:
        result = "<=";
        break;
    case TokenType::op_geq:
        result = ">=";
        break;
    case TokenType::op_becomes:
        result = ":=";
        break;
    case TokenType::kw_module:
        result = "MODULE";
        break;
    case TokenType::kw_import:
        result = "IMPORT";
        break;
    case TokenType::kw_procedure:
        result = "PROCEDURE";
        break;
    case TokenType::kw_begin:
        result = "BEGIN";
        break;
    case TokenType::kw_end:
        result = "END";
        break;
    case TokenType::kw_if:
        result = "IF";
        break;
    case TokenType::kw_then:
        result = "THEN";
        break;
    case TokenType::kw_else:
        result = "ELSE";
        break;
    case TokenType::kw_elsif:
        result = "ELSIF";
        break;
    case TokenType::kw_loop:
        result = "LOOP";
        break;
    case TokenType::kw_exit:
        result = "EXIT";
        break;
    case TokenType::kw_while:
        result = "WHILE";
        break;
    case TokenType::kw_do:
        result = "DO";
        break;
    case TokenType::kw_repeat:
        result = "REPEAT";
        break;
    case TokenType::kw_until:
        result = "UNTIL";
        break;
    case TokenType::kw_for:
        result = "FOR";
        break;
    case TokenType::kw_to:
        result = "TO";
        break;
    case TokenType::kw_by:
        result = "BY";
        break;
    case TokenType::kw_array:
        result = "ARRAY";
        break;
    case TokenType::kw_record:
        result = "RECORD";
        break;
    case TokenType::kw_const:
        result = "CONST";
        break;
    case TokenType::kw_type:
        result = "TYPE";
        break;
    case TokenType::kw_var:
        result = "VAR";
        break;
    case TokenType::kw_of:
        result = "OF";
        break;
    case TokenType::kw_extern:
        result = "EXTERN";
        break;
    case TokenType::kw_return:
        result = "RETURN";
        break;
    case TokenType::kw_nil:
        result = "NIL";
        break;
    default:
        result = "undefined token";
        break;
    }
    stream << result;
    return stream;
}

#include <sstream> // Needed for JSON string construction
#include "Token.h"

// Convert TokenType enum to a string for JSON output
std::string tokenTypeToString(TokenType type)
{
    switch (type)
    {
    case TokenType::eof:
        return "eof";
    case TokenType::undef:
        return "undef";
    case TokenType::boolean_literal:
        return "boolean_literal";
    case TokenType::byte_literal:
        return "byte_literal";
    case TokenType::char_literal:
        return "char_literal";
    case TokenType::short_literal:
        return "short_literal";
    case TokenType::int_literal:
        return "int_literal";
    case TokenType::long_literal:
        return "long_literal";
    case TokenType::float_literal:
        return "float_literal";
    case TokenType::double_literal:
        return "double_literal";
    case TokenType::string_literal:
        return "string_literal";
    case TokenType::const_ident:
        return "const_ident";
    case TokenType::period:
        return "period";
    case TokenType::comma:
        return "comma";
    case TokenType::colon:
        return "colon";
    case TokenType::semicolon:
        return "semicolon";
    case TokenType::lparen:
        return "lparen";
    case TokenType::rparen:
        return "rparen";
    case TokenType::lbrack:
        return "lbrack";
    case TokenType::rbrack:
        return "rbrack";
    case TokenType::lbrace:
        return "lbrace";
    case TokenType::rbrace:
        return "rbrace";
    case TokenType::varargs:
        return "varargs";
    case TokenType::pipe:
        return "pipe";
    case TokenType::op_times:
        return "op_times";
    case TokenType::op_div:
        return "op_div";
    case TokenType::op_mod:
        return "op_mod";
    case TokenType::op_plus:
        return "op_plus";
    case TokenType::op_minus:
        return "op_minus";
    case TokenType::op_and:
        return "op_and";
    case TokenType::op_or:
        return "op_or";
    case TokenType::op_not:
        return "op_not";
    case TokenType::op_eq:
        return "op_eq";
    case TokenType::op_neq:
        return "op_neq";
    case TokenType::op_lt:
        return "op_lt";
    case TokenType::op_gt:
        return "op_gt";
    case TokenType::op_leq:
        return "op_leq";
    case TokenType::op_geq:
        return "op_geq";
    case TokenType::op_becomes:
        return "op_becomes";
    case TokenType::kw_module:
        return "kw_module";
    case TokenType::kw_import:
        return "kw_import";
    case TokenType::kw_procedure:
        return "kw_procedure";
    case TokenType::kw_extern:
        return "kw_extern";
    case TokenType::kw_return:
        return "kw_return";
    case TokenType::kw_begin:
        return "kw_begin";
    case TokenType::kw_end:
        return "kw_end";
    case TokenType::kw_if:
        return "kw_if";
    case TokenType::kw_then:
        return "kw_then";
    case TokenType::kw_else:
        return "kw_else";
    case TokenType::kw_elsif:
        return "kw_elsif";
    case TokenType::kw_loop:
        return "kw_loop";
    case TokenType::kw_exit:
        return "kw_exit";
    case TokenType::kw_while:
        return "kw_while";
    case TokenType::kw_do:
        return "kw_do";
    case TokenType::kw_repeat:
        return "kw_repeat";
    case TokenType::kw_until:
        return "kw_until";
    case TokenType::kw_for:
        return "kw_for";
    case TokenType::kw_to:
        return "kw_to";
    case TokenType::kw_by:
        return "kw_by";
    case TokenType::kw_case:
        return "kw_case";
    case TokenType::kw_array:
        return "kw_array";
    case TokenType::kw_record:
        return "kw_record";
    case TokenType::kw_const:
        return "kw_const";
    case TokenType::kw_type:
        return "kw_type";
    case TokenType::kw_var:
        return "kw_var";
    case TokenType::kw_of:
        return "kw_of";
    case TokenType::kw_pointer:
        return "kw_pointer";
    case TokenType::kw_nil:
        return "kw_nil";
    default:
        return "undefined";
    }
}

// Convert Token to JSON format
std::string Token::json() const
{
    std::ostringstream json;
    json << "{"
         << "\"type\":\"" << tokenTypeToString(type_) << "\","
         << "\"start\":{\"line\":" << start_.lineNo << ",\"char\":" << start_.charNo << "},"
         << "\"end\":{\"line\":" << end_.lineNo << ",\"char\":" << end_.charNo << "}"
         << "}";
    return json.str();
}
