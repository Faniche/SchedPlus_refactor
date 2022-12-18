//
// Created by faniche on 22-11-14.
//

#include "MoGaSolver.h"
#include "../../components/node/EndSystem.h"

MoGaSolver::MoGaSolver(std::shared_ptr<Input> _input,
                       bool _debug = false,
                       int _generations = 100) :
        input(std::move(_input)),
        debug(_debug),
        generations(_generations) {
    std::sort(input->streams.begin(), input->streams.end(),
              [](const std::shared_ptr<Stream> &a, const std::shared_ptr<Stream> &b) {
                  return a->getPeriod() < b->getPeriod() || a->getLength() < b->getLength() ||
                         a->getPcp() < b->getPcp();
              });
    for (int i = 0; i < input->streams.size(); ++i)
        input->streamsId[input->streams[i]->getId()] = i;
    for (const auto &stream: input->streams) {
        if (hyperPeriod != 0)
            hyperPeriod = std::lcm(hyperPeriod, stream->getPeriod());
        else
            hyperPeriod = stream->getPeriod();
    }
}

void MoGaSolver::vSolve() {
    GA_Type ga_obj;
    ga_obj.problem_mode = EA::GA_MODE::NSGA_III;
    ga_obj.multi_threading = !debug;
    ga_obj.dynamic_threading = !debug;
    ga_obj.verbose = debug;
    ga_obj.population = 10;
    ga_obj.generation_max = generations;
    ga_obj.calculate_MO_objectives = std::bind(&MoGaSolver::calculate_MO_objectives, this, std::placeholders::_1);
    ga_obj.init_genes = bind(&MoGaSolver::init_genes, this, std::placeholders::_1, std::placeholders::_2);
    ga_obj.eval_solution = bind(&MoGaSolver::eval_solution, this, std::placeholders::_1, std::placeholders::_2);
    ga_obj.mutate = bind(&MoGaSolver::mutate, this, std::placeholders::_1, std::placeholders::_2,
                         std::placeholders::_3);
    ga_obj.crossover = bind(&MoGaSolver::crossover, this, std::placeholders::_1, std::placeholders::_2,
                            std::placeholders::_3);
    ga_obj.MO_report_generation = bind(&MoGaSolver::MO_report_generation, this, std::placeholders::_1,
                                       std::placeholders::_2, std::placeholders::_3);

    ga_obj.crossover_fraction = 0.7;
    ga_obj.mutation_rate = 0.4;

    ga_obj.solve();
    std::cout << "ga solver" << std::endl;
}

std::vector<std::shared_ptr<DirectedLink>> MoGaSolver::getRouteLinks(stream_id_t streamId, route_t routeId) const {
    return input->streams[streamId]->getRoutes()[routeId]->getLinks();
}

void MoGaSolver::setEachHopStartTime(const TtStreams &p, MyMiddleCost &c) {
    for (const auto &stream: input->streams) {
        if (!p.useNoWait && stream->getPcp() == P5)
            continue;
        stream_id_t streamId = stream->getId();
        const auto &route = getRouteLinks(streamId, p.routes[streamId]);
        sched_time_t accDelay = p.offsets[streamId];
        for (hop_t i = 0; i < route.size(); ++i) {
            accDelay += route[i]->getSrcNode()->getDpr();
            c.trafficOffsets[streamId][i] = accDelay;
            accDelay += (sched_time_t) (
                    stream->getLength() * route[i]->getSrcPort()->getTransmitSpd()
                    + route[i]->getPropSpeed() * route[i]->getLen()
            );
        }
        c.trafficOffsets[streamId][route.size()] = accDelay;
        if (stream->getPcp() == P6)
            c.ddlOrE2E[streamId] = accDelay;
        if (p.useNoWait && stream->getPcp() == P5)
            c.ddlOrE2E[streamId] = accDelay - p.offsets[streamId];
    }
}

