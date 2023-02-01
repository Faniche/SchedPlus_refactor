//
// Created by faniche on 22-11-14.
//

//#define OPENGA_EXTERN_LOCAL_VARS
#include <fstream>
#include "MoGaSolver.h"
#include "../SaveSolution.h"

MoGaSolver::MoGaSolver(std::shared_ptr<Input> _input,
                       bool _debug = false,
                       int _generations = 100,
                       bool _flagUseNoWait = false) :
        input(std::move(_input)),
        debug(_debug),
        generations(_generations),
        flagUseNoWait(_flagUseNoWait) {}

void MoGaSolver::vSolve(const std::string &path, int32_t runId) {
    GA_Type ga_obj;
    ga_obj.problem_mode = EA::GA_MODE::NSGA_III;
    ga_obj.multi_threading = !debug;
    ga_obj.dynamic_threading = !debug;
    ga_obj.verbose = debug;
    ga_obj.population = 50;
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

    EA::Chronometer timer;
    timer.tic();
    ga_obj.solve();
    double cost = timer.toc();
    save_results(ga_obj, path, runId, cost);
    std::cout << "ga solver" << std::endl;
}


void MoGaSolver::setEachHopStartTime(const TtStreams &p, MyMiddleCost &c) {
    for (const auto &[groupId, streamsInGroup]: c.groupStream) {
        for (const auto &streamId: streamsInGroup) {
            const auto &stream = input->getStream(streamId);
            int pos = input->getStreamPos(streamId);
            if (!p.useNoWait && stream->getPcp() == P5)
                continue;
            const auto &route = input->getRouteLinks(streamId, p.routes[pos]);
            sched_time_t accDelay = p.offsets[pos];
            for (hop_t i = 0; i < route.size(); ++i) {
                accDelay += route[i]->getSrcNode()->getDpr();
                double transmitDelay = stream->getLength() * route[i]->getSrcPort()->getTransmitSpd();
                c.trafficOffsets[groupId][streamId][i] = std::make_pair(accDelay, transmitDelay);
                accDelay += (sched_time_t) (
                        transmitDelay
                        + route[i]->getPropSpeed() * route[i]->getLen()
                );
            }
            c.trafficOffsets[groupId][streamId][route.size()] = std::make_pair(accDelay, 0);
            if (stream->getPcp() == P6)
                c.ddlOrE2E[streamId] = accDelay;
            if (p.useNoWait && stream->getPcp() == P5)
                c.ddlOrE2E[streamId] = accDelay - p.offsets[pos];
        }
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

void MoGaSolver::groupStreamsInit(const std::vector<route_t> &routes,
                                  map<group_id_t, vector<stream_id_t>> &groupStream) {
    map<link_id_t, vector<std::pair<stream_id_t, hop_t>>> linkFlows;
    for (const auto &[streamId, pos]: input->streamsId) {
        auto const &route = input->getRouteLinks(streamId, routes[pos]);
        hop_t j = 0;
        for (const auto &link: route)
            linkFlows[link->getId()].emplace_back(streamId, j++);
    }

    std::map<stream_id_t, bool> streamVisit;
    for (const auto &stream: input->streams)
        streamVisit[stream->getId()] = false;
    group_id_t groupId = 0;
    for (const auto &stream: input->streams) {
        if (streamVisit[stream->getId()]) continue;
        std::vector<stream_id_t> group;
        std::queue<stream_id_t> streamQueue;
        streamVisit[stream->getId()] = true;
        group.push_back(stream->getId());
        streamQueue.push(stream->getId());
        while (!streamQueue.empty()) {
            const auto &route = input->getRouteLinks(streamQueue.front(),
                                                     routes[input->getStreamPos(streamQueue.front())]);
            streamQueue.pop();
            for (const auto &link: route) {
                for (const auto &[streamId, _]: linkFlows[link->getId()]) {
                    if (!streamVisit[streamId]) {
                        group.push_back(streamId);
                        streamQueue.push(streamId);
                        streamVisit[streamId] = true;
                    }
                }
            }
        }
        groupStream[groupId++] = group;
    }
    for (auto &[_groupId, streamsInGroup]: groupStream) {
        std::sort(streamsInGroup.begin(), streamsInGroup.end(),
                  [this](stream_id_t &a, stream_id_t &b) {
                      return input->getStream(a)->getPcp() > input->getStream(b)->getPcp()
                             || input->getStream(a)->getPeriod() < input->getStream(b)->getPeriod();
                  });
    }
}

void MoGaSolver::groupStreamsEval(const std::vector<route_t> &routes, MyMiddleCost &c) {
    for (const auto &[groupId, streamsInGroup]: c.groupStream) {
        for (const auto &streamId: streamsInGroup) {
            if (input->getStream(streamId)->getPcp() == P5)
                c.cachedFlows[groupId].push_back(streamId);
            else if (input->getStream(streamId)->getPcp() == P6)
                c.uncachedFlows[groupId].push_back(streamId);
        }
    }
    c.linkFlows.clear();
    for (const auto &[groupId, streamsInGroup]: c.uncachedFlows) {
        for (auto const &streamId: streamsInGroup) {
            auto const &route = input->getRouteLinks(streamId, routes[input->getStreamPos(streamId)]);
            hop_t i = 0;
            for (const auto &link: route) {
                c.linkFlows[link->getId()].emplace_back(streamId, i++);
            }
        }
    }
    for (const auto &[groupId, streamsInGroup]: c.groupStream) {
        for (auto const &streamId: streamsInGroup) {
            if (c.groupHyperPeriod.contains(groupId))
                c.groupHyperPeriod[groupId] = std::lcm(c.groupHyperPeriod[groupId],
                                                       input->getStream(streamId)->getPeriod());
            else
                c.groupHyperPeriod[groupId] = input->getStream(streamId)->getPeriod();
        }
    }
}

void MoGaSolver::setLinkHyperperiod(const TtStreams &p, MyMiddleCost &c) {
    if (!c.linkHyperperiod.empty()) {
        c.linkHyperperiod.clear();
    }
    if (p.useNoWait) {
        for (const auto &[groupId, streamsInGroup]: c.groupStream) {
            for (const auto &streamId: streamsInGroup) {
                for (const auto &link: input->getRouteLinks(streamId, p.routes[input->getStreamPos(streamId)])) {
                    const auto &stream = input->getStream(streamId);
                    if (c.linkHyperperiod.contains(link->getId()))
                        c.linkHyperperiod[link->getId()]
                                = std::lcm(c.linkHyperperiod[link->getId()], stream->getPeriod());
                    else
                        c.linkHyperperiod[link->getId()] = stream->getPeriod();
                }
            }
        }
    } else {
        for (const auto &[groupId, streamsInGroup]: c.uncachedFlows) {
            for (const auto &streamId: streamsInGroup) {
                const auto &stream = input->getStream(streamId);
                for (const auto &link: input->getRouteLinks(streamId, p.routes[input->getStreamPos(streamId)])) {
                    if (c.linkHyperperiod.contains(link->getId()))
                        c.linkHyperperiod[link->getId()]
                                = std::lcm(c.linkHyperperiod[link->getId()], stream->getPeriod());
                    else
                        c.linkHyperperiod[link->getId()] = stream->getPeriod();
                }
            }
        }
    }
}

void MoGaSolver::setLinkStreamInterval(const TtStreams &p, MyMiddleCost &c) {
    for (const auto &[groupId, streamsInGroup]: c.groupStream) {
        for (const auto &streamId: streamsInGroup) {
//                sched_time_t streamPeriod = input->getStream(streamId)->getPeriod();
            const auto stream = input->getStream(streamId);
            if (stream->getPcp() == P5 && !p.useNoWait)  continue;
            const auto &route = input->getRouteLinks(streamId, p.routes[input->getStreamPos(streamId)]);
            for (hop_t i = 0; i < route.size(); ++i) {
                for (int j = 0; j < c.linkHyperperiod[route[i]->getId()] / stream->getPeriod(); ++j) {
                    sched_time_t t = c.trafficOffsets[groupId][streamId][i].first + j * stream->getPeriod();
                    std::string gsv;
                    if (stream->getPcp() == P5)
                        gsv = "00100000";
                    else if (stream->getPcp() == P6)
                        gsv = "01000000";
                    auto gce = std::make_tuple(t, t + c.trafficOffsets[groupId][streamId][i].second, gsv);
                    c.linkInterval[route[i]->getId()].emplace(gce);
                    if (!p.useNoWait)
                        c.linkIntervalDuplex[route[i]->getId()][gce].emplace(t, t + c.trafficOffsets[groupId][streamId][i].second, streamId);
                }
            }
        }
    }
}

bool MoGaSolver::checkCollisionWithInterval(const TtStreams &p, MyMiddleCost &c) {
    for (auto &[_, intervals]: c.linkInterval) {
        auto prev = intervals.end();
        for (auto iter = intervals.begin(); iter != intervals.end(); ++iter) {
            if (prev != intervals.end() && get<1>(*prev) + IFG_TIME > get<0>(*iter)) {
                return false;
            }
            prev = iter;
        }
    }
    return true;
}

bool MoGaSolver::checkCollisionWithStreamHelp(sched_time_t siPeriod, sched_time_t siMid, sched_time_t siLen,
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

bool MoGaSolver::checkCollisionWithStream(const TtStreams &p, MyMiddleCost &c) {
    c.linkFlows.clear();
    for (const auto &[streamId, pos]: input->streamsId) {
        auto const &stream = input->getStream(streamId);
        const auto &route = input->getRouteLinks(streamId, p.routes[pos]);
        for (hop_t i = 0; i < route.size(); ++i) {
            link_id_t linkId = route[i]->getId();
            c.linkFlows[linkId].emplace_back(streamId, i);
        }
    }
    for (auto const &[linkId, streamHop]: c.linkFlows) {
        double transmitSpd = input->links[linkId]->getSrcPort()->getTransmitSpd();
        for (int i = 0; i < streamHop.size(); ++i) {
            const auto &streamI = input->streams[input->streamsId[streamHop[i].first]];
            group_id_t gi = getGrpIdOfStream(streamI->getId(), c);
            sched_time_t siLen = c.trafficOffsets[gi][streamHop[i].first][streamHop[i].second].second;
            sched_time_t siMid = c.trafficOffsets[gi][streamHop[i].first][streamHop[i].second].first + siLen / 2;
            for (int j = i + 1; j < streamHop.size(); ++j) {
                const auto &streamJ = input->streams[input->streamsId[streamHop[j].first]];
                group_id_t gj = getGrpIdOfStream(streamJ->getId(), c);
                sched_time_t sjLen = c.trafficOffsets[gj][streamHop[j].first][streamHop[j].second].second;
                sched_time_t sjMid = c.trafficOffsets[gj][streamHop[j].first][streamHop[j].second].first + sjLen / 2;
                if (!checkCollisionWithStreamHelp(streamI->getPeriod(), siMid, siLen,
                                                  streamJ->getPeriod(), sjMid, sjLen)) {
                    return false;
                }
            }
        }
    }

    for (auto &[linkId, intervals]: c.linkInterval) {
        auto prev = intervals.end();
        for (auto iter = intervals.begin(); iter != intervals.end(); ++iter) {
            if (prev != intervals.end() && get<1>(*prev) + IFG_TIME == get<0>(*iter)) {
                std::string gsv = get<2>(*iter);
                if (get<2>(*prev) != get<2>(*iter))
                    gsv = "01100000";
                auto newGce = std::make_tuple(get<0>(*prev), get<1>(*iter), gsv);
                iter = c.linkInterval[linkId].erase(prev, iter.operator++());
                iter = c.linkInterval[linkId].insert(iter, newGce);
            }
            prev = iter;
        }
    }
    return true;
}

bool MoGaSolver::scheduleP5Help(const TtStreams &p, MyMiddleCost &c) {

    return true;
}

bool MoGaSolver::scheduleP5(const TtStreams &p, MyMiddleCost &c) {
    c.totalCache = 0;
    for (const auto &[groupId, streamsInGroup]: c.cachedFlows) {
        for (auto const &streamId: streamsInGroup) {
            const auto &stream = input->getStream(streamId);
            const auto &route = input->getRouteLinks(streamId, p.routes[input->getStreamPos(streamId)]);
            for (hop_t i = 0; i < route.size(); ++i) {
                double transmitSpd = route[i]->getSrcPort()->getTransmitSpd();
                link_id_t linkId = route[i]->getId();
                /* set start time and check collision */
                for (int j = 0; j < c.groupHyperPeriod[groupId] / stream->getPeriod(); ++j) {
                    sched_time_t send = 0;
                    sched_time_t transmitDelay = stream->getLength() * route[i]->getSrcPort()->getTransmitSpd();
                    if (i > 0) {
                        /* 2 -- n */
                        send = get<0>(c.p5TrafficOffsets[streamId][j][i - 1]) +
                               get<2>(c.p5TrafficOffsets[streamId][j][i - 1]) +
                               route[i - 1]->getPropSpeed() * route[i - 1]->getLen() +
                               input->links[linkId]->getSrcNode()->getDpr();
                    } else {
                        /* 1 */
                        send = (int64_t) (p.offsets[input->getStreamPos(streamId)] + j * stream->getPeriod());
                    }
                    sched_time_t siMid = send + transmitDelay / 2;
                    if (!c.linkHyperperiod.contains(linkId))
                        c.linkHyperperiod[linkId] = stream->getPeriod();
                    sched_time_t siMidMod = siMid % c.linkHyperperiod[linkId];
                    sched_time_t siStart = send % c.linkHyperperiod[linkId];
                    sched_time_t siEnd = (siStart + transmitDelay) % c.linkHyperperiod[linkId];
                    if (siEnd < siStart)
                        return false;
                    auto newGce = std::make_tuple(siStart, siEnd, "00100000");
                    bool hasMoveBack = false;
                    for (auto iter = c.linkInterval[linkId].begin(); iter != c.linkInterval[linkId].end(); ++iter) {
                        sched_time_t sjStart = get<0>(*iter);
                        sched_time_t sjEnd = get<1>(*iter);
                        sched_time_t sjMid = sjStart + (sjEnd - sjStart) / 2;
                        sched_time_t d = (transmitDelay + sjEnd - sjStart) / 2 + IFG_TIME;

                        /* no collision with next gce, break the loop */
                        if (hasMoveBack && siEnd + IFG_TIME < sjStart)
                            break;
                        while (std::abs(sjMid - siMidMod) > d && iter != c.linkInterval[linkId].end()) {
                            iter++;
                            if (iter != c.linkInterval[linkId].end()) {
                                sjStart = get<0>(*iter);
                                sjEnd = get<1>(*iter);
                                sjMid = sjStart + (sjEnd - sjStart) / 2;
                                d = (transmitDelay + sjEnd - sjStart) / 2 + IFG_TIME;
                            }
                        }

                        sched_time_t moveBack = 0;
                        if (std::abs(sjMid - siMidMod) <= d) {
                            get<0>(newGce) = get<0>(newGce) < sjStart ? get<0>(newGce) : sjStart;
                            get<1>(newGce) = get<1>(newGce) < sjEnd ? sjEnd : get<1>(newGce);
                            if (c.linkIntervalDuplex[linkId][*iter].size() == 1 && get<2>(*iter) == "01000000") {
                                if (siStart < sjStart && sjStart < siEnd + IFG_TIME)
                                    return false;
                                if (siStart >= sjStart) {
                                    moveBack = sjEnd + IFG_TIME - siStart;
                                    siStart  += moveBack;
                                    siEnd    += moveBack;
                                    siMidMod += moveBack;
                                    siMid    += moveBack;
                                    siEnd    %= c.linkHyperperiod[linkId];
                                    if (siEnd < siStart)
                                        return false;
                                    send += moveBack;
                                    c.totalCache += moveBack;
                                    get<1>(newGce) += moveBack;
                                }
                                c.linkIntervalDuplex[linkId][newGce].emplace(send, send + transmitDelay, streamId);
                                c.linkIntervalDuplex[linkId][newGce].insert(c.linkIntervalDuplex[linkId][*iter].begin(),
                                                                            c.linkIntervalDuplex[linkId][*iter].end());
                                c.linkIntervalDuplex[linkId].erase(*iter);
                                iter = c.linkInterval[linkId].erase(iter);
                                iter = c.linkInterval[linkId].insert(iter, newGce);
                            } else {
                                int intervalUseCount = 0;
                                sched_time_t _siMid = siMid;
                                for (const auto &[skStart, skEnd, _streamId]: c.linkIntervalDuplex[linkId][*iter]) {
                                    sched_time_t _d = (transmitDelay + skEnd - skStart) / 2 + IFG_TIME;
                                    sched_time_t skMid = (skStart + skEnd) / 2;
                                    if (std::abs(skMid - _siMid) <= _d) {
                                        intervalUseCount++;
                                        if (intervalUseCount > 1) {
                                            spdlog::get("console")->debug("{}:{}: something error", __FILE__, __LINE__);
                                            return false;
                                        }
                                        if (send < skStart && skStart < send + transmitDelay + IFG_TIME)
                                            return false;
                                        if (send > skStart) {
                                            moveBack = skEnd + IFG_TIME - send;
                                            siStart  += moveBack;
                                            siEnd    += moveBack;
                                            siMidMod += moveBack;
                                            siMid    += moveBack;
                                            siEnd    %= c.linkHyperperiod[linkId];
                                            if (siEnd < siStart)
                                                return false;
                                            send += moveBack;
                                            c.totalCache += moveBack;
                                            if (siEnd > get<1>(newGce))
                                                get<1>(newGce) += moveBack;
                                        }
                                        _siMid += moveBack;
                                    }
                                }
                                c.linkIntervalDuplex[linkId][newGce].emplace(send, send + transmitDelay, streamId);
                                if (*iter != newGce) {
                                    c.linkIntervalDuplex[linkId][newGce].insert(c.linkIntervalDuplex[linkId][*iter].begin(),
                                                                                c.linkIntervalDuplex[linkId][*iter].end());
                                    c.linkIntervalDuplex[linkId].erase(*iter);
                                    iter = c.linkInterval[linkId].erase(iter);
                                    iter = c.linkInterval[linkId].insert(iter, newGce);
                                }
                            }

                            if (get<2>(*iter)[1] == '1')  // gsv == "01000000" || gsv == "01100000"
                                get<2>(newGce) = "01100000";
                            hasMoveBack = true;
                        } else {
                            if (!c.linkInterval[linkId].contains(newGce))
                                c.linkInterval[linkId].emplace(newGce);
                            auto tmp = std::make_tuple(send, send + transmitDelay, streamId);
                            if (!(c.linkIntervalDuplex[linkId].contains(newGce) && c.linkIntervalDuplex[linkId][newGce].contains(tmp)))
                                c.linkIntervalDuplex[linkId][newGce].emplace(send, send + transmitDelay, streamId);
                            break;
                        }
                    }

                    c.p5TrafficOffsets[streamId][j][i] = std::make_tuple(send, send % c.linkHyperperiod[linkId],
                                                                         transmitDelay);
                    if (i + 1 == route.size()) {
                        send = get<0>(c.p5TrafficOffsets[streamId][j][i - 1]) +
                               get<2>(c.p5TrafficOffsets[streamId][j][i - 1]) +
                               route[i - 1]->getPropSpeed() * route[i - 1]->getLen();
                        c.p5TrafficOffsets[streamId][j][i + 1] = std::make_tuple(send, 0, 0);
                    }
                }
            }
        }
    }
    /* merge the neighboured intervals*/
    c.totalGcl = 0;
    c.longestGcl = 0;
    for (auto &[linkId, intervals]: c.linkInterval) {
        double len = intervals.size() * 2 + 1;
        c.linkGclSize[linkId] = len;
        c.totalGcl += len;
        if (c.longestGcl == 0)
            c.longestGcl = len;
        else
            c.longestGcl = std::max(c.longestGcl, len);
    }
    /* get jitter of p5 traffic */
    for (auto &[streamId, sendIdxs]: c.p5TrafficOffsets) {
        vector<sched_time_t> e2e(sendIdxs.size(), 0);
        for (auto &[sendIdx, hop]: sendIdxs) {
            sched_time_t start = get<0>(hop[0]);
            sched_time_t end = get<0>(hop[hop.size() - 1]);
            e2e[sendIdx] = end - start;
        }
        double totalDiff = 0;
        for (size_t i = 0; i < e2e.size(); ++i) {
            if (i > 0)
                totalDiff += std::abs(e2e[i] - e2e[i - 1]);
        }
        if (totalDiff == 0)
            c.cachedStreamJitter[streamId] = 0;
        c.cachedStreamJitter[streamId] = totalDiff / (e2e.size() - 1);
        c.p5E2e[streamId] = e2e;
    }
    c.maxJitter = std::make_pair(0, -1);
    for (const auto &item: c.cachedStreamJitter) {
        if (item.second > c.maxJitter.second)
            c.maxJitter = item;
    }
    return true;
}

group_id_t MoGaSolver::getGrpIdOfStream(stream_id_t streamId, MyMiddleCost &c){
    for (const auto &[groupId, streamsInGroup]: c.groupStream) {
        for (const auto &_streamId: streamsInGroup) {
            if (_streamId == streamId)
                return groupId;
        }
    }
    return 0;
}

void MoGaSolver::init_genes(TtStreams &p, const std::function<double(void)> &rnd01) {
    p.offsets.assign(input->streams.size(), 0);
    p.routes.assign(input->streams.size(), 0);
    p.upperBounds.assign(input->streams.size(), 0);
    /* set route index */
    for (const auto &[streamId, pos]: input->streamsId) {
        if (input->streams[pos]->getRoutes().size() == 1)
            p.routes[pos] = 0;
        else {
            do {
                p.routes[pos] = input->streams[pos]->getRoutes().size() * rnd01();
            } while (input->streams[pos]->getRoutes()[p.routes[pos]]->getE2E()
                     > input->streams[pos]->getDeliveryGuarantee()->getLowerVal());
        }
    }
    groupStreamsInit(p.routes, p.groupStream);
    for (const auto &[groupId, streamsInGroup]: p.groupStream) {
        p.offsets[input->getStreamPos(streamsInGroup[0])] = 0;
        for (size_t i = 1; i < streamsInGroup.size(); ++i) {
            stream_id_t streamId = streamsInGroup[i];
            const auto &stream = input->getStream(streamId);
            int pos = input->getStreamPos(streamId);
            sched_time_t upperBound = stream->getPeriod()
                                      - stream->getSrc()->getDpr()
                                      - stream->getSrcTransmitDelay();
            if (stream->getPcp() == P6) {
                sched_time_t tmp = stream->getDeliveryGuarantee()->getLowerVal()
                                   - stream->getRoutes()[p.routes[input->getStreamPos(streamId)]]->getE2E();
                upperBound = std::min(upperBound, tmp);
                p.offsets[pos] = upperBound * rnd01();
                p.upperBounds[pos] = upperBound;
            } else if (stream->getPcp() == P5) {
                p.upperBounds[pos] = upperBound;
                do {
                    p.offsets[pos] = upperBound * rnd01();
                } while (p.offsets[pos]
                         + stream->getRoutes()[p.routes[pos]]->getE2E()
                         > stream->getDeliveryGuarantee()->getLowerVal());
            }
        }
    }
    p.useNoWait = flagUseNoWait;
}

bool MoGaSolver::eval_solution(const TtStreams &p, MyMiddleCost &c) {
    c.groupStream = p.groupStream;
    groupStreamsEval(p.routes, c);
    setEachHopStartTime(p, c);
    setLinkHyperperiod(p, c);
    setLinkStreamInterval(p, c);
    std::pair<stream_id_t, sched_time_t> max_ddl;
    std::pair<stream_id_t, sched_time_t> max_e2e;
    if (!checkDDLorE2E(c, p.useNoWait, max_ddl, max_e2e))
        return false;
    if (p.useNoWait) {
        if (!checkCollisionWithStream(p, c))
            return false;
        c.totalGcl = 0;
        c.longestGcl = 0;
        c.totalCache = 0;
        for (auto &[linkId, intervals]: c.linkInterval) {
            double len = intervals.size() * 2 + 1;
            c.linkGclSize[linkId] = len;
            c.totalGcl += len;
            if (c.longestGcl == 0)
                c.longestGcl = len;
            else
                c.longestGcl = std::max(c.longestGcl, len);
        }
    } else {
        if (!checkCollisionWithInterval(p, c))
            return false;
        if (!scheduleP5(p, c))
            return false;
    }
    c.groupSize = 1.0 / static_cast<double>(c.groupStream.size());
    return true;
}

double MoGaSolver::get_shrink_scale(int n_generation, const std::function<double(void)> &rnd01) {
    double scale = (n_generation <= 5 ? 1.0 : 1.0 / sqrt(n_generation - 5 + 1));
    if (rnd01() < 0.4)
        scale *= scale;
    else if (rnd01() < 0.1)
        scale = 1.0;
    return scale;
}

TtStreams MoGaSolver::mutate(
        const TtStreams &X_base,
        const std::function<double(void)> &rnd01,
        double shrink_scale) {
    TtStreams X_new;
    const double mu = 0.2 * shrink_scale; // mutation radius (adjustable)
    X_new = X_base;
    for (const auto &[streamId, pos]: input->streamsId) {
        const auto &stream = input->streams[pos];
        if (stream->getRoutes().size() != 1) {
            do {
                X_new.routes[pos] += stream->getRoutes().size() * rnd01();
                X_new.routes[pos] %= stream->getRoutes().size();
            } while (stream->getRoutes()[X_new.routes[pos]]->getE2E() > stream->getDeliveryGuarantee()->getLowerVal());
        } else {
            X_new.routes[pos] = 0;
        }
    }
    groupStreamsInit(X_new.routes, X_new.groupStream);
    for (const auto &[groupId, streamsInGroup]: X_base.groupStream) {
        X_new.offsets[input->getStreamPos(streamsInGroup[0])] = 0;
        for (size_t i = 1; i < streamsInGroup.size(); ++i) {
            stream_id_t streamId = streamsInGroup[i];
            const auto &stream = input->getStream(streamId);
            int pos = input->getStreamPos(streamId);
            sched_time_t upperBound = stream->getPeriod()
                                      - stream->getSrc()->getDpr()
                                      - stream->getSrcTransmitDelay();
            if (stream->getPcp() == P6) {
                sched_time_t tmp = stream->getDeliveryGuarantee()->getLowerVal()
                                   - stream->getRoutes()[X_new.routes[pos]]->getE2E();
                upperBound = std::min(upperBound, tmp);
                X_new.offsets[pos] = upperBound * rnd01();
//                X_new.upperBounds[pos] = upperBound;
            } else if (stream->getPcp() == P5) {
//                X_new.upperBounds[pos] = upperBound;
            }
            double offset = X_new.offsets[pos] / pow(10, 6);
            if (X_new.offsets[pos] >= upperBound) {
                X_new.offsets[pos] = upperBound * rnd01();
            } else {
                while (true) {
                    double tmp = mu * (rnd01() - rnd01());
                    offset += tmp;
                    offset *= pow(10, 6);
                    if (offset > 0 && (offset < upperBound))
                        break;
                    else
                        offset = X_new.offsets[pos] / pow(10, 6);
                }
            }
            X_new.offsets[pos] = offset;
        }
    }
    return X_new;
}

TtStreams MoGaSolver::crossover(const TtStreams &X1, const TtStreams &X2, const std::function<double(void)> &rnd01) {
    TtStreams X_new;
    if (X1.groupStream.size() > X2.groupStream.size())
        X_new = X1;
    else
        X_new = X2;
    double rRoute = rnd01();
    double rOffset = rnd01();
    for (const auto &[streamId, pos]: input->streamsId) {
        /* generate a route index for offspring */
        const auto &stream = input->streams[pos];
        route_t route = 0;
        if (stream->getRoutes().size() != 1) {
            while (true) {
                double t = rRoute * X1.routes[pos] + (1.0 - rRoute) * X2.routes[pos];
                route_t route = t;
                if (stream->getRoutes()[route]->getE2E() >
                    stream->getDeliveryGuarantee()->getLowerVal()) {
                    rRoute = rnd01();
                } else if (X_new.routes[pos] < stream->getRoutes().size()) {
                    X_new.routes[pos] = route;
                    break;
                }
            }
        } else {
            X_new.routes[pos] = 0;
        }
    }
    groupStreamsInit(X_new.routes, X_new.groupStream);
    for (const auto &[groupId, streamsInGroup]: X_new.groupStream) {
        X_new.offsets[input->getStreamPos(streamsInGroup[0])] = 0;
        for (size_t i = 1; i < streamsInGroup.size(); ++i) {
            stream_id_t streamId = streamsInGroup[i];
            const auto &stream = input->getStream(streamId);
            int pos = input->getStreamPos(streamId);
            sched_time_t upperBound = stream->getPeriod()
                                      - stream->getSrc()->getDpr()
                                      - stream->getSrcTransmitDelay();
            if (stream->getPcp() == P6) {
                sched_time_t tmp = stream->getDeliveryGuarantee()->getLowerVal()
                                   - stream->getRoutes()[X_new.routes[pos]]->getE2E();
                upperBound = std::min(upperBound, tmp);
//                X_new.upperBounds[pos] = upperBound;
            } else if (stream->getPcp() == P5) {
//                X_new.upperBounds[pos] = upperBound;
            }

            double offset = 0;
            while (true) {
                if (X1.offsets[pos] > upperBound && X2.offsets[pos] > upperBound)
                    offset = rOffset * X1.offsets[pos] + rOffset * X2.offsets[pos];
                else
                    offset = rOffset * X1.offsets[pos] + (1.0 - rOffset) * X2.offsets[pos];
                if (offset < upperBound) {
                    X_new.offsets[pos] = offset;
                    break;
                } else {
                    rOffset = rnd01();
                }
            }
        }
    }
    return X_new;
}

vector<double> MoGaSolver::calculate_MO_objectives(const GA_Type::thisChromosomeType &X) {
    return {
            X.middle_costs.longestGcl,
            X.middle_costs.maxJitter.second,
            X.middle_costs.groupSize,
            X.middle_costs.totalCache
    };
}

void MoGaSolver::MO_report_generation(
        int generation_number,
        const EA::GenerationType<TtStreams, MyMiddleCost> &last_generation,
        const vector<unsigned int> &pareto_front) {
    (void) last_generation;
    std::cout << "Generation [" << generation_number << "], ";
    std::cout << "Pareto-Front {";
    for (unsigned int i = 0; i < pareto_front.size(); i++) {
        std::cout << (i > 0 ? "," : "");
        std::cout << pareto_front[i];
    }
    std::cout << "}" << std::endl;
}

void MoGaSolver::save_results(GA_Type &ga_obj, const std::string &path, int32_t runId, double costTime) {
    vector<unsigned int> paretoFrontIndices = ga_obj.last_generation.fronts[0];
    std::string resultSummaryPath = path;

    size_t first = path.find('/') + 1;
    size_t second = path.find('/', first);
    std::string topology = path.substr(first, second - first);

    resultSummaryPath.append(topology + "_" + std::to_string(input->streams.size()) + "_result_summary.txt");
    std::fstream resultSummary(resultSummaryPath, std::fstream::out | std::fstream::app);
    spdlog::get("console")->info("file: {}, {}", resultSummaryPath, strerror(errno));
//    std::ofstream out_file_offset_route(result_offset_route);
    if (runId == 0) {
        resultSummary << "------+---------+--------------------+--------------------+--------------------+--------------------+--------------------+--------------------" << std::endl;
        resultSummary << std::setw(6) << "run_id"
                      << "|" << std::setw(9) << "pareto_id"
                      << "|" << std::setw(20) << "jitter"
                      << "|" << std::setw(20) << "total_cache"
                      << "|" << std::setw(20) << "total_gcl"
                      << "|" << std::setw(20) << "longest_gcl"
                      << "|" << std::setw(20) << "group_size"
                      << "|" << std::setw(20) << "cost_time"<< std::endl;
        resultSummary << "------|---------|--------------------|--------------------|--------------------+--------------------+--------------------+--------------------" << std::endl;
    }

    for (unsigned int i: paretoFrontIndices) {
        /* set flow offset and route index */
        auto &X = ga_obj.last_generation.chromosomes[i];
        resultSummary << std::setw(6) << runId
                      << "|" << std::setw(9) << i
                      << "|" << std::setw(20) << X.middle_costs.maxJitter.second
                      << "|" << std::setw(20) << X.middle_costs.totalCache
                      << "|" << std::setw(20) << X.middle_costs.totalGcl
                      << "|" << std::setw(20) << X.middle_costs.longestGcl
                      << "|" << std::setw(20) << std::round(1.0 / X.middle_costs.groupSize)
                      << "|" << std::setw(20) << costTime << std::endl;

        SaveSolution saveSolution(input, &X.genes, &X.middle_costs);
        saveSolution.saveGCL();

        std::string solutionPath = path + std::to_string(runId) + "/";

        std::filesystem::create_directories(solutionPath);

        std::string routeFile = solutionPath + std::to_string(i) + "_route.xml";
        saveSolution.saveRoute(routeFile);

        std::string gclFileDir = solutionPath + std::to_string(i);
        saveSolution.saveGCL(gclFileDir);

        std::string routeFileName = std::to_string(i) + "_route.xml";
        std::string gclFileName = std::to_string(i) + "_gcl.xml";
        std::string iniFile = solutionPath;
        iniFile.append(std::to_string(i) + "_" + topology + ".ini");
        saveSolution.saveIni(routeFileName, gclFileName, iniFile, topology, i);

        saveSolution.saveScheduleAndMiddleCost(solutionPath, i);
//        std::string event_file = OUT_LOCATION_WAIT;
//        event_file.append("/" + std::to_string(i) + "_event.txt");
//        saveSolution.saveEvent(X.genes, X.middle_costs, event_file);



    }
    resultSummary.close();
//    out_file_offset_route.close();
}
