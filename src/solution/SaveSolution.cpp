//
// Created by faniche on 23-1-4.
//

#include "SaveSolution.h"
#include "../components/node/Switch.h"
#include "../components/node/EndSystem.h"

SaveSolution::SaveSolution(const std::shared_ptr<Input> &_input,
                           TtStreams *_p,
                           MyMiddleCost *_c) : input(_input) {
    p = _p;
    c = _c;
}

void SaveSolution::saveRoute(const std::string &routeFileLocation) {
    map<node_id_t, vector<link_id_t>> swLinks;
    c->linkFlows.clear();
    for (const auto &[streamId, pos]: input->streamsId) {
        auto const &stream = input->getStream(streamId);
        const auto &route = input->getRouteLinks(streamId, p->routes[pos]);
        for (hop_t i = 0; i < route.size(); ++i) {
            link_id_t linkId = route[i]->getId();
            c->linkFlows[linkId].emplace_back(streamId, i);
        }
    }
    for (const auto &sw: input->swList) {
        for (const auto &[linkId, streamIds]: c->linkFlows) {
            if (input->links[linkId]->getSrcNode() == sw) {
                swLinks[sw->getId()].emplace_back(linkId);
            }
        }
    }
    pugi::xml_document xdoc;
    pugi::xml_node xdec = xdoc.prepend_child(pugi::node_declaration);
    xdec.append_attribute("version").set_value("1.0");
    xdec.append_attribute("encoding").set_value("utf-8");
    pugi::xml_node xdbs = xdoc.append_child("filteringDatabases");

    /* iterate all switches */
    for (const auto &[nodeId, linksId]: swLinks) {
        auto sw = input->nodeIdMap[nodeId];
        pugi::xml_node xdb = xdbs.append_child("filteringDatabase");
        pugi::xml_attribute xsw_id = xdb.append_attribute("id");
        xsw_id.set_value(sw->getName().c_str());
        pugi::xml_node xstatic = xdb.append_child("static");
        pugi::xml_node xforward = xstatic.append_child("forward");
        /* iterate a switch's ports */
        auto ports = std::dynamic_pointer_cast<Switch>(sw)->getPorts();
        for (auto &linkId: linksId) {
            for (const auto &[streamId, hop]: c->linkFlows[linkId]) {
                pugi::xml_node xmulticast_addr = xforward.append_child("multicastAddress");
                pugi::xml_attribute xmac = xmulticast_addr.append_attribute("macAddress");
                std::string mac = "255-0-00-00-00-" + std::to_string(streamId);
                if (streamId < 10) {
                    mac.insert(mac.length() - 1, "0");
                }
                xmac.set_value(mac.c_str());
                pugi::xml_attribute xports = xmulticast_addr.append_attribute("ports");
                xports.set_value(input->links[linkId]->getSrcPort()->getId());
            }
        }
    }
    xdoc.save_file(routeFileLocation.c_str());
}

void SaveSolution::saveGCL() {
    for (const auto &link: input->links) {
        link.second->clearGateControlEntry();
    }
    /* map<link_id_t, set<std::tuple<sched_time_t, sched_time_t, bool>>> linkInterval; */
    for (const auto &[linkId, intervals]: c->linkInterval) {
//        if (input->nodeIdMap[linkId.first]->getNodeType() == END_SYSTEM)    continue;
        for (auto &interval: intervals) {
            input->links[linkId]->getSrcPort()->addGateControlEntry(std::make_shared<GateControlEntry>(interval));
        }
    }
    for (auto &link: input->links)
        link.second->sortGCL();
}

void SaveSolution::saveGCL(const std::string &gclFileLocation) {
    saveSwPortSchedule(gclFileLocation);
    saveEsSchedule(gclFileLocation);
}

