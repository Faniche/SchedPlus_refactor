//
// Created by faniche on 22-10-17.
//

#ifndef SCHEDPLUS_REFACTOR_ENDSYSTEM_H
#define SCHEDPLUS_REFACTOR_ENDSYSTEM_H


#include "Node.h"
#include "Port.h"

class EndSystem: public Node{
private:
    std::shared_ptr<Port> port;

    const std::string & vToString() override;
public:
    EndSystem(std::string &&_name, uint64_t _dpr);

    [[nodiscard]] std::shared_ptr<Port> getPort() const;
};


#endif //SCHEDPLUS_REFACTOR_ENDSYSTEM_H
