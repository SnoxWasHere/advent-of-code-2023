#include <iostream>
#include <istream>
#include <fstream>
#include <vector>
#include <cstdint>
#include <map>
#define array(T) std::vector<std::vector<T>>
#define GRIDSIZE 130

#define GARDEN 0
#define ROCK 1
#define FARMER 2

template <typename T>
inline bool outOfBounds(T x, T y, T w = GRIDSIZE, T h = GRIDSIZE) {
    return (x < 0 || x > w || y < 0 || y > h);
}

struct Coord {
    int32_t x, y;
    int64_t hash() {
        return (x << 8) + y;
    }
};

std::vector<Coord> cardinals {{0, 1}, {1, 0}, {0, -1}, {-1, 0}};

int main() {
    std::ifstream input ("input41.txt", std::ios::in);
    array(uint8_t) grid;
    Coord starting {0, 0};
    while (!input.eof()) {
        std::string line;
        std::getline(input, line);
        grid.emplace_back();
        for (int i = 0; i < line.length(); i++) {
            grid.back().push_back(line[i] == '#');
            if (line[i] == 'S') {
                starting.x = i;
                starting.y = grid.size() - 1;
            }
        }
    } input.close();

    std::map<int64_t, Coord> active {{starting.hash(), starting}};
    for (int x = 0; x < 64; x++) {
        std::map<int64_t, Coord> newActive {};
        for (auto& [h, node] : active) {
            for (const auto& cardinal : cardinals) {
                Coord pos {node.x + cardinal.x, node.y + cardinal.y};
                if (!outOfBounds(pos.x, pos.y)) {
                    auto& val = grid[pos.y][pos.x];
                    if (val != ROCK) {
                        val = FARMER;
                        newActive.insert({pos.hash(), pos});
                    }
                }
            }
        }   
        active = newActive;
    }
    
    
}  