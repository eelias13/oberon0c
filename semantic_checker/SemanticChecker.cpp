//
// Created by M on 15.12.2024.
//

#include "SemanticChecker.h"

SemanticChecker::SemanticChecker(Logger &logger) : logger_(logger) {
    scope_table_ = ScopeTable();
}