bool MoGaSolver::checkDDLorE2E(MyMiddleCost &c,
                               bool useNoWait,
                               std::pair<stream_id_t, sched_time_t> &maxDdl,
                               std::pair<stream_id_t, sched_time_t> &maxE2E) {
    for (const auto &[streamId, pos]: input->streamsId) {
        const auto &stream = input->streams[pos];
        if (stream->getPcp() == P6
            && c.ddlOrE2E[streamId] >= stream->getDeliveryGuarantee()->getLowerVal())
            return false;
        if (stream->getPcp() == P6
            && maxDdl.second < c.ddlOrE2E[streamId])
            maxDdl = std::make_pair(streamId, c.ddlOrE2E[streamId]);
        if (c.ddlOrE2E.contains(streamId)
            && stream->getPcp() == P5) {
            if (c.ddlOrE2E[streamId] >= stream->getDeliveryGuarantee()->getLowerVal())
                return false;
            if (maxE2E.second < c.ddlOrE2E[streamId])
                maxE2E = std::make_pair(streamId, c.ddlOrE2E[streamId]);
        }
    }
    return true;
}

void MoGaSolver::groupStreams(const std::vector<route_t> &routes,
                              MyMiddleCost &c) {
    for (stream_id_t i = 0; i < routes.size(); ++i) {
        auto const &route = getRouteLinks(i, routes[i]);
        hop_t j = 0;
        for (const auto &link: route)
            c.linkFlows[link->getId()].emplace_back(i, j++);
    }
    std::map<stream_id_t, bool> streamVisit;

    for (const auto &stream: input->streams)
        streamVisit[stream->getId()] = false;
    group_id_t groupId = 0;
    for (const auto &stream: input->streams) {
        if (streamVisit[stream->getId()]) continue;
        std::vector<stream_id_t> group;
        std::queue<stream_id_t> tmp;
        streamVisit[stream->getId()] = true;
        group.push_back(stream->getId());
        tmp.push(stream->getId());
        while (!tmp.empty()) {
            const auto &route = getRouteLinks(stream->getId(), routes[tmp.front()]);
            tmp.pop();
            for (const auto &link: route) {
                for (const auto& [streamId, _]: c.linkFlows[link->getId()]) {
                    if (!streamVisit[streamId]) {
                        group.push_back(streamId);
                        tmp.push(streamId);
                        streamVisit[streamId] = true;
                    }
                }
            }
        }
        c.groupStream[groupId++] = group;
    }

    for (const auto &[_groupId, streamsInGroup]: c.groupStream) {
        for (const auto & streamId: streamsInGroup) {
            if (input->streams[input->streamsId[streamId]]->getPcp() == P5)
                c.cachedFlows[_groupId].push_back(streamId);
            else if (input->streams[input->streamsId[streamId]]->getPcp() == P6)
                c.uncachedFlows[_groupId].push_back(streamId);
        }
    }

    c.linkFlows.clear();
    for (const auto &[_groupId, streamsInGroup]: c.uncachedFlows) {
        for (auto const &streamId: streamsInGroup) {
            auto const &route = getRouteLinks(streamId, routes[streamId]);
            hop_t i = 0;
            for (const auto &link: route) {
                c.linkFlows[link->getId()].emplace_back(streamId, i++);
            }
        }
    }

}

void MoGaSolver::setLinkHyperperiod(const TtStreams &p, MyMiddleCost &c) {
    if (!c.linkHyperperiod.empty()) {
        c.linkHyperperiod.clear();
    }
    if (p.useNoWait) {
        for (const auto &[groupId, streamsInGroup]: c.groupStream) {
            for (const auto & streamId: streamsInGroup) {
                for (const auto &link: getRouteLinks(streamId, p.routes[streamId])) {
                    const auto &stream = input->streams[input->streamsId[streamId]];
                    if (c.linkHyperperiod[groupId].contains(link->getId()))
                        c.linkHyperperiod[groupId][link->getId()]
                            = std::lcm(c.linkHyperperiod[groupId][link->getId()], stream->getPeriod());
                    else
                        c.linkHyperperiod[groupId][link->getId()] = stream->getPeriod();
                }
            }
        }
    } else {
        for (const auto &[groupId, streamsInGroup]: c.uncachedFlows) {
            for (const auto & streamId: streamsInGroup) {
                const auto &stream = input->streams[input->streamsId[streamId]];
                for (const auto &link: getRouteLinks(streamId, p.routes[streamId])) {
                    if (c.linkHyperperiod[groupId].contains(link->getId()))
                        c.linkHyperperiod[groupId][link->getId()]
                            = std::lcm(c.linkHyperperiod[groupId][link->getId()], stream->getPeriod());
                    else
                        c.linkHyperperiod[groupId][link->getId()] = stream->getPeriod();
                }
            }
        }
    }
}

