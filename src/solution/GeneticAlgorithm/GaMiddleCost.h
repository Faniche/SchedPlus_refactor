//
// Created by faniche on 22-11-28.
//

#ifndef SCHEDPLUS_REFACTOR_GAMIDDLECOST_H
#define SCHEDPLUS_REFACTOR_GAMIDDLECOST_H

#include <vector>
#include <map>
#include <set>
#include <bitset>
#include "../../type.h"

using std::map;     using std::vector;
using std::set;


struct MyMiddleCost {

    /*   flow_id       hop     offset     */
    map<stream_id_t, map<hop_t, sched_time_t>> p6_traffic_offsets;

    map<group_id_t, vector<stream_id_t>> groupStream;

    map<group_id_t, sched_time_t> groupHyperPeriod;
    /*  group_id       stream_id         hop              offset        transmitDelay     */
    map<group_id_t, map<stream_id_t, map<hop_t, std::pair<sched_time_t, sched_time_t>>>> trafficOffsets;
    /*  stream_id        snd_times     hop               start time    module time   trans_delay */
    map<stream_id_t, map<uint64_t, map<hop_t, std::tuple<sched_time_t, sched_time_t, sched_time_t>>>> p5TrafficOffsets;

    map<link_id_t, set<std::tuple<sched_time_t, sched_time_t, std::string>>> linkInterval;
    /*  link_id        interval                                             the_interval_in_hyperperiod */
    map<link_id_t, map<std::tuple<sched_time_t, sched_time_t, std::string>, set<std::tuple<sched_time_t, sched_time_t, stream_id_t>>>> linkIntervalDuplex;

    map<stream_id_t, sched_time_t> ddlOrE2E;

    /*   flow_id     snd_times   e2e  */
    map<stream_id_t, vector<sched_time_t>> p5E2e;

    /*   flow_id  jitter  */
    map<stream_id_t, double> cachedStreamJitter;

    /*  link_id_t                    flow_id     hop */
    map<link_id_t, vector<std::pair<stream_id_t, hop_t>>> linkFlows;

    map<group_id_t, vector<stream_id_t>> cachedFlows;

    map<group_id_t, vector<stream_id_t>> uncachedFlows;

    /*  link_id_t   hyperperiod of link */
    map<link_id_t, sched_time_t> linkHyperperiod;

    map<link_id_t, uint64_t> linkGclSize;

    map<link_id_t, uint64_t> linkGclMergeCount;

    // the minimum variance of generation
    double variance = 0;

    double e2e;

    double ddl;

    int mergeCount;

    double groupSize;

    double totalCache = 0;

    double totalGcl;

    double longestGcl;

    std::pair<stream_id_t, double> maxJitter;
//    double delayQueue = 0;
//
//    double bandwidthUsage = 0;

};
#endif //SCHEDPLUS_REFACTOR_GAMIDDLECOST_H
