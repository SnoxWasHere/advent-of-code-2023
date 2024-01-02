#include <iostream>
#include <istream>
#include <fstream>
#include <vector>
#include <cstdint>
#include <set>
#include <map>
#define array(x) std::vector<std::vector<x>>

#define SPACE 0
#define CIRCLE 1
#define SQUARE 2

std::map<char, uint8_t> valsMap {
    {'.', SPACE},
    {'O', CIRCLE}, //or
    {'#', SQUARE}
};

inline bool outOfBounds(int x, int y, int w = 99, int h = 99) {
    return (x < 0 || x > w || y < 0 || y > h);
}

template <typename T>
void printVec(const std::vector<std::vector<T>>& vec) {
    std::ofstream output("output.txt");
    for (int y = 0; y < vec.size(); y++) {
        for (int x = 0; x < vec[y].size(); x++) {
            output << (int(vec[y][x]));

        }
        output << std::endl;
    }
}


int main() {
    std::ifstream input("input27.txt", std::ios::in);
    array(uint8_t) grid;
    while (!input.eof()) {
        std::string line;
        std::getline(input, line); 
        grid.emplace_back();
        for (auto& c : line) {
            grid.back().push_back(valsMap[c]);
        }
    } input.close();

    auto oldGrid = grid;

    for (uint64_t i = 0; i < 100'000; i++) {
        //north
        for (int y = 0; y < 100; y++) {
            for (int x = 0; x < grid[y].size(); x++) {
                uint8_t val = grid[y][x];
                if (val == CIRCLE) {
                    int wy = y;
                    while (((wy-1) >= 0) && (grid[wy-1][x] == SPACE)) {
                        grid[wy][x] = SPACE;
                        grid[wy-1][x] = CIRCLE;
                        wy--;
                    }
                }
            }
        }
        //west
        for (int x = 0; x < 100; x++) {
            for (int y = 0; y < 100; y++) {
                uint8_t val = grid[y][x];
                if (val == CIRCLE) {
                    int wx = x;
                    while (((wx-1) >= 0) && (grid[y][wx-1] == SPACE)) {
                        grid[y][wx] = SPACE;
                        grid[y][wx-1] = CIRCLE;
                        wx--;
                    }
                }
            }
        }
        //south
        for (int y = 99; y >= 0; y--) {
            for (int x = 0; x < grid[y].size(); x++) {
                uint8_t val = grid[y][x];
                if (val == CIRCLE) {
                    int wy = y;
                    while (((wy+1) <= 99) && (grid[wy+1][x] == SPACE)) {
                        grid[wy][x] = SPACE;
                        grid[wy+1][x] = CIRCLE;
                        wy++;
                    }
                }
            }
        }
        //east
        for (int x = 99; x >= 0; x--) {
            for (int y = 0; y < 100; y++) {
                uint8_t val = grid[y][x];
                if (val == CIRCLE) {
                    int wx = x;
                    while (((wx+1) <= 99) && (grid[y][wx+1] == SPACE)) {
                        grid[y][wx] = SPACE;
                        grid[y][wx+1] = CIRCLE;
                        wx++;
                    }
                }
            }
        }
        //if (i % 10'000 == 0) {std::cout << i << std::endl;}
        //if (grid == oldGrid) {std::cout << i << std::endl;}
        if (i >= 10'000) {
            uint64_t output = 0;
            for (int y = 0; y < 100; y++) {
                for (int x = 0; x < grid[y].size(); x++) {
                    uint8_t val = grid[y][x];
                    if (val == CIRCLE) {
                        output += (100 - y);
                    }
                }
            }
            std::cout << i << " - "<< output << std::endl;
        }
        
    }
    
    


    uint64_t output = 0;
    for (int y = 0; y < 100; y++) {
        for (int x = 0; x < grid[y].size(); x++) {
            uint8_t val = grid[y][x];
            if (val == CIRCLE) {
                output += (100 - y);
            }
        }
    }
    std::cout << output << std::endl;
    return 0;
}