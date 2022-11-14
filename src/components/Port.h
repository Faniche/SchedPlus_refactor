//
// Created by faniche on 22-10-17.
//

#ifndef SCHEDPLUS_REFACTOR_PORT_H
#define SCHEDPLUS_REFACTOR_PORT_H

#include <vector>
#include <cstdint>
#include <string>
#include "../type.h"
#include "GateControlEntry.h"

extern int g_port_counter;


class Port {
private:
    port_id id;

    /* speed of port, default: 1Gbps */
    uint64_t speed = 1000000000;

    /* sending 1 byte shall spend 8 ns with Gigabit full duplex ethernet */
    double transmitSpd = 8;

    std::vector<std::shared_ptr<GateControlEntry>> gateControlList;

    /* the bytes of cached frame in queue with priority 5 and 6 */
    uint64_t qsize = 0;

    /* the num of cached frame in queue with priority 5 and 6 */
    uint16_t qlen = 0;

    std::vector<uint64_t> frameQueue;

public:
    explicit Port(uint64_t _speed = 1000000000);

    [[nodiscard]] port_id getId() const;

    [[nodiscard]] uint64_t getSpeed() const;

    [[nodiscard]] double getTransmitSpd() const;

    [[nodiscard]] const std::vector<std::shared_ptr<GateControlEntry>> &getGateControlList() const;

    void addGateControlEntry(std::shared_ptr<GateControlEntry> &&gateControlEntry);

    void clearGCL();

    void sortGCL();

    void mergeGCL();

    [[nodiscard]] uint64_t getQsize() const;

    void setQsize(uint64_t _qsize);

    [[nodiscard]] uint32_t getQlen() const;

    void setQlen(uint32_t _qlen);

    [[nodiscard]] const std::vector<uint64_t> &getFrameQueue() const;

    void setFrameQueue(const std::vector<uint64_t> &frameQueue);
};


#endif //SCHEDPLUS_REFACTOR_PORT_H