void SaveSolution::saveSwPortSchedule(const std::string &schedFileLocation) {
    pugi::xml_document xdoc;
    pugi::xml_node xdec = xdoc.prepend_child(pugi::node_declaration);
    xdec.append_attribute("version").set_value("1.0");
    xdec.append_attribute("encoding").set_value("utf-8");
    pugi::xml_node xschedules = xdoc.append_child("schedules");
    pugi::xml_node xhyperperiod = xschedules.append_child("defaultcycle");
    std::string hyp_str = std::to_string(input->hyperPeriod) + "ns";
    xhyperperiod.append_child(pugi::node_pcdata).set_value(hyp_str.c_str());
    for (auto &sw: input->swList) {
        pugi::xml_node xswitch = xschedules.append_child("switch");
        pugi::xml_attribute xname = xswitch.append_attribute("name");
        xname.set_value(sw->getName().c_str());
        auto ports = std::dynamic_pointer_cast<Switch>(sw)->getPorts();
        for (int i = 0; i < ports.size(); ++i) {
            for (const auto &[linkId, link]: input->links) {
                if (linkId.first == sw->getId()
                    && link->getSrcPort() == ports[i]
                    && !ports[i]->getGateControlList().empty()) {
                    if (!c->linkInterval.contains(linkId))
                        spdlog::get("console")->error("{}:{}: some error", __FILE__, __LINE__);
                    pugi::xml_node xport = xswitch.append_child("port");
                    pugi::xml_attribute xportId = xport.append_attribute("id");
                    xportId.set_value(i);
                    pugi::xml_node xschedule = xport.append_child("schedule");
                    pugi::xml_attribute xcycle_time = xschedule.append_attribute("cycleTime");
                    std::string cycTimeStr = std::to_string(c->linkHyperperiod[linkId]) + "ns";
                    xcycle_time.set_value(cycTimeStr.c_str());
                    sched_time_t cur = 0;
                    for (const auto &[start, end, gsv]: c->linkInterval[linkId]) {
                        if (gsv == "01100000")
                            spdlog::get("console")->debug("{}:{} P5 and P6 open at same time", __FILE__, __LINE__);
                        sched_time_t gap = start - cur;
                        /* gap */
                        GateControlEntry gateControlEntry;
                        pugi::xml_node xallOpenEntry = xschedule.append_child("entry");
                        std::string wndLenStr = std::to_string(gap) + "ns";
                        pugi::xml_node xallOpenLen = xallOpenEntry.append_child("length");
                        xallOpenLen.append_child(pugi::node_pcdata).set_value(wndLenStr.c_str());
                        pugi::xml_node xappOpenBitvec = xallOpenEntry.append_child("bitvector");
                        xappOpenBitvec.append_child(pugi::node_pcdata).set_value(gateControlEntry.toBitStr().c_str());
                        /* gce */
                        pugi::xml_node xentry = xschedule.append_child("entry");
                        pugi::xml_node xlen = xentry.append_child("length");
                        wndLenStr = std::to_string(end - start) + "ns";
                        xlen.append_child(pugi::node_pcdata).set_value(wndLenStr.c_str());
                        pugi::xml_node xbitvec = xentry.append_child("bitvector");
                        xbitvec.append_child(pugi::node_pcdata).set_value(gsv.c_str());

                        cur = end;
                    }
                    if (cur < c->linkHyperperiod[linkId]) {
                        sched_time_t gap = c->linkHyperperiod[linkId] - cur;
                        GateControlEntry gateControlEntry;
                        pugi::xml_node xentry = xschedule.append_child("entry");
                        pugi::xml_node xlen = xentry.append_child("length");
                        std::string wndLenStr = std::to_string(gap) + "ns";
                        xlen.append_child(pugi::node_pcdata).set_value(wndLenStr.c_str());
                        pugi::xml_node xbitvec = xentry.append_child("bitvector");
                        xbitvec.append_child(pugi::node_pcdata).set_value(gateControlEntry.toBitStr().c_str());
                    }
                }
            }
        }
    }
    std::string switch_gcl_file = schedFileLocation + "_gcl.xml";
    xdoc.save_file(switch_gcl_file.c_str());
}

