//
// Created by faniche on 22-11-16.
//

#include <iostream>
#include <chrono>
#include <vector>
#include <random>
#include <set>

int getMt(const std::vector<int>& arr){
    std::vector<int> rec(1000, 0);
    std::vector<int> mts(1000, 0);
    for (int mt = 1; mt <= 1000; ++mt) {
        int waste = 0;
        int _mts = 0;
        for (int frame_len : arr) {
            _mts += frame_len * 8 / (mt * 1000) + 1;
            waste += (mt * 1000 - frame_len * 8 % (mt * 1000));
        }
        rec[mt - 1] = waste;
        mts[mt - 1] = _mts;
    }
    int best_mt = 1;
    int min_waste = rec[0];
    for (int i = 1; i < rec.size(); ++i) {
        if (rec[i] < min_waste) {
            best_mt = i;
            min_waste = rec[i];
        }
    }
    return best_mt;
}

int main(int argc, char **argv) {
//    uint64_t a = 10;
//    uint64_t b = 8;
//    int c = static_cast<int>(b - a);
//    std::cout << "c = " << c << std::endl;
//    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
//    std::mt19937 rand_num(seed);
//    std::uniform_int_distribution<int> dist(80, 1542);
//    std::vector<int> lens;
//    for (int i = 0; i < 100; ++i) {
//        lens.push_back(dist(rand_num));
//    }
//    std::cout << getMt(lens) << std::endl;

    std::set<std::pair<int64_t, int64_t>> setPairs;
    setPairs.emplace(100, 110);
    setPairs.emplace(111, 120);
    setPairs.emplace(122, 130);
    setPairs.emplace(133, 140);
    setPairs.emplace(140, 180);
    setPairs.emplace(180, 200);
    setPairs.emplace(201, 210);
    for(auto &iter: setPairs) {
        std::cout << iter.first << ", " << iter.second << std::endl;
    }
    auto prev = setPairs.end();
    for (auto iter = setPairs.begin(); iter != setPairs.end(); ++iter) {
        if (prev != setPairs.end() && prev->second == iter->first) {
            auto tmp = std::make_pair(prev->first, iter->second);
            iter = setPairs.erase(prev, iter.operator++());
            iter = setPairs.insert(iter.operator--(), tmp);
        }
        prev = iter;
    }
    std::cout << "-------------------------" << std::endl;
    for(auto &iter: setPairs) {
        std::cout << iter.first << ", " << iter.second << std::endl;
    }


}
