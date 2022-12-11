//
// Created by faniche on 22-11-16.
//

#include <iostream>
#include <chrono>
#include <vector>
#include <random>

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
    uint64_t a = 10;
    uint64_t b = 8;
    int c = static_cast<int>(b - a);
    std::cout << "c = " << c << std::endl;
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::mt19937 rand_num(seed);
    std::uniform_int_distribution<int> dist(80, 1542);
    std::vector<int> lens;
    for (int i = 0; i < 100; ++i) {
        lens.push_back(dist(rand_num));
    }
    std::cout << getMt(lens) << std::endl;
}
