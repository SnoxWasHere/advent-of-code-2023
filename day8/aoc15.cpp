#include <iostream>
#include <istream>
#include <fstream>
#include <vector>
#include <cstdint>
#include <map>
#include <algorithm>

#define left false
#define right true

bool done(std::vector<std::string>& positions) {
    for (auto& pos : positions) {
        if(pos[2] != 'Z') {
            return false;
        }
    }
    return true;
}


int main() {
    std::vector<uint8_t> directions {left,right,left,right,left,right,left,left,right,right,left,right,right,left,right,right,right,left,right,right,left,right,left,left,right,right,right,left,right,right,right,left,right,right,left,left,left,left,right,right,right,left,right,left,left,right,right,left,right,right,left,right,right,left,left,left,right,right,right,left,right,right,right,left,right,right,left,right,left,right,right,left,right,left,right,left,right,left,left,right,right,right,left,right,right,right,left,left,right,right,right,left,right,right,right,left,right,right,right,left,right,left,left,left,right,right,left,right,left,right,right,right,left,right,left,right,right,right,left,left,right,right,right,left,right,left,left,right,left,right,right,right,left,right,left,right,right,right,left,right,right,left,left,right,left,right,left,right,right,left,right,left,right,left,right,right,left,right,left,right,left,right,right,right,left,right,right,left,right,left,left,right,right,left,right,right,right,left,right,right,right,left,right,right,left,right,right,right,left,right,right,left,right,left,right,right,right,left,left,right,right,right,left,left,right,right,left,right,left,right,right,right,left,right,right,right,left,left,right,right,right,left,right,left,right,right,left,right,left,right,left,right,right,left,right,left,left,right,right,left,right,left,right,left,right,right,left,right,right,right,left,right,right,right,left,right,left,right,right,left,right,right,left,left,left,right,right,right,left,left,right,left,right,right,right,left,left,right,right,right,right};
    
    std::map<std::string, std::pair<std::string, std::string>> map;
    std::vector<std::string> positions;

    std::ifstream input("input15.txt", std::ios::in);
    while (!input.eof()) {
        std::string line;
        std::getline(input, line);
        if (line[2] == 'A') {
            positions.push_back(line.substr(0, 3));
        }
        map.insert({line.substr(0, 3), {line.substr(4, 3), line.substr(8, 3)}});
    } input.close();

    uint64_t idx = 0;
    uint64_t total = 0;
    uint64_t zCount = 0;
    while (true) {
        bool done = true;
        for (int x = 0; x < positions.size(); x++) {
            auto& pos = positions[x];
            if(pos[2] == 'Z') {
                positions.erase(positions.begin() + x);
                x--;
                std::cout << total << std::endl;
                continue;
            }
            if(directions[idx] == left) {
                pos = map[pos].first;
            } else {
                pos = map[pos].second;
            }
            if (done) {
                done = (pos[2] == 'Z');
                zCount++;
            }
        }
        if (idx >= directions.size() - 1) {
            idx = 0;
            }
        else {idx++;}
        total++;
        if(done) {
            break;
            }
    }

    std::cout << total << std::endl;

    return 0;
}