#include <iostream>
#include <istream>
#include <fstream>
#include <vector>
#include <cstdint>

int main() {
    std::pair<uint64_t,uint64_t> race {61677571, 430103613071150};
    uint64_t total = 0;
    for (uint64_t x = 1; x <= race.first; x++) {
        if (x * (race.first - x) > race.second) {total++;}
    }
    std::cout << total << std::endl;
}