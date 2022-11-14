//
// Created by faniche on 22-11-14.
//

#ifndef SCHEDPLUS_REFACTOR_GASOLVER_H
#define SCHEDPLUS_REFACTOR_GASOLVER_H


#include "Solver.h"

class GASolver: public Solver {
private:
    std::shared_ptr<Input> input;

    void vSolve() override;
public:
    explicit GASolver(std::shared_ptr<Input> input);
};


#endif //SCHEDPLUS_REFACTOR_GASOLVER_H