bool MoGaSolver::checkCollisionHelp(sched_time_t siPeriod, sched_time_t siMid, sched_time_t siLen,
                                    sched_time_t sjPeriod, sched_time_t sjMid, sched_time_t sjLen) {
    sched_time_t hpij = std::lcm(siPeriod, sjPeriod);
    sched_time_t dist = (siLen + sjLen) / 2 + IFG_TIME;
    if (siPeriod < sjPeriod) {
        sched_time_t _fi_mid = siMid % siPeriod;
        for (int k = 0; k < hpij / sjPeriod; ++k) {
            int d = (sjMid + k * sjPeriod) % siPeriod - _fi_mid;
            if (std::abs(d) < dist) {
                SPDLOG_LOGGER_TRACE(spdlog::get("console"), "dist = {}, d = {}", dist, d);
                return false;
            }
        }
    } else {
        sched_time_t _fj_mid = sjMid % sjPeriod;
        for (int k = 0; k < hpij / siPeriod; ++k) {
            int d = (siMid + k * siPeriod) % sjPeriod - _fj_mid;
            if (std::abs(d) < dist) {
                SPDLOG_LOGGER_TRACE(spdlog::get("console"), "dist = {}, d = {}", dist, d);
                return false;
            }
        }
    }
    return true;
}

bool MoGaSolver::checkCollision(MyMiddleCost &c, const std::vector<route_t> &routes) {
    for (auto const &[linkId, streamHop]: c.linkFlows) {
        double transmitSpd = input->links[linkId]->getSrcPort()->getTransmitSpd();
        for (int i = 0; i < streamHop.size(); ++i) {
            const auto &streamI = input->streams[input->streamsId[streamHop[i].first]];
            sched_time_t fiMid = c.trafficOffsets[streamHop[i].first][streamHop[i].second]
                                 + streamI->getLength() * transmitSpd / 2;
            for (int j = i + 1; j < streamHop.size(); ++j) {
                const auto &streamJ = input->streams[input->streamsId[streamHop[j].first]];
                sched_time_t fjMid = c.trafficOffsets[streamHop[j].first][streamHop[j].second]
                                     + streamJ->getLength() * transmitSpd / 2;
                if (!checkCollisionHelp(streamI->getPeriod(), fiMid, streamI->getLength() * transmitSpd,
                                        streamJ->getPeriod(), fjMid, streamJ->getLength() * transmitSpd)) {
                    return false;
                }
            }
        }
    }
    return true;
}

std::pair<sched_time_t, sched_time_t>
MoGaSolver::compressP6Help(sched_time_t siPeriod, sched_time_t siStart, sched_time_t siLen,
                           sched_time_t sjPeriod, sched_time_t sjStart, sched_time_t sjLen,
                           bool &couldMoveRight,
                           bool &couldMoveLeft) {
    sched_time_t hpij = std::lcm(siPeriod, sjPeriod);
    sched_time_t dist = (siLen + sjLen) / 2 + IFG_TIME;
    sched_time_t left = INT64_MIN;
    sched_time_t right = INT64_MAX;
    sched_time_t _sjMid = (sjStart + sjLen / 2) % sjPeriod;
    for (int k = 0; k < hpij / siPeriod; ++k) {
        /*           d = (sjMid + k * sjPeriod % siPeriod  - fi_mid*/
        sched_time_t d = (siStart + siLen / 2 + k * siPeriod) % sjPeriod - _sjMid;
        if (std::abs(d) == dist) {
            SPDLOG_LOGGER_TRACE(spdlog::get("console"), "dist = {}, d = {}", dist, d);
            return std::make_pair(left, right);
        } else if (d < 0) {
            /* right is negative, choose the bigger */
            couldMoveRight = true;
            if (right != INT64_MAX)
                right = (d + dist) < right ? right : (d - dist);
            else
                right = d + dist;
        } else if (d > 0) {
            couldMoveLeft = true;
            if (left != INT64_MIN)
                left = (d - dist) > left ? left : (d - dist);
            else
                left = d - dist;
        }
    }
    return std::make_pair(-left, -right);
}

