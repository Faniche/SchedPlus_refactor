//
// Created by faniche on 23-1-4.
//

#ifndef SCHEDPLUS_REFACTOR_SAVESOLUTION_H
#define SCHEDPLUS_REFACTOR_SAVESOLUTION_H


#include <string>
#include "GeneticAlgorithm/GaSolution.h"
#include "GeneticAlgorithm/GaMiddleCost.h"
#include "../topology/Input.h"
#include "../../lib/pugixml/pugixml.hpp"

class SaveSolution {
    std::shared_ptr<Input> input;

    TtStreams *p;

    MyMiddleCost *c;

    std::string topology;

public:

    SaveSolution(const std::shared_ptr<Input> &_input,
                 TtStreams *_p,
                 MyMiddleCost *_c);

    void saveRoute(const std::string &routeFileLocation);

    void saveGCL();

    void saveGCL(const std::string &gclFileLocation);

    void saveSwPortSchedule(const std::string &schedFileLocation);

    void saveEsSchedule(const std::string &schedFileLocation);

    void saveIni(const std::string &routeFileName,
                 const std::string &gclFileName,
                 const std::string &iniFile,
                 const std::string &topology,
                 size_t solution_id);

    void saveScheduleAndMiddleCost(const std::string &middleCostPath, size_t solutionId);
};


#endif //SCHEDPLUS_REFACTOR_SAVESOLUTION_H
