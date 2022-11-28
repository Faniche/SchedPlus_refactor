//
// Created by faniche on 22-11-27.
//

#include <iostream>
#include <chrono>
#include <map>
#include <random>

using namespace std;

int main(int argc, char **argv) {
    map<pair<uint64_t, uint64_t>, pair<uint64_t, uint64_t>> note;

    auto f1 = make_pair(1, 2);
    auto p1 = make_pair(1, 2);
    auto f2 = make_pair(1, 2);
    auto p2 = make_pair(1, 2);
    note[f1] = p1;
    note[f2] = p2;
    cout << boolalpha << "f1 exist? " << note.contains(f1) << endl;
    cout << boolalpha << "f1.p1 == f2.p2 " << (note[f1] == note[f2]) << endl;
    auto f3 = make_pair(1, 4);
    note.at(f3);
    cout << boolalpha << "f3 exist? " << note.contains(f3) << endl;
}
