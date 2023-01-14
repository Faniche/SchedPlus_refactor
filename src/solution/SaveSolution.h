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

    bool isPortInUse(const std::shared_ptr<Port>& port);
public:

    SaveSolution(const std::shared_ptr<Input> &_input,
                 TtStreams *_p,
                 MyMiddleCost *_c);

    void saveRoute(const std::string &routeFileLocation);

    void saveGCL();

    void saveGCL(const std::string &gclFileLocation);

    void saveSwPortSchedule(const std::string &schedFileLocation);

    void saveEsSchedule(const std::string &schedFileLocation);

    void saveIni(const std::string &route_file,
                 const std::string &gcl_file,
                 const std::string &ini_file,
                 const std::string &ned_file,
                 size_t solution_id);
};


#endif //SCHEDPLUS_REFACTOR_SAVESOLUTION_H