void MoGaSolver::compressP6(const TtStreams &p, MyMiddleCost &c) {
    std::vector<std::pair<stream_id_t, sched_time_t>> offsets(p.offsets.size());
    for (int i = 0; i < p.offsets.size(); ++i)
        offsets.emplace_back(i, p.offsets[i]);
    std::sort(offsets.begin(), offsets.end(), [](std::pair<stream_id_t, sched_time_t> a, std::pair<stream_id_t, sched_time_t> b){
        return a.second < b.second;
    });
//    std::vector<stream_id_t> compressedStreams;
//    compressedStreams.push_back(input->streams[0]->getId());
    for (size_t i = 1; i < offsets.size(); ++i) {
        if (input->streams[offsets[i].first]->getPcp() != P6) continue;
        const auto& stream = input->streams[offsets[i].first];
        sched_time_t moveRight = INT64_MAX;
        sched_time_t moveLeft = INT64_MIN;
        std::vector<std::pair<sched_time_t, sched_time_t>> compressInterval;
        const auto &route = getRouteLinks(offsets[i].first, p.routes[offsets[i].first]);
        bool couldMoveRight = false;
        bool couldMoveLeft = false;
        for (hop_t j = 0; j < route.size(); ++j) {
            const auto &link = route[j];
            double transmitSpd = input->links[link->getId()]->getSrcPort()->getTransmitSpd();
            for (const auto &[streamId, hop]: c.linkFlows[link->getId()]) {
                if (streamId == stream->getId()) continue;
                bool hasCompressed = false;
                for (int k = 0; k < i; ++k) {
                    if (offsets[k].first == streamId)
                        hasCompressed = true;
                }
                const auto &streamComp = input->streams[input->streamsId[streamId]];
                auto comp = compressP6Help(stream->getPeriod(),
                                           c.trafficOffsets[stream->getId()][j],
                                           stream->getLength() * transmitSpd,
                                           streamComp->getPeriod(),
                                           c.trafficOffsets[streamId][hop],
                                           streamComp->getLength() * transmitSpd,
                                           couldMoveRight, couldMoveLeft);
                if (!couldMoveLeft && !couldMoveRight) break;
                compressInterval.emplace_back(std::move(comp));
            }
        }
        if (!couldMoveLeft && !couldMoveRight) continue;
        if (couldMoveRight) {
            std::sort(compressInterval.begin(), compressInterval.end(),
                      [](std::pair<sched_time_t, sched_time_t> a, std::pair<sched_time_t, sched_time_t> b) {
                          return a.second < b.second;
                      });
            moveRight = compressInterval[0].second;
        }
        if (couldMoveLeft) {
            std::sort(compressInterval.begin(), compressInterval.end(),
                      [](std::pair<sched_time_t, sched_time_t> a, std::pair<sched_time_t, sched_time_t> b) {
                          return a.first > b.first;
                      });
            moveLeft = compressInterval[0].first;
        }
        if (!couldMoveRight && couldMoveLeft) {
            for (size_t i = 0; i <= route.size(); ++i)
                c.trafficOffsets[stream->getId()][i] += moveLeft;
        } else if (!couldMoveLeft && couldMoveRight) {
            for (size_t i = 0; i <= route.size(); ++i)
                c.trafficOffsets[stream->getId()][i] += moveRight;
        } else {
            sched_time_t move = std::abs(moveRight) < std::abs(moveLeft) ? moveRight : moveLeft;
            for (size_t i = 0; i <= route.size(); ++i)
                c.trafficOffsets[stream->getId()][i] += -move;
        }

    }
}

