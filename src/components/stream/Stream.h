//
// Created by faniche on 22-10-17.
//

#ifndef SCHEDPLUS_REFACTOR_STREAM_H
#define SCHEDPLUS_REFACTOR_STREAM_H


#include <vector>
#include <map>
#include <random>
#include "../../type.h"
#include "DeliveryGuarantee.h"
#include "../node/Node.h"
#include "../link/Route.h"
#include "Frame.h"
#include "../link/DirectedLink.h"


class Stream {
protected:
    stream_id_t id;
    sched_time_t period = 0;
    uint32_t length = 0;
    pcp_t pcp;
    std::unique_ptr<DeliveryGuarantee> deliveryGuarantee;
    /* The source node of a Flow */
    std::shared_ptr<Node> src;

    /* The destination node of a Flow */
    std::shared_ptr<Node> dest;

    std::vector<std::shared_ptr<Route>> routes;

    sched_time_t srcTransmitDelay;

    uint32_t chosenRoute = 0;
    std::vector<std::shared_ptr<Frame>> frames;
    std::map<std::shared_ptr<DirectedLink>, std::vector<Frame>> linkFrames;

    static const std::vector<int> randPeriod;
public:
    Stream(stream_id_t id, sched_time_t period, uint32_t length, pcp_t pcp, std::shared_ptr<Node> &src, std::shared_ptr<Node> &dest);

    [[nodiscard]] stream_id_t getId() const;

    void setId(stream_id_t _id);

    [[nodiscard]] sched_time_t getPeriod() const;

    void setPeriod(sched_time_t period);

    [[nodiscard]] uint32_t getLength() const;

    void setLength(uint32_t length);

    [[nodiscard]] pcp_t getPcp() const;

    void setPcp(pcp_t pcp);

    [[nodiscard]] const DeliveryGuarantee *getDeliveryGuarantee() const;

    void setDeliveryGuarantee(std::unique_ptr<DeliveryGuarantee> &&_deliveryGuarantee);

    [[nodiscard]] std::shared_ptr<Node> getSrc() const;

    void setSrc(std::shared_ptr<Node> &src);

    [[nodiscard]] std::shared_ptr<Node> getDest() const;

    void setDest(std::shared_ptr<Node> &dest);

    [[nodiscard]] const std::vector<std::shared_ptr<Route>> &getRoutes() const;

    void setRoutes(std::vector<std::shared_ptr<Route>> &&_routes);

    [[nodiscard]] const sched_time_t &getSrcTransmitDelay() const;

    [[nodiscard]] uint32_t getChosenRoute() const;

    void setChosenRoute(uint32_t selectedRouteInx);

    [[nodiscard]] const std::map<std::shared_ptr<DirectedLink>, std::vector<Frame>> &getLinkFrames() const;

    void setFramesOffset (size_t idx, sched_time_t offset);

    static sched_time_t getRandomPeriod(pcp_t pcp);

    static sched_time_t getRandomFrameLength(pcp_t pcp);

};


#endif //SCHEDPLUS_REFACTOR_STREAM_H
