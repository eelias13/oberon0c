//
// Created by M on 18.11.2024.
//

#include "RepeatStatement.h"

void RepeatStatement::print(ostream &stream) const {
    stream << "REPEAT " << statements_ << " UNTIL " << condition_;
}