void SaveSolution::saveEsSchedule(const std::string &schedFileLocation) {
    map<node_id_t, vector<std::shared_ptr<Stream>>> esStreamMap;
    /* Group the flow with src */
    for (auto &stream: input->streams) {
        node_id_t nodeId = stream->getSrc()->getId();
        esStreamMap[nodeId].push_back(stream);
    }
    /* Sort the flow with PCP */
    for (auto &[src, streams]: esStreamMap) {
        std::sort(streams.begin(), streams.end(), [](std::shared_ptr<Stream> &a, std::shared_ptr<Stream> &b) {
            return a->getPcp() < b->getPcp();
        });
    }

    for (auto &[srcId, esStreams]: esStreamMap) {
        const auto &es = input->nodeIdMap[srcId];
        pugi::xml_document xesdoc;
        pugi::xml_node xesdec = xesdoc.prepend_child(pugi::node_declaration);
        xesdec.append_attribute("version").set_value("1.0");
        xesdec.append_attribute("encoding").set_value("utf-8");
        pugi::xml_node xes_schedules = xesdoc.append_child("schedules");
        pugi::xml_node xes_cycle = xes_schedules.append_child("defaultcycle");
        std::string cycStr;
        for (const auto &[linkId, link]: input->links) {
            if (link->getSrcNode() == es) {
                cycStr = std::to_string(input->hyperPeriod) + "ns";
                break;
            }
        }
        xes_cycle.append_child(pugi::node_pcdata).set_value(cycStr.c_str());
        pugi::xml_node xhost = xes_schedules.append_child("host");
        pugi::xml_attribute xname = xhost.append_attribute("name");
        xname.set_value(es->getName().c_str());
        pugi::xml_node xcycle = xhost.append_child("cycle");
        xcycle.append_child(pugi::node_pcdata).set_value(cycStr.c_str());
        for (const auto &stream: esStreams) {
            pugi::xml_node xentry = xhost.append_child("entry");
            pugi::xml_node xstart = xentry.append_child("start");

            std::string start_str = std::to_string(p->offsets[input->getStreamPos(stream->getId())]) + "ns";
            xstart.append_child(pugi::node_pcdata).set_value(start_str.c_str());
            pugi::xml_node xqueue = xentry.append_child("queue");
            xqueue.append_child(pugi::node_pcdata).set_value(
                    std::to_string(stream->getPcp()).c_str());
            pugi::xml_node xdest = xentry.append_child("dest");
            std::string dest_str = "255:0:00:00:00:" + std::to_string(stream->getId());
            if (stream->getId() < 10) {
                dest_str.insert(dest_str.length() - 1, "0");
            }
            xdest.append_child(pugi::node_pcdata).set_value(dest_str.c_str());
            pugi::xml_node xsize = xentry.append_child("size");
            std::string size_str = std::to_string(stream->getLength() - HEADER_LEN) + "B";
            xsize.append_child(pugi::node_pcdata).set_value(size_str.c_str());
            pugi::xml_node xflowId = xentry.append_child("flowId");
            xflowId.append_child(pugi::node_pcdata).set_value(std::to_string(stream->getId()).c_str());
            pugi::xml_node xflowPeriod = xentry.append_child("period");
            std::string period_str = std::to_string(stream->getPeriod()) + "ns";
            xflowPeriod.append_child(pugi::node_pcdata).set_value(period_str.c_str());
        }
        std::string es_traffic_gen = schedFileLocation + "_" + es->getName() + ".xml";
        xesdoc.save_file(es_traffic_gen.c_str());
    }
}

