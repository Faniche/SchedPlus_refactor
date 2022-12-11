//
// Created by faniche on 22-10-17.
//

#ifndef SCHEDPLUS_REFACTOR_TYPE_H
#define SCHEDPLUS_REFACTOR_TYPE_H

#include <cstdint>
#include <memory>
#include <iostream>
#include "../lib/spdlog/include/spdlog/spdlog.h"

/**
 * @brief Definition of Priority Code Point for vlan frame https://en.wikipedia.org/wiki/IEEE_P802.1p
 */
typedef uint8_t pcp_t;
constexpr pcp_t P0 = 0;     // 0 (lowest)       Background
constexpr pcp_t P1 = 1;     // 1 (default)      Best effort
constexpr pcp_t P2 = 2;     // 2                Excellent effort
constexpr pcp_t P3 = 3;     // 3                Critical applications
constexpr pcp_t P4 = 4;     // 4                Video, < 100 ms latency and jitter
constexpr pcp_t P5 = 5;     // 5                Voice, < 10 ms latency and jitter
constexpr pcp_t P6 = 6;     // 6                Internetwork control
constexpr pcp_t P7 = 7;     // 7 (highest)      Network control

/**
 * @brief Inter Frame Gap, in Gigbit ethernet: 96ns
 **/
constexpr uint8_t IFG_TIME = 96;

/**
 * @brief Inter Frame Gap, in Gigbit ethernet: 96ns
 **/
constexpr uint8_t HEADER_LEN = 22 + 8;

typedef int32_t frame_id_t;
typedef int32_t stream_id_t;
typedef int32_t node_id_t;
typedef std::pair<node_id_t, node_id_t> link_id_t;
typedef int32_t port_id_t;
typedef int64_t sched_time_t;
typedef int64_t route_t;
typedef size_t hop_t;

typedef bool gate_event_t;
constexpr gate_event_t GATE_CLOSE = false;
constexpr gate_event_t GATE_OPEN =  true;

constexpr int32_t MTU = 1500;

typedef int64_t eth_speed_t;
constexpr eth_speed_t   _10_ETHERNET = 10000000;
constexpr eth_speed_t  _100_ETHERNET = 100000000;
constexpr eth_speed_t _1000_ETHERNET = 1000000000;



#endif //SCHEDPLUS_REFACTOR_TYPE_H
