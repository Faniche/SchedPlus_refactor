//
// Created by faniche on 22-10-17.
//

#ifndef SCHEDPLUS_REFACTOR_STREAM_H
#define SCHEDPLUS_REFACTOR_STREAM_H


#include <vector>
#include <map>
#include "../type.h"
#include "DeliveryGuarantee.h"
#include "Node.h"
#include "Route.h"
#include "Frame.h"
#include "DirectedLink.h"


class Stream {
protected:
    stream_id id;
    uint64_t period = 0;
    uint32_t length = 0;
    PRIORITY_CODE_POINT pcp;
    std::unique_ptr<DeliveryGuarantee> deliveryGuarantee;
    /* The source node of a Flow */
    std::shared_ptr<Node> src;

    /* The destination node of a Flow */
    std::shared_ptr<Node> dest;

    std::vector<std::shared_ptr<Route>> routes;

    uint32_t chosenRoute = 0;
    std::vector<std::shared_ptr<Frame>> frames;
    std::map<std::shared_ptr<DirectedLink>, std::vector<Frame>> linkFrames;

public:
    Stream(stream_id id, uint64_t period, uint32_t length, PRIORITY_CODE_POINT pcp, std::shared_ptr<Node> &src, std::shared_ptr<Node> &dest);

    [[nodiscard]] stream_id getId() const;

    void setId(stream_id _id);

    [[nodiscard]] uint64_t getPeriod() const;

    void setPeriod(uint64_t period);

    [[nodiscard]] uint32_t getLength() const;

    void setLength(uint32_t length);

    [[nodiscard]] PRIORITY_CODE_POINT getPcp() const;

    void setPcp(PRIORITY_CODE_POINT pcp);

    [[nodiscard]] const DeliveryGuarantee *getDeliveryGuarantee() const;

    void setDeliveryGuarantee(std::unique_ptr<DeliveryGuarantee> &&_deliveryGuarantee);

    [[nodiscard]] std::shared_ptr<Node> getSrc() const;

    void setSrc(std::shared_ptr<Node> &src);

    [[nodiscard]] std::shared_ptr<Node> getDest() const;

    void setDest(std::shared_ptr<Node> &dest);

    [[nodiscard]] const std::vector<std::shared_ptr<Route>> &getRoutes() const;

    void setRoutes(const std::vector<std::shared_ptr<Route>> &routes);

    [[nodiscard]] uint32_t getChosenRoute() const;

    void setChosenRoute(uint32_t selectedRouteInx);

    [[nodiscard]] const std::map<std::shared_ptr<DirectedLink>, std::vector<Frame>> &getLinkFrames() const;

};


#endif //SCHEDPLUS_REFACTOR_STREAM_H
