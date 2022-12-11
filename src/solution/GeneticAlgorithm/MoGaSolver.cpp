//
// Created by faniche on 22-11-14.
//

#include "MoGaSolver.h"
#include "../../components/node/EndSystem.h"

MoGaSolver::MoGaSolver(std::shared_ptr<Input> _input,
                       bool _debug = false,
                       int _generations = 100) : input(std::move(_input)), debug(_debug), generations(_generations) {
    std::sort(input->streams.begin(), input->streams.end(),
              [](const std::shared_ptr<Stream> &a, const std::shared_ptr<Stream> &b) {
                  return a->getPeriod() < b->getPeriod() || a->getLength() < b->getLength() ||
                         a->getPcp() < b->getPcp();
              });
    for (int i = 0; i < input->streams.size(); ++i)
        input->streamsId[input->streams[i]->getId()] = i;
}

void MoGaSolver::vSolve() {
    GA_Type ga_obj;
    ga_obj.problem_mode = EA::GA_MODE::NSGA_III;
    ga_obj.multi_threading = !debug;
    ga_obj.dynamic_threading = !debug;
    ga_obj.verbose = debug;
    ga_obj.population = 100;
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

void MoGaSolver::setEachHopStartTime(const TtStreams &p, MyMiddleCost &c) {
    for (const auto &[streamId, pos]: input->streamsId) {
        const auto &stream = input->streams[pos];
        auto routeLinks = stream->getRoutes()[p.routes[streamId]]->getLinks();
        sched_time_t accDelay = p.offsets[streamId];
        for (int i = 0; i < routeLinks.size(); ++i) {
            accDelay += routeLinks[i]->getSrcNode()->getDpr();
            c.trafficOffsets[streamId][i] = accDelay;
            accDelay += (sched_time_t) (
                    stream->getLength() * routeLinks[i]->getSrcPort()->getTransmitSpd()
                    + routeLinks[i]->getPropSpeed() * routeLinks[i]->getLen()
            );
        }
        c.trafficOffsets[streamId][routeLinks.size()] = accDelay;
        if (stream->getPcp() == P5)
            c.ddlOrE2E[streamId] = accDelay - p.offsets[streamId];
        else if (stream->getPcp() == P6)
            c.ddlOrE2E[streamId] = accDelay;
    }
}

bool MoGaSolver::checkDDLorE2E(MyMiddleCost &c,
                               std::pair<stream_id_t, sched_time_t> &max_ddl,
                               std::pair<stream_id_t, sched_time_t> &max_e2e) {
    for (const auto &[streamId, pos]: input->streamsId) {
        const auto &stream = input->streams[pos];
        if (c.ddlOrE2E[streamId] >= stream->getDeliveryGuarantee()->getLowerVal())
            return false;
        if (stream->getPcp() == P5) {
            if (max_e2e.second < c.ddlOrE2E[streamId])
                max_e2e = std::make_pair(streamId, c.ddlOrE2E[streamId]);
        } else if (stream->getPcp() == P6)
            if (max_ddl.second < c.ddlOrE2E[streamId])
                max_ddl = std::make_pair(streamId, c.ddlOrE2E[streamId]);
    }
    return true;
}

void MoGaSolver::setLinkHyperperiod(const std::vector<route_t> &routes,
                                    map<link_id_t, sched_time_t> &linkHyperperiod) {
    if (!linkHyperperiod.empty()) {
        linkHyperperiod.clear();
    }
    for (const auto &[streamId, pos]: input->streamsId) {
        const auto &stream = input->streams[pos];
        for (const auto &link: stream->getRoutes()[routes[streamId]]->getLinks()) {
            if (linkHyperperiod.contains(link->getId()))
                linkHyperperiod[link->getId()] = std::lcm(linkHyperperiod[link->getId()], stream->getPeriod());
            else
                linkHyperperiod[link->getId()] = stream->getPeriod();
        }
    }
}

void MoGaSolver::groupStreams(const std::vector<route_t> &routes,
                              MyMiddleCost &c) {
    for (auto const &streamId: input->streamsGroupByPcp[P5]) {
        bool couldCache = true;
        auto const &stream = input->streams[input->streamsId[streamId]];
        auto routeLinks = stream->getRoutes()[routes[streamId]]->getLinks();
        sched_time_t srcHyp = c.linkHyperperiod[routeLinks[0]->getId()];
        for (int i = 1; i < routeLinks.size(); ++i) {
            if (c.linkHyperperiod[routeLinks[i]->getId()] < srcHyp
                && (c.linkHyperperiod[routeLinks[i]->getId()] % srcHyp != 0)) {
                couldCache = false;
                break;
            }
        }
        if (couldCache)
            c.cachedFlows.push_back(streamId);
        else
            c.uncachedFlows.push_back(streamId);
    }
    for (auto const &streamId: c.uncachedFlows) {
        auto const &routeLinks = input->streams[input->streamsId[streamId]]->getRoutes()[routes[streamId]]->getLinks();
        hop_t i = 0;
        for (const auto &link: routeLinks) {
            c.linkFlows[link->getId()].emplace_back(streamId, i++);
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
MoGaSolver::compressP6Help(sched_time_t siPeriod, sched_time_t siMid, sched_time_t siLen,
                           sched_time_t sjPeriod, sched_time_t sjMid, sched_time_t sjLen,
                           bool &couldMoveRight,
                           bool &couldMoveLeft) {
    sched_time_t hpij = std::lcm(siPeriod, sjPeriod);
    sched_time_t dist = (siLen + sjLen) / 2 + IFG_TIME;
    sched_time_t left = INT64_MIN;
    sched_time_t right = INT64_MAX;
    sched_time_t _sjMid = sjMid % sjPeriod;
    for (int k = 0; k < hpij / siPeriod; ++k) {
        /*           d = (sjMid + k * sjPeriod % siPeriod  - fi_mid*/
        sched_time_t d = (siMid + k * siPeriod) % sjPeriod - _sjMid;
        if (std::abs(d) < dist) {
            SPDLOG_LOGGER_TRACE(spdlog::get("console"), "dist = {}, d = {}", dist, d);
            couldMoveLeft = false;
            couldMoveRight = false;
            return std::make_pair(left, right);
        } else if (d < 0 && couldMoveRight) {
            if (left != INT64_MIN)
                left = (d + dist) > left ? (d + dist) : left;
            else
                left = d + dist;
        } else if (d > 0 && couldMoveLeft) {
            if (right != INT64_MAX)
                right = (d - dist) < right ? (d - dist) : right;
            else
                right = d - dist;
        }
    }
    if (left == INT64_MIN && couldMoveRight)
        couldMoveRight = false;
    if (right == INT64_MAX && couldMoveLeft)
        couldMoveLeft = false;
    return std::make_pair(left, right);
}

void MoGaSolver::compressP6(const TtStreams &p, MyMiddleCost &c) {
    for (const auto &stream: input->streams) {
        sched_time_t moveRight = INT64_MIN;
        sched_time_t moveLeft = INT64_MAX;
        std::vector<std::pair<sched_time_t, sched_time_t>> compressInterval;
        const auto &route = stream->getRoutes()[p.routes[stream->getId()]]->getLinks();
        bool couldMoveRight = true;
        bool couldMoveLeft = true;
        for (hop_t i = 0; i < route.size(); ++i) {
            const auto &link = route[i];
            double transmitSpd = input->links[link->getId()]->getSrcPort()->getTransmitSpd();
            for (const auto &[streamId, hop]: c.linkFlows[link->getId()]) {
                if (streamId == stream->getId()) continue;
                const auto &streamComp = input->streams[streamId];
                auto comp = compressP6Help(stream->getPeriod(),
                                           c.trafficOffsets[stream->getId()][i],
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
                          return a.first > b.first;
                      });
            moveRight = compressInterval[0].first;
        }
        if (couldMoveLeft) {
            std::sort(compressInterval.begin(), compressInterval.end(),
                      [](std::pair<sched_time_t, sched_time_t> a, std::pair<sched_time_t, sched_time_t> b) {
                          return a.second < b.second;
                      });
            moveLeft = compressInterval[0].second;
        }
        if (!couldMoveRight) {
            for (size_t i = 0; i <= route.size(); ++i)
                c.trafficOffsets[stream->getId()][i] += -moveRight;
        } else if (!couldMoveLeft) {
            for (size_t i = 0; i <= route.size(); ++i)
                c.trafficOffsets[stream->getId()][i] += -moveLeft;
        } else {
            sched_time_t move = std::min(std::abs(moveRight), std::abs(moveLeft));
            for (size_t i = 0; i <= route.size(); ++i)
                c.trafficOffsets[stream->getId()][i] += -move;
        }

    }
}

bool MoGaSolver::checkQueueCache(const TtStreams &p, MyMiddleCost &c) {
    for (const auto &streamId: c.cachedFlows) {
        const auto streamI = input->streams[input->streamsId[streamId]];
        const auto route = streamI->getRoutes()[p.routes[streamId]]->getLinks();
        sched_time_t srcHyp = c.linkHyperperiod[route[0]->getId()];
        size_t srcSendTimes = srcHyp / streamI->getPeriod();
        /* Set send time at first hop */
        for (size_t i = 0; i < srcSendTimes; ++i)
            c.p5TrafficOffsets[streamId][0][i] = p.offsets[streamId] + i * streamI->getPeriod();
        /* repeat each hop of flow i */
        for (size_t i = 0; i < route.size(); ++i) {
            const auto &link = route[i];
            size_t count = 0;
            sched_time_t fi_len = static_cast<sched_time_t>(streamI->getLength() *
                                                            link->getSrcPort()->getTransmitSpd());
            sched_time_t dist = fi_len / 2;
            size_t fi_cur_snd_times = c.linkHyperperiod[link->getId()] / streamI->getPeriod();
            for (size_t j = 0; j < fi_cur_snd_times; ++j) {
                if (j >= srcSendTimes) {
                    c.p5TrafficOffsets[streamId][i][j] =
                            c.p5TrafficOffsets[streamId][i][j % srcSendTimes] + j / srcSendTimes * srcHyp;
                } else {
                    c.p5TrafficOffsets[streamId][i][j] += link->getSrcNode()->getDpr();
                }
                sched_time_t fi_start = c.p5TrafficOffsets[streamId][i][j];
                sched_time_t fi_mid = fi_start + fi_len / 2;
                for (auto const &[fj_id, hop_j]: c.linkFlows[link->getId()]) {
                    const auto streamJ = input->streams[input->streamsId[fj_id]];
                    /* check queue cache with isochronous flows */
                    if (streamJ->getPcp() != P6) continue;
                    sched_time_t fj_start = c.trafficOffsets[fj_id][hop_j] % streamJ->getPeriod();
                    int d = static_cast<int>(fi_mid % streamJ->getPeriod() - fj_start);
                    if (std::abs(d) < dist) {
                        SPDLOG_LOGGER_TRACE(spdlog::get("console"),
                                            "NO COLLISION CHECK FAILED, flow[{}] and flow[{}]", flowId,
                                            streamJ.getId());
                        return false;
                    }
                    sched_time_t fj_len = static_cast<int>(streamJ->getLength() * link->getSrcPort()->getTransmitSpd());
                    if (d > 0 && fi_start % streamJ->getPeriod() < (fj_start + fj_len + IFG_TIME)) {
                        if (j > srcSendTimes)
                            return false;
                        if (i > 0 && streamI->getPeriod() % streamJ->getPeriod() != 0)
                            return false;
                        sched_time_t q_delay = fj_start + fj_len + IFG_TIME - fi_start % streamJ->getPeriod();
                        c.totalCache += static_cast<double>(q_delay);
                        c.p5TrafficOffsets[streamId][i][j] += q_delay;
                        count++;
                    }
                }
                if (j < srcSendTimes) {
                    c.p5TrafficOffsets[streamId][i + 1][j] =
                            c.p5TrafficOffsets[streamId][i][j] + fi_len + link->getPropSpeed() * link->getLen();
                }
            }
            count *= fi_cur_snd_times / srcSendTimes;
            if (input->links[link->getId()].get()->getSrcNode()->getNodeType() == SWITCH) {
                c.linkGclSize[link->getId()] =
                        c.linkGclSize[link->getId()] - count;
                if (c.linkGclMergeCount.contains(link->getId()))
                    c.linkGclMergeCount[link->getId()] += count;
                else
                    c.linkGclMergeCount[link->getId()] = count;
            }
        }
    }
    return true;
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
    p.useNoWait = rnd01() <= 0.5;
}

bool MoGaSolver::eval_solution(const TtStreams &p, MyMiddleCost &c) {
    setEachHopStartTime(p, c);
    std::pair<stream_id_t, sched_time_t> max_ddl;
    std::pair<stream_id_t, sched_time_t> max_e2e;
    if (!checkDDLorE2E(c, max_ddl, max_e2e))
        return false;
    if (p.useNoWait) {
        for (const auto &[streamId, pos]: input->streamsId) {
            auto const &stream = input->streams[input->streamsId[streamId]];
            for (int i = 0; i < stream->getRoutes()[p.routes[streamId]]->getLinks().size(); ++i) {
                link_id_t linkId = stream->getRoutes()[p.routes[streamId]]->getLinks()[i]->getId();
                c.linkFlows[linkId].emplace_back(streamId, i);
            }
        }
        if (!checkCollision(c, p.routes))
            return false;
    } else {
        c.uncachedFlows.insert(c.uncachedFlows.end(),
                               input->streamsGroupByPcp[P6].begin(),
                               input->streamsGroupByPcp[P6].end());
        setLinkHyperperiod(p.routes, c.linkHyperperiod);
        groupStreams(p.routes, c);

        if (!checkCollision(c, p.routes))
            return false;

        compressP6(p, c);

    }

    for (auto const &streamId: c.cachedFlows) {
        auto const &stream = input->streams[input->streamsId[streamId]];
        for (int i = 0; i < stream->getRoutes()[p.routes[streamId]]->getLinks().size(); ++i) {
            link_id_t linkId = stream->getRoutes()[p.routes[streamId]]->getLinks()[i]->getId();
            c.linkFlows[linkId].emplace_back(streamId, i);
        }
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

    if (!checkQueueCache(p, c))
        return false;
//
//    if (!checkP5CollisionCachedWithUncached(c, oss))
//        return false;
//
//    if (!checkP5CollisionCached(c, oss))
//        return false;
//
//
//    if (!checkP5E2E(p, c))
//        return false;

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