void MoGaSolver::scheduleP5(const TtStreams &p, MyMiddleCost &c) {
    for (const auto &[groupId, streamsInGroup]: c.cachedFlows) {
        for (auto const &streamIdI: streamsInGroup) {
            const auto &streamI = input->streams[input->streamsId[streamIdI]];
            const auto &route = getRouteLinks(streamIdI, p.routes[streamIdI]);
            for (hop_t i = 0; i < route.size(); ++i) {
                double transmitSpd = route[i]->getSrcPort()->getTransmitSpd();
                sched_time_t streamILen = streamI->getLength() * transmitSpd;
                link_id_t linkId = route[i]->getId();


                for (int j = 0; j < hyperPeriod / streamI->getPeriod(); ++j) {
                    sched_time_t send = 0;
                    if (i > 0)
                        send = c.p5TrafficOffsets[streamIdI][i - 1][j].first +
                               streamI->getLength() * route[i - 1]->getSrcPort()->getTransmitSpd() +
                               route[i - 1]->getPropSpeed() * route[i - 1]->getLen();
                    else
                        send = (int64_t) (p.offsets[streamIdI] + j * streamI->getPeriod());
                    c.p5TrafficOffsets[streamIdI][i][j] =
                            std::make_pair(send, send % c.linkHyperperiod[linkId]);
                }

            }
        }
    }
    for (const auto &streamIdI: c.cachedFlows) {
        const auto &streamI = input->streams[input->streamsId[streamIdI]];
        const auto &route = getRouteLinks(streamIdI, p.routes[streamIdI]);
        for (hop_t i = 0; i < route.size(); ++i) {
            double transmitSpd = route[i]->getSrcPort()->getTransmitSpd();
            sched_time_t streamILen = streamI->getLength() * transmitSpd;
            link_id_t linkId = route[i]->getId();
            /* set start time at beginning of check collision */
            for (int j = 0; j < hyperPeriod / streamI->getPeriod(); ++j) {
                sched_time_t send = 0;
                if (i > 0)
                    send = c.p5TrafficOffsets[streamIdI][i - 1][j].first +
                           streamI->getLength() * route[i - 1]->getSrcPort()->getTransmitSpd() +
                           route[i - 1]->getPropSpeed() * route[i - 1]->getLen();
                else
                    send = (int64_t) (p.offsets[streamIdI] + j * streamI->getPeriod());
                c.p5TrafficOffsets[streamIdI][i][j] =
                        std::make_pair(send, send % c.linkHyperperiod[linkId]);
            }
            for (const auto &[streamIdJ, hopJ]: c.linkFlows[linkId]) {
                const auto &streamJ = input->streams[input->streamsId[streamIdJ]];
                sched_time_t streamJLen = streamJ->getLength() * transmitSpd;
                sched_time_t d = (streamILen + streamJLen) / 2 + IFG_TIME;
                if (streamJ->getPcp() == P6) {
                    sched_time_t sjMid = c.trafficOffsets[streamIdJ][hopJ] + streamJLen / 2;
                    sched_time_t sjMidMod = sjMid % streamJ->getPeriod();
                    for (int j = 0; j < hyperPeriod / streamI->getPeriod(); ++j) {
                        sched_time_t siMid = c.p5TrafficOffsets[streamIdI][i][j].first + streamILen / 2;
                        sched_time_t siMidMod = siMid % streamJ->getPeriod();
                        if (siMidMod > sjMidMod && siMidMod - sjMidMod < d) {
                            /*                                           (d - (siMidMod - sjMidMod))*/
                            c.p5TrafficOffsets[streamIdI][i][j].first += (d - siMidMod + sjMidMod);
                            c.p5TrafficOffsets[streamIdI][i][j].second =
                                    c.p5TrafficOffsets[streamIdI][i][j].first % c.linkHyperperiod[linkId];
                        } else if (siMidMod < sjMidMod && sjMidMod - siMidMod < d) {
                            /* (sjMidMod + streamJLen / 2 - (siMidMod + streamILen / 2)) */
                            c.p5TrafficOffsets[streamIdI][i][j].first += (sjMidMod - siMidMod + d);
                            c.p5TrafficOffsets[streamIdI][i][j].second =
                                    c.p5TrafficOffsets[streamIdI][i][j].first % c.linkHyperperiod[linkId];
                        }
                    }
                } else if (streamJ->getPcp() == P5) {
                    for (const auto &[sjIdx, sjOffset]: c.p5TrafficOffsets[streamIdJ][hopJ]) {
                        sched_time_t sjMid = sjOffset.first + streamJLen / 2;
                        sched_time_t sjMidMod = sjOffset.second + streamJLen / 2;
                        for (const auto &[siIdx, siOffset]: c.p5TrafficOffsets[streamIdI][i]) {
                            sched_time_t siMid = siOffset.first + streamILen / 2;
                            sched_time_t siMidMod = siOffset.second + streamILen / 2;
                            if (std::abs(siMidMod - sjMidMod) <= d) {
                                if (!c.reuseGroupIndex.contains(linkId))
                                    c.reuseGroupIndex[linkId] = 0;
                                std::pair<sched_time_t, sched_time_t> _siOffset = siOffset;
                                std::pair<sched_time_t, sched_time_t> _sjOffset = sjOffset;

                                if (std::abs(siMid - sjMid) > d) {
                                    if (siMidMod > sjMidMod) {
//                                        if (siMid / c.linkHyperperiod[linkId] == )
                                        _siOffset.first += (d - siMidMod + sjMidMod);
                                        _siOffset.second += (d - siMidMod + sjMidMod);
                                    } else {

                                    }
                                } else {
                                    if (siMidMod > sjMidMod) {

                                    } else {

                                    }
                                }
                                c.linkReuseMapRecorder[linkId][c.reuseGroupIndex[linkId]][streamIdI] = _siOffset;
                                c.linkReuseMapRecorder[linkId][c.reuseGroupIndex[linkId]][streamIdJ] = _sjOffset;
                            }

                            if (siMidMod > sjMidMod && siMidMod - sjMidMod < d) {
                                /*                                           (d - (siMidMod - sjMidMod))*/
                                c.p5TrafficOffsets[streamIdI][i][siIdx].first += (d - siMidMod + sjMidMod);
                                c.p5TrafficOffsets[streamIdI][i][siIdx].second =
                                        c.p5TrafficOffsets[streamIdI][i][siIdx].first % c.linkHyperperiod[linkId];
                            } else if (siMidMod < sjMidMod && sjMidMod - siMidMod < d) {
                                /* (sjMidMod + streamJLen / 2 - (siMidMod + streamILen / 2)) */
                                c.p5TrafficOffsets[streamIdI][i][siIdx].first += (sjMidMod - siMidMod + d);
                                c.p5TrafficOffsets[streamIdI][i][siIdx].second =
                                        c.p5TrafficOffsets[streamIdI][i][siIdx].first % c.linkHyperperiod[linkId];
                            }
                        }
                    }
                }
            }
            c.linkFlows[linkId].emplace_back(streamIdI, i);
        }
    }
}

