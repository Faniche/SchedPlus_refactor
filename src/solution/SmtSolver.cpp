//
// Created by faniche on 22-10-21.
//

#include "SmtSolver.h"

#include <utility>

SmtSolver::SmtSolver(std::shared_ptr<Input> input) : input(std::move(input)) {}

void SmtSolver::vSolve(const std::string &path, int32_t runId) {
    context c;


}
