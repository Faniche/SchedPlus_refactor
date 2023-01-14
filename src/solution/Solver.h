//
// Created by faniche on 22-10-23.
//

#ifndef SCHEDPLUS_REFACTOR_SOLVER_H
#define SCHEDPLUS_REFACTOR_SOLVER_H


#include "../topology/Input.h"

class Solver {
private:
//    std::unique_ptr<Input> input;
    virtual void vSolve(const std::string &path, int32_t runId) = 0;
public:
    void solve(const std::string &path, int32_t runId);
};


#endif //SCHEDPLUS_REFACTOR_SOLVER_H
