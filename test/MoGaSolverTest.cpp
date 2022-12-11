//
// Created by faniche on 22-12-4.
//

#include <iostream>
#include <chrono>
#include <map>
#include <random>
#include <memory>
#include "../src/topology/Line_2sw_2es.h"
#define OPENGA_EXTERN_LOCAL_VARS
#include "../src/solution/GeneticAlgorithm/MoGaSolver.h"

using namespace std;

sched_time_t compressP6Help(sched_time_t siPeriod, sched_time_t siMid, sched_time_t siLen,
                                        sched_time_t sjPeriod, sched_time_t sjMid, sched_time_t sjLen) {
    sched_time_t hpij = std::lcm(siPeriod, sjPeriod);
    sched_time_t dist = (siLen + sjLen) / 2 + IFG_TIME;
    if (siPeriod < sjPeriod) {
        sched_time_t _fi_mid = siMid % siPeriod;
        for (int k = 0; k < hpij / sjPeriod; ++k) {
            int d = (sjMid + k * sjPeriod) % siPeriod - _fi_mid;
            cout << "k[" << k << "].d = " << d << endl;
            if (std::abs(d) < dist) {
                SPDLOG_LOGGER_TRACE(spdlog::get("console"), "dist = {}, d = {}", dist, d);
                return UINT64_MAX;
            }
        }
    } else {
        sched_time_t _fj_mid = sjMid % sjPeriod;
        for (int k = 0; k < hpij / siPeriod; ++k) {
            int d = (siMid + k * siPeriod) % sjPeriod - _fj_mid;
            cout << "k[" << k << "].d = " << d << endl;
            if (std::abs(d) < dist) {
                SPDLOG_LOGGER_TRACE(spdlog::get("console"), "dist = {}, d = {}", dist, d);
                return UINT64_MAX;
            }
        }
    }
    return 1;
}

int main(int argc, char **argv) {
    compressP6Help(200000, 400, 800,     // period: 200us, start: 0, len: 800, mid: 400
                   300000, 1400, 800);   // period: 300us, start: 1000, len: 800, mid: 1400
    int a = 10;
    cout << "-a = " << -a << endl;
}
