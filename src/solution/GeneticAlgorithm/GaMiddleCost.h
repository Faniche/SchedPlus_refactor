//
// Created by faniche on 22-11-28.
//

#ifndef SCHEDPLUS_REFACTOR_GAMIDDLECOST_H
#define SCHEDPLUS_REFACTOR_GAMIDDLECOST_H

#include <vector>
#include <map>
#include "../../type.h"

using std::map;     using std::vector;

struct MyMiddleCost {

    /*   flow_id       hop     offset     */
    map<stream_id_t, map<hop_t, sched_time_t>> p6_traffic_offsets;

    map<group_id_t, vector<stream_id_t>> groupStream;

    map<group_id_t, sched_time_t> groupHyperPeriod;
/*   flow_id       hop     offset     */
    map<stream_id_t, map<hop_t, sched_time_t>> trafficOffsets;
    map<stream_id_t, sched_time_t> ddlOrE2E;
    /*   flow_id     snd_times       hop     offset     */
//    map<uint32_t, map<uint64_t, map<uint8_t, uint64_t>>> p5TrafficOffsets;

    /*   flow_id     hop           snd_times offset     start time    module time */
    map<stream_id_t, map<hop_t, map<uint64_t, std::pair<sched_time_t, sched_time_t>>>> p5TrafficOffsets;

    map<link_id_t, int64_t> reuseGroupIndex;

    /*  linkid         reuse_index  streamId               start time    module time */
    map<link_id_t, map<int64_t, map<stream_id_t, std::pair<sched_time_t, sched_time_t>>>> linkReuseMapRecorder;

    map<stream_id_t, map<link_id_t, int64_t>> streamReuseMapRecorder;

    /*   flow_id     snd_times   e2e  */
    map<stream_id_t, vector<std::pair<uint64_t, uint64_t>>> p5_e2e;

    /*   flow_id  jitter  */
    map<stream_id_t, double> p5_cached_jitter;

    /*  link_id_t                    flow_id     hop */
    map<link_id_t, vector<std::pair<stream_id_t, hop_t>>> linkFlows;

    map<group_id_t, vector<stream_id_t>> cachedFlows;

    map<group_id_t, vector<stream_id_t>> uncachedFlows;

    /*  link_id_t   hyperperiod of link */
    map<group_id_t, map<link_id_t, sched_time_t>> linkHyperperiod;

    map<link_id_t, uint64_t> linkGclSize;

    map<link_id_t, uint64_t> linkGclMergeCount;

    // the minimum variance of generation
    double variance = 0;

    double e2e;

    double ddl;

    double total_transmit;

    double v_transmit;

    double totalCache = 0;

    double total_gcl;
//    double delayQueue = 0;
//
//    double bandwidthUsage = 0;

};
#endif //SCHEDPLUS_REFACTOR_GAMIDDLECOST_H
