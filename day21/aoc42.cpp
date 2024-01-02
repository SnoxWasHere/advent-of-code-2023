#include <iostream>
#include <istream>
#include <fstream>
#include <vector>
#include <cstdint>
#include <map>
#define array(T) std::vector<std::vector<T>>
#define EXTRA_COPIES 5
#define GRIDSIZE 11*EXTRA_COPIES-1

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

template <typename T>
void printVec(const std::vector<std::vector<T>>& vec) {
    std::ofstream output("output.txt");
    int origin = 11*(EXTRA_COPIES/2);
    for (int y = 0; y < vec.size(); y++) {
        for (int x = 0; x < vec[y].size(); x++) {
            if (/*(abs(y-origin-65) + (abs(x-origin-65)) <= 65)*/ true) {
                output << char((vec[y][x] == 0) ? '.' : ((vec[y][x] == FARMER) ? 'O' : '#'));
            } else {
                output << '.';
            }

        }
        output << std::endl;
    }
}

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
        auto old = grid.back();
        for (int i = 1; i <= EXTRA_COPIES; i++) {
            grid.back().insert(grid.back().end(), old.begin(), old.end());
        }
    } input.close();

    auto old = grid;
    for (int i = 1; i <= EXTRA_COPIES; i++) {
        grid.insert(grid.end(), old.begin(), old.end());
    }
    printVec(grid);
    starting.x += 11*(EXTRA_COPIES/2);
    starting.y += 11*(EXTRA_COPIES/2);
    std::map<int64_t, Coord> active {{starting.hash(), starting}};
    for (int i = 0; i < EXTRA_COPIES; i++) {
        int COUNT = (i == 0) ? 5 : 11;
        for (int x = 0; x < COUNT; x++) {
            std::map<int64_t, Coord> newActive {};
            for (auto& [h, node] : active) {
                grid[node.y][node.x] = GARDEN;
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
            //count on original
            // int notal = 0;
            // int origin = 131*(EXTRA_COPIES/2);
            // for (int k = origin; k < 131+origin; k++) {
            //     for (int j = origin; j < 131+origin; j++) {
            //         if (/*((abs(j-origin-65) + (abs(k-origin-65)) <= 65))
            //         &&*/ (grid[k][j] == FARMER)) {notal++;}
            //     }
            // }
            //std::cout << "N" << notal << "\n";
            std::cout << int(i*131 + x) << " - " << active.size() << std::endl;
        }
        printVec(grid);
        
        
    }
    return 0;
}  