void SaveSolution::saveIni(const std::string &routeFileName,
                           const std::string &gclFileName,
                           const std::string &iniFile,
                           const std::string &topology,
                           size_t solution_id) {
    std::ofstream output(iniFile);
    output << "[General]\n"
              "network = " << topology << "\n"
                                          "\n"
                                          "record-eventlog = false \n"
                                          "debug-on-errors = true\n"
                                          "result-dir = result/" << topology << "\n"
                                                                                "sim-time-limit = "
           << std::to_string(input->hyperPeriod / 1000000) << "ms\n"
                                                              "\n"
                                                              "# debug\n"
                                                              "**.displayAddresses = false\n"
                                                              "**.verbose = false" << std::endl;
    output << "# MAC Addresses" << std::endl;
    for (const auto &es: input->esList) {
        node_id_t nodeId = es->getId();
        output << "**." << es->getName()
               << R"(.eth.address = "00-00-00-00-00-)" << std::setw(2) << std::setfill('0') << std::hex << nodeId
               << "\"" << std::endl;
    }
    output << R"(**.frequency = 1GHz)" << std::endl;
    output << "# Switches" << std::endl;

    output << R"(**.switch*.processingDelay.delay = 20000ns)" << std::endl;

    output << R"(**.filteringDatabase.database = xmldoc("xml/)" << topology << "/" << routeFileName
           << R"(", "/filteringDatabases/"))"
           << std::endl;
    for (const auto &sw: input->swList) {
        auto ports = std::dynamic_pointer_cast<Switch>(sw)->getPorts();
        for (int i = 0; i < ports.size(); ++i) {
            if (!ports[i]->getGateControlList().empty()) {
                output << "**." << sw->getName() << ".eth[" << std::to_string(i)
                       << R"(].queue.gateController.initialSchedule = xmldoc("xml/)" << topology << "/" << gclFileName;
                output << R"(", "/schedules/switch[@name=')" << sw->getName() << R"(']/port[@id=')"
                       << std::to_string(i) << R"(']/schedule"))" << std::endl;
            }
        }
    }
    output << R"(**.gateController.enableHoldAndRelease = true)" << std::endl;
    for (int i = 0; i < 8; ++i) {
        output << R"(**.switch*.eth[*].queuing.tsAlgorithms[)" << std::to_string(i)
               << R"(].typename = "StrictPriority")" << std::endl;
    }
//        output << R"(**.queues[*].bufferCapacity = 363360b)" << std::endl;
//        output << R"(**.sw*.eth[*].mac.enablePreemptingFrames = false)" << std::endl;

    for (auto &es: input->esList) {
        for (auto &stream: input->streams) {
            if (stream->getSrc() == es) {
                output << "**." << es->getName() << R"(.trafGenSchedApp.initialSchedule = xmldoc("xml/)" << topology
                       << "/"
                       << std::to_string(solution_id) << "_" << es->getName() << R"(.xml"))" << std::endl;
                break;
            }
        }
    }
}

