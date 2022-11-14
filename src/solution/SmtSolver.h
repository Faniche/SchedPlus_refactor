//
// Created by faniche on 22-10-21.
//

#ifndef SCHEDPLUS_REFACTOR_SMTSOLVER_H
#define SCHEDPLUS_REFACTOR_SMTSOLVER_H

#include <z3++.h>
#include "../topology/Input.h"
#include "../type.h"
#include "Solver.h"

using namespace z3;

class SmtSolver: public Solver{
private:
    std::shared_ptr<Input> input;

    void vSolve() override;

public:
    explicit SmtSolver(std::shared_ptr<Input> input);

};


#endif //SCHEDPLUS_REFACTOR_SMTSOLVER_H
