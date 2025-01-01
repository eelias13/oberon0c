/*
 * Main class of the Oberon-0 compiler.
 *
 * Created by Michael Grossniklaus on 12/14/17.
 */

#include <iostream>
#include <string>
#include "scanner/Scanner.h"
#include "parser/Parser.h"
#include "semantic_checker/SemanticChecker.h"

using std::cerr;
using std::cout;
using std::endl;
using std::string;

int main(const int argc, const char *argv[]) {
    if (argc < 2 || argc > 3) {
        cerr << "Usage: oberon0c <filename> (no-debug)" << endl;
        exit(1);
    }
    string filename = argv[1];
    Logger logger;

    if(argc > 2){
        if(string(argv[2]) == "no-debug"){
            logger.setLevel(LogLevel::INFO);
        }
        else{
            cerr << "Invalid argument: " << argv[2] << std::endl << "Usage: oberon0c <filename> ('no-debug')" << std::endl;
            exit(1);
        }
    }
    else{
        logger.setLevel(LogLevel::DEBUG);
    }

    // Scanning
    Scanner scanner(filename, logger);

    // Parsing
    Parser parser(scanner,logger);
    auto ast = parser.parse();
    if(ast && logger.getErrorCount() == 0){
        std::cout << std::endl << "Compiled Program:" << std::endl << *ast << std::endl;
        std::cout << "Parsing successful." << std::endl;

        // Semantic Checking
        SemanticChecker semantics(logger);
        semantics.validate_program(*ast);

        if(logger.getErrorCount() > 0){
            std::cout << "Errors occurred during semantic checking." << std::endl;
        }
        else{

            // Code Generation

        }

    }else{
        std::cout << "Errors occurred during parsing." << std::endl;
    }

    string status = (logger.getErrorCount() == 0 ? "complete" : "failed");
    logger.info("Compilation " + status + ": " +
                to_string(logger.getErrorCount()) + " error(s), " +
                to_string(logger.getWarningCount()) + " warning(s), " +
                to_string(logger.getInfoCount()) + " message(s).");
    exit(logger.getErrorCount() != 0);
}