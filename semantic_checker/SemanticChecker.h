//
// Created by M on 15.12.2024.
//

#ifndef OBERON0C_SEMANTICCHECKER_H
#define OBERON0C_SEMANTICCHECKER_H

#include "ScopeTable.h"
#include "util/Logger.h"

class SemanticChecker {

    private:
        ScopeTable scope_table_;
        Logger& logger_;

    public:
        explicit SemanticChecker(Logger& logger);
};


#endif //OBERON0C_SEMANTICCHECKER_H