void SaveSolution::saveScheduleAndMiddleCost(const std::string &middleCostPath, size_t solutionId) {
    std::string path = middleCostPath;
    path.append(std::to_string(solutionId) + "_mid_cost_and_schedule.txt");
    std::fstream midCostAndSchedule(path, std::fstream::out);
    midCostAndSchedule << "[schedule]" << std::endl;
    midCostAndSchedule
            << "----------+---------------+---------------+---------------------+---------------+--------------------------------------------------------------------------------"
            << std::endl;
    midCostAndSchedule << std::right << std::setw(10) << "stream_id" << "|"
                       << std::right << std::setw(15) << "period" << "|"
                       << std::right << std::setw(15) << "offset" << "|"
                       << std::right << std::setw(21) << "e2e/ddl" << "|"
                       << std::right << std::setw(15) << "jitter" << "|"
                       << std::left << std::setw(80) << "    route" << std::endl;
    midCostAndSchedule
            << "----------+---------------+---------------+---------------------+---------------+--------------------------------------------------------------------------------"
            << std::endl;
    for (const auto &[streamId, pos]: input->streamsId) {
        auto stream = input->getStream(streamId);
        midCostAndSchedule << std::right << std::setw(10) << streamId << "|"
                           << std::right << std::setw(15) << stream->getPeriod() << "|"
                           << std::right << std::setw(15) << p->offsets[pos] << "|";
        if (!p->useNoWait) {
            if (stream->getPcp() == P6) {
                midCostAndSchedule << std::right << std::setw(21) << c->ddlOrE2E[streamId] << "|";
                midCostAndSchedule << std::right << std::setw(16) << "0" << "|";
            } else if (stream->getPcp() == P5) {
                midCostAndSchedule << std::right << std::setw(10) << *min_element(c->p5E2e[streamId].begin(), c->p5E2e[streamId].end()) << "/"
                                   << std::left << std::setw(10) << *max_element(c->p5E2e[streamId].begin(), c->p5E2e[streamId].end()) << "|";
                midCostAndSchedule << std::right << std::setw(15) << c->cachedStreamJitter[streamId]<< "|";
            }
        } else {
            midCostAndSchedule << std::right << std::setw(21) << c->ddlOrE2E[streamId] << "|";
            midCostAndSchedule << std::right << std::setw(16) << "0" << "|";
        }
        midCostAndSchedule << std::left << "    " << stream->getRoutes()[p->routes[pos]]->toString() << std::endl;
    }
    midCostAndSchedule
            << "----------+---------------+---------------+---------------------+---------------+--------------------------------------------------------------------------------"
            << std::endl;
    /* ========================================================================== */
    midCostAndSchedule << "[hyper_period]" << std::endl;
    midCostAndSchedule << "------------------------------+----------+--------------------" << std::endl;
    midCostAndSchedule << std::left << std::setw(30) << "src --> dest" << "|"
                       << std::right << std::setw(10) << "gcl_length" << "|"
                       << std::right << std::setw(16) << "BPC" << std::endl;
    midCostAndSchedule << "------------------------------+----------+--------------------" << std::endl;
    for (const auto &[linkId, hyperperiod]: c->linkHyperperiod) {
        midCostAndSchedule << std::left << std::setw(30) << input->getLinkStr(linkId) << "|"
                           << std::right << std::setw(10) << c->linkGclSize[linkId] << "|"
                           << std::right << std::setw(16) << hyperperiod << std::endl;
    }
    midCostAndSchedule << "------------------------------+----------+--------------------" << std::endl;
    /* ========================================================================== */
    midCostAndSchedule << "[group]" << std::endl;
    midCostAndSchedule
            << "----------+-----------------+--------------------------------------------------------------------------------"
            << std::endl;
    midCostAndSchedule << std::right << std::setw(10) << "group_id" << "|"
                       << std::right << std::setw(17) << "group_hyperperiod" << "|"
                       << std::left << std::setw(80) << "streams_id" << std::endl;
    midCostAndSchedule
            << "----------+-----------------+--------------------------------------------------------------------------------"
            << std::endl;
    for (const auto &[groupId, hyperperiod]: c->groupHyperPeriod) {
        midCostAndSchedule << std::right << std::setw(10) << groupId << "|"
                           << std::right << std::setw(17) << hyperperiod << "|";
        midCostAndSchedule << std::left;
        for (auto &streamId: c->groupStream[groupId])
            midCostAndSchedule << streamId << ", ";
        midCostAndSchedule << std::endl;
    }
    midCostAndSchedule
            << "----------+-----------------+--------------------------------------------------------------------------------"
            << std::endl;

    midCostAndSchedule << "[gce_reuse]" << std::endl;
    midCostAndSchedule << "merge_count = " << c->mergeCount << std::endl;
    for (const auto &[linkId, reuseIntervals]: c->linkIntervalDuplex) {
        for (const auto &[intervals, reusedStreamsInfo]: reuseIntervals) {
            if (reusedStreamsInfo.size() > 1) {
                midCostAndSchedule << input->getLinkStr(linkId) << std::endl;
                midCostAndSchedule << "\t<" << get<0>(intervals) << ", " << get<1>(intervals) << ", "
                                   << get<2>(intervals) << ">" << std::endl;
                for (auto &[start, end, streamId]: reusedStreamsInfo) {
                    midCostAndSchedule << "\t\t<" << start << ", " << end << ", " << streamId << ">" << std::endl;
                }
            }
        }
    }
    midCostAndSchedule.close();
}
