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
typedef uint8_t PRIORITY_CODE_POINT;
constexpr PRIORITY_CODE_POINT P0 = 0;     // 0 (lowest)       Background
constexpr PRIORITY_CODE_POINT P1 = 1;     // 1 (default)      Best effort
constexpr PRIORITY_CODE_POINT P2 = 2;     // 2                Excellent effort
constexpr PRIORITY_CODE_POINT P3 = 3;     // 3                Critical applications
constexpr PRIORITY_CODE_POINT P4 = 4;     // 4                Video, < 100 ms latency and jitter
constexpr PRIORITY_CODE_POINT P5 = 5;     // 5                Voice, < 10 ms latency and jitter
constexpr PRIORITY_CODE_POINT P6 = 6;     // 6                Internetwork control
constexpr PRIORITY_CODE_POINT P7 = 7;     // 7 (highest)      Network control
//enum PRIORITY_CODE_POINT {
//    P0,     // 0 (lowest)       Background
//    P1,     // 1 (default)      Best effort
//    P2,     // 2                Excellent effort
//    P3,     // 3                Critical applications
//    P4,     // 4                Video, < 100 ms latency and jitter
//    P5,     // 5                Voice, < 10 ms latency and jitter
//    P6,     // 6                Internetwork control
//    P7      // 7 (highest)      Network control
//};

/**
 * @brief Inter Frame Gap, in Gigbit ethernet: 96ns
 **/
constexpr uint8_t IFG_TIME = 96;

/**
 * @brief Inter Frame Gap, in Gigbit ethernet: 96ns
 **/
constexpr uint8_t HEADER_LEN = 22 + 8;

typedef uint32_t frame_id;
typedef uint32_t stream_id;
typedef uint32_t node_id;
typedef uint32_t link_id;
typedef uint32_t port_id;

typedef bool GATE_EVENT;
constexpr GATE_EVENT GATE_CLOSE = false;
constexpr GATE_EVENT GATE_OPEN =  true;

constexpr uint32_t MTU = 1500;


#endif //SCHEDPLUS_REFACTOR_TYPE_H