void MoGaSolver::init_genes(TtStreams &p, const std::function<double(void)> &rnd01) {
    p.offsets.assign(input->streams.size(), 0);
    p.routes.assign(input->streams.size(), 0);
    /* set route index */
    for (const auto &[streamId, pos]: input->streamsId) {
        if (input->streams[pos]->getRoutes().size() == 1)
            p.routes[streamId] = 0;
        else {
            do {
                p.routes[streamId] = input->streams[pos]->getRoutes().size() * rnd01();
            } while (input->streams[pos]->getRoutes()[p.routes[streamId]]->getE2E()
                     > input->streams[pos]->getDeliveryGuarantee()->getLowerVal());
        }

    }
    p.offsets[input->streams[0]->getId()] = 0;
    for (int i = 1; i < input->streams.size(); ++i) {
        auto stream = input->streams[i];
        stream_id_t streamId = stream->getId();
        sched_time_t upperBound = stream->getPeriod()
                                  - stream->getSrc()->getDpr()
                                  - stream->getLength() *
                                    std::static_pointer_cast<EndSystem>(stream->getSrc())->getPort()->getTransmitSpd();
        if (stream->getPcp() == P6) {
            sched_time_t tmp = stream->getDeliveryGuarantee()->getLowerVal()
                               - stream->getRoutes()[p.routes[streamId]]->getE2E();
            upperBound = std::min(upperBound, tmp);
            p.offsets[streamId] = upperBound * rnd01();
        } else if (stream->getPcp() == P5) {
            do {
                p.offsets[streamId] = upperBound * rnd01();
            } while (p.offsets[streamId]
                     + stream->getRoutes()[p.routes[streamId]]->getE2E()
                     > stream->getDeliveryGuarantee()->getLowerVal());
        }
    }
//    p.useNoWait = rnd01() <= 0.5;
    p.useNoWait = false;
}

