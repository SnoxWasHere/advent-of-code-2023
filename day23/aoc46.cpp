#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <cstdint>
#include <set>
#include <optional>
#include <unordered_set>
#include <unordered_map>
#include <bitset>
#include <array>
#include <bit>
#include <cassert>
#include <queue>
#define array(x) std::vector<std::vector<x>>

#define SIZE 141
#define NODECOUNT 9414
#define JUNCTIONCOUNT 36

std::vector<std::string> process(std::string line, const std::string delimiter, const int size = 1) {
    std::vector<std::string> output;
    while (true) {
        int pos = (line.find(delimiter) != std::string::npos) ? line.find(delimiter) : line.length();
        output.push_back(line.substr(0, pos));
        if (line.length() > pos + size) {
            line.erase(0, pos + size);
        } else {
            break;
        }
    }
    return output;
}

struct Path {
    uint32_t idx;
    uint32_t score;
    std::bitset<JUNCTIONCOUNT> seen {};
    Path(uint32_t i, uint32_t s) : idx(i), score(s) {
        seen[i] = 1;
    }
    Path(uint32_t i, uint32_t s, auto b) : idx(i), score(s), seen(b) {
        seen[i] = 1;
    }
};
auto operator<=>(const Path& lhs, const Path& rhs) {
    return lhs.score <=> rhs.score;
}

class Tree {
private:
    std::array<std::array<uint32_t, JUNCTIONCOUNT>, JUNCTIONCOUNT> matrix; 
    std::vector<uint32_t> getConnects(uint32_t idx, const std::bitset<JUNCTIONCOUNT>& seen) {
        std::vector<uint32_t> out;
        for (uint32_t x = 0; x < matrix.size(); x++) {
            if ((matrix[idx][x] > 0) && (seen[x] != 1)) {out.push_back(x);}        
        }
        return out;
    }
public:
    Tree(auto& m) : matrix(m) {}
    uint32_t findLongest() {
        std::priority_queue<Path, std::vector<Path>, std::less<Path>> pq;
        std::set<uint32_t> scores;
        pq.push(Path(0, 0));
        while (!pq.empty()) {
            Path current = pq.top();
            pq.pop();
            if (current.idx == 1) {scores.insert(current.score);}
            auto connects = getConnects(current.idx, current.seen);
            for (const auto& c : connects) {
                pq.push(Path(c, current.score + matrix[current.idx][c], current.seen));
            }
        }
        return 0;
    }
};

int main() {
    std::ifstream input("input46b.txt", std::ios::in);
    std::array<std::array<uint32_t, JUNCTIONCOUNT>, JUNCTIONCOUNT> smallmatrix {};

    for (int y = 0; y < JUNCTIONCOUNT; y++) {
        std::string line;
        std::getline(input, line); 
        auto split = process(line, " ");
        for (int x = 0; x < JUNCTIONCOUNT; x++) {
            smallmatrix[y][x] = std::stoi(split[x]);
        }
    } input.close();

    Tree t(smallmatrix);
    t.findLongest();

    return 0;
}