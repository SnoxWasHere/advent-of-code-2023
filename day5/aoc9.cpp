#include <iostream>
#include <istream>
#include <fstream>
#include <vector>
#include <cstdint>
#include <map>
#include <algorithm>
#define pair_3 uint64_t, std::pair<uint64_t, uint64_t>


std::pair<pair_3> process(std::string line) {
    std::pair<pair_3> output;
    output.second.first = std::stoull(line.substr(0, line.find(" ")));
    line.erase(0, line.find(" ") + 1);

    output.first = std::stoull(line.substr(0, line.find(" ")));
    line.erase(0, line.find(" ") + 1);

    output.second.second = std::stoull(line);

    return output;
}


uint64_t search(std::map<pair_3>& map, uint64_t key) {
    auto itr = map.lower_bound(key);
    if(!map.count(key)) {
        itr--;
    }
    uint64_t range = key - itr->first;
    if (range > itr->second.second) {
        return key;
    } else {
        return itr->second.first + range;
    }
}

int main() {
    std::vector<uint64_t> seeds {763445965, 78570222, 1693788857, 146680070, 1157620425, 535920936, 3187993807, 180072493, 1047354752, 20193861, 2130924847, 274042257, 20816377, 596708258, 950268560, 11451287, 3503767450, 182465951, 3760349291, 265669041};
    std::vector<std::string> inputs {"seed-to-soil", "soil-to-fertilizer", "fertilizer-to-water", "water-to-light", "light-to-temperature", "temperature-to-humidity", "humidity-to-location"};
    //data is dest, source, range
    //source, dest, range
    std::vector<std::map<pair_3>> maps;
    for (auto& file : inputs) {
        std::ifstream input(file + ".txt", std::ios::in);
        maps.emplace_back();
        while (!input.eof()) {
            std::string line;
            std::getline(input, line);
    
            std::pair<pair_3> data = process(line);
            maps.back().insert(data);
        }
    }
    uint64_t lowestVal = UINT64_MAX;
    for (auto& seed : seeds) {
        uint64_t value = seed;
        for(auto& map : maps) {
            value = search(map, value);
        }
        if (value < lowestVal) {lowestVal = value;}
    }
    std::cout << lowestVal << std::endl;
    return 0;
}
