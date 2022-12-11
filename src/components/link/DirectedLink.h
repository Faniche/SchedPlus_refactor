//
// Created by faniche on 22-10-18.
//

#ifndef SCHEDPLUS_REFACTOR_DIRECTEDLINK_H
#define SCHEDPLUS_REFACTOR_DIRECTEDLINK_H

#include "../node/Node.h"
#include "../node/Port.h"

class DirectedLink {
private:
    link_id_t id;
    std::shared_ptr<Node> srcNode;
    std::shared_ptr<Node> destNode;
    std::shared_ptr<Port> srcPort;
    std::shared_ptr<Port> destPort;
    /* the length of link, meter */
    uint32_t len = 20;
    /* the signal propagation speed of the media, val: nanosecond per meter */
    uint32_t propSpeed = 5;

public:
    DirectedLink();

    DirectedLink(std::shared_ptr<Node> _srcNode,
                 std::shared_ptr<Node> _destNode,
                 std::shared_ptr<Port> _srcPort,
                 std::shared_ptr<Port> _destPort);

    DirectedLink(std::shared_ptr<Node> _srcNode,
                 std::shared_ptr<Node> _destNode,
                 std::shared_ptr<Port> _srcPort,
                 std::shared_ptr<Port> _destPort,
                 uint32_t _len);

    [[nodiscard]] link_id_t getId() const;

    void setId(link_id_t _id);

    [[nodiscard]] std::shared_ptr<Node> getSrcNode() const;

    [[nodiscard]] std::shared_ptr<Node> getDestNode() const;

    [[nodiscard]] std::shared_ptr<Port> getSrcPort() const;

    [[nodiscard]] std::shared_ptr<Port> getDestPort() const;

    void addGateControlEntry(std::shared_ptr<GateControlEntry> &&gateControlEntry);

    void clearGateControlEntry();

    void sortGCL();

    void mergeGCL();

    [[nodiscard]] uint32_t getLen() const;

    void setLen(uint32_t len);

    [[nodiscard]] uint32_t getPropSpeed() const;

    void setPropSpeed(uint32_t _propSpeed);

};


#endif //SCHEDPLUS_REFACTOR_DIRECTEDLINK_H