bool MoGaSolver::eval_solution(const TtStreams &p, MyMiddleCost &c) {
    setEachHopStartTime(p, c);

    std::pair<stream_id_t, sched_time_t> max_ddl;
    std::pair<stream_id_t, sched_time_t> max_e2e;
    if (!checkDDLorE2E(c, p.useNoWait, max_ddl, max_e2e))
        return false;
    if (p.useNoWait) {
        for (const auto &[streamId, pos]: input->streamsId) {
            auto const &stream = input->streams[input->streamsId[streamId]];
            const auto &route = getRouteLinks(streamId, p.routes[streamId]);
            for (hop_t i = 0; i < route.size(); ++i) {
                link_id_t linkId = route[i]->getId();
                c.linkFlows[linkId].emplace_back(streamId, i);
            }
        }
        if (!checkCollision(c, p.routes))
            return false;
    } else {
        groupStreams(p.routes, c);
        setLinkHyperperiod(p, c);
        if (!checkCollision(c, p.routes))
            return false;
//        compressP6(p, c);
        scheduleP5(p, c);
    }

    for (auto const &[linkId, streamHop]: c.linkFlows) {
        if (input->links[linkId]->getSrcNode()->getNodeType() == SWITCH) {
            for (auto const &[streamId, hop]: streamHop) {
                sched_time_t period = input->streams[input->streamsId[streamId]]->getPeriod();
                if (!c.linkGclSize.contains(linkId)) {
                    c.linkGclSize[linkId] = c.linkHyperperiod[linkId] / period;
                } else {
                    c.linkGclSize[linkId] += c.linkHyperperiod[linkId] / period;
                }
            }
        }
    }


    for (int i = 0; i < input->streams.size(); ++i) {
        auto si = input->streams[input->streamsId[i]];
        auto ri = si->getRoutes()[p.routes[input->streamsId[i]]];
        for (int j = i + 1; j < input->streams.size(); ++j) {
            auto sj = input->streams[input->streamsId[j]];
            auto rj = si->getRoutes()[p.routes[input->streamsId[j]]];
            for (auto &siLink: ri->getLinks()) {
                for (auto &sjLink: rj->getLinks()) {
                    if (siLink->getId() == sjLink->getId()) {

                    }
                }
            }
        }
    }
    return true;
}

double MoGaSolver::get_shrink_scale(int n_generation, const std::function<double(void)> &rnd01) {

}

TtStreams MoGaSolver::mutate(
        const TtStreams &X_base,
        const std::function<double(void)> &rnd01,
        double shrink_scale) {

}

TtStreams MoGaSolver::crossover(
        const TtStreams &X1,
        const TtStreams &X2,
        const std::function<double(void)> &rnd01) {

}

vector<double> MoGaSolver::calculate_MO_objectives(const GA_Type::thisChromosomeType &X) {
    return {
//                X.middle_costs.variance,
            X.middle_costs.totalCache,
            X.middle_costs.v_transmit,
            X.middle_costs.total_gcl
    };
}

void MoGaSolver::MO_report_generation(
        int generation_number,
        const EA::GenerationType<TtStreams, MyMiddleCost> &last_generation,
        const vector<unsigned int> &pareto_front) {
    (void) last_generation;

}
