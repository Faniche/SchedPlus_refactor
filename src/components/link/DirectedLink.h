//
// Created by faniche on 22-10-18.
//

#ifndef SCHEDPLUS_REFACTOR_DIRECTEDLINK_H
#define SCHEDPLUS_REFACTOR_DIRECTEDLINK_H

#include "../node/Node.h"
#include "../node/Port.h"

extern int g_link_id;

class DirectedLink {
private:
    link_id id;
    std::shared_ptr<Node> srcNode;
    std::shared_ptr<Node> destNode;
    std::shared_ptr<Port> srcPort;
    std::shared_ptr<Port> destPort;
    uint64_t speed = 1000000000;
    /* the length of link, meter */
    uint32_t len = 20;
    /* the signal propagation speed of the media, val: nanosecond per meter */
    uint32_t propSpeed = 5;
    uint32_t macrotick;
public:
    DirectedLink();

    DirectedLink(std::shared_ptr<Node> _srcNode,
                 std::shared_ptr<Node> _destNode,
                 std::shared_ptr<Port> _srcPort,
                 std::shared_ptr<Port> _destPort,
                 uint32_t _macrotick);

    DirectedLink(std::shared_ptr<Node> _srcNode,
                 std::shared_ptr<Node> _destNode,
                 std::shared_ptr<Port> _srcPort,
                 std::shared_ptr<Port> _destPort,
                 uint32_t _len,
                 uint32_t _propSpeed,
                 uint32_t _macrotick);

    [[nodiscard]] link_id getId() const;

    void setId(link_id _id);

    [[nodiscard]] std::shared_ptr<Node> getSrcNode() const;

    [[nodiscard]] std::shared_ptr<Node> getDestNode() const;

    [[nodiscard]] std::shared_ptr<Port> getSrcPort() const;

    [[nodiscard]] std::shared_ptr<Port> getDestPort() const;

    void addGateControlEntry(std::shared_ptr<GateControlEntry> &&gateControlEntry);

    void clearGateControlEntry();

    void sortGCL();

    void mergeGCL();

    [[nodiscard]] uint64_t getSpeed() const;

    [[nodiscard]] uint32_t getLen() const;

    void setLen(uint32_t len);

    [[nodiscard]] uint32_t getPropSpeed() const;

    void setPropSpeed(uint32_t _propSpeed);

    [[nodiscard]] uint32_t getMacrotick() const;

    void setMacrotick(uint32_t macrotick);
};


#endif //SCHEDPLUS_REFACTOR_DIRECTEDLINK_H
