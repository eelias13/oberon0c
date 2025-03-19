#include <emscripten.h>
#include <string>
#include <iostream>
#include <sstream>
#include "scanner/Scanner.h"
#include "global.h"
#include "parser/Parser.h"
#include "semantic_checker/SemanticChecker.h"
#include "parser/ast/JsonVisitor.h"

using std::ostringstream;
using std::string;
using std::unique_ptr;

extern "C"
{

    EMSCRIPTEN_KEEPALIVE
    char *check(const char *source_code)
    {

        ostringstream json_logs;
        ostringstream out;
        ostringstream err;
        Logger logger(json_logs, out, err);

        Scanner scanner(string(source_code), logger);
        Parser parser(scanner, logger);
        auto ast = parser.parse();

        SemanticChecker semantics(logger);
        semantics.validate_program(*ast);

        JsonVisitor jv;
        jv.visit(*ast.get());

        ostringstream result_st;
        result_st << "{ \"logs\" : [" << json_logs.str() << "]";
        result_st << ",\"ast\" : {" << jv.getJson() << "}}";

        // Allocate memory for the output string
        string result = result_st.str();
        char *output = (char *)malloc(result.size() + 1);
        strcpy(output, result.c_str());

        return output;
    }

    EMSCRIPTEN_KEEPALIVE
    char *parse(const char *source_code)
    {
        ostringstream json_logs;
        ostringstream out;
        ostringstream err;
        Logger logger(json_logs, out, err);

        Scanner scanner(string(source_code), logger);
        Parser parser(scanner, logger);
        auto ast = parser.parse();

        JsonVisitor jv;
        jv.visit(*ast.get());

        ostringstream result_st;
        result_st << "{ \"logs\" : [" << json_logs.str() << "]";
        result_st << ",\"ast\" : {" << jv.getJson() << "}}";

        // Allocate memory for the output string
        string result = result_st.str();
        char *output = (char *)malloc(result.size() + 1);
        strcpy(output, result.c_str());

        return output;
    }

    EMSCRIPTEN_KEEPALIVE
    char *scanner(const char *source_code)
    {

        ostringstream json_logs;
        ostringstream out;
        ostringstream err;
        Logger logger(json_logs, out, err);
        Scanner scanner(string(source_code), logger);
        ostringstream json_output;
        json_output << "\"tokens\" :[";

        bool first = true;
        while (true)
        {
            unique_ptr<const Token> token = scanner.next();

            if (!first)
            {
                json_output << ",";
            }
            first = false;

            json_output << token->json();
            if (token->type() == TokenType::eof)
                break;
        }
        json_output << "] ";

        ostringstream result_st;
        result_st << "{ ";
        result_st << json_output.str();
        result_st << ", \"logs\" : [";
        result_st << json_logs.str();
        result_st << "] }";

        // Allocate memory for the output string
        string result = result_st.str();
        char *output = (char *)malloc(result.size() + 1);
        strcpy(output, result.c_str());

        return output; // Return JSON string of tokens
    }
}
