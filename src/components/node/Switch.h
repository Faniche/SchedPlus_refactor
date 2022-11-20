//
// Created by faniche on 22-10-17.
//

#ifndef SCHEDPLUS_REFACTOR_SWITCH_H
#define SCHEDPLUS_REFACTOR_SWITCH_H


#include <memory>
#include "Port.h"
#include "Node.h"

class Switch: public Node{
private:
    std::vector<std::shared_ptr<Port>> ports;

    const std::string & vToString() override;
public:
    Switch(std::string &&_name, uint64_t _dpr, size_t _portNum = 8);

    [[nodiscard]] const std::vector<std::shared_ptr<Port>> &getPorts() const;
};


#endif //SCHEDPLUS_REFACTOR_SWITCH_H
