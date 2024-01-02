#include <iostream>
#include <istream>
#include <fstream>
#include <vector>
#include <cstdint>
#define array(x) std::vector<std::vector<x>>

template <typename T>
std::vector<T> getRow(const array(T)& grid, int x) {
    std::vector<T> output;
    for (size_t y = 0; y < grid.size(); y++) {
        output.push_back(grid[y][x]);
    }
    return output;
}

//x, y
std::pair<int, int> search(const array(uint8_t)& grid, int by = -1, int bx = -1) {
    //vertical search
    for (int y = 0; y <= grid.size() - 2; y++) {
        if (y == by) {continue;}
        if(grid[y] == grid[y+1]) {
            int idx = 0;
            while ((y - idx >= 0) && (y+1+idx <= grid.size() - 1)) {
                if (grid[y-idx] != grid[y+1+idx]) {idx = -1; break;}
                idx++;
            }
            if (idx > 0) {
                return std::make_pair(-1, y);
            }
        }
    }
    
    //horizontal search
    for (int x = 0; x <= grid.back().size() - 2; x++) {
        if(getRow(grid, x) == getRow(grid, x+1)) {
            if (x == bx) {continue;}
            int idx = 1;
            while (((x - idx) >= 0) && ((x+1+idx) <= grid.back().size() - 1)) {
                auto left = getRow(grid, x-idx);
                auto right = getRow(grid, x+1+idx);
                if (left != right) {idx = -1; break;}
                idx++;
            }
            if (idx > 0) {
                return std::make_pair(x, -1);
            }
        }
    }
    return std::make_pair(-1, -1);
}

std::pair<int, int> smudgeSearch(array(uint8_t) grid) {
    auto [bx, by] = search(grid);
    for (int y = 0; y < grid.size(); y++) {
        for (int x = 0; x < grid[y].size(); x++) {
            auto smudgedGrid = grid;
            smudgedGrid[y][x] ^= 1;
            auto pair = search(smudgedGrid, by, bx);
            if (pair != std::make_pair(-1, -1)) {
                return pair;
            }
        }  
    }
    std::cerr << "Whoops!" << std::endl;
    return {-1, -1};
}


int main() {
    std::ifstream input("input25.txt", std::ios::in);
    std::vector<array(uint8_t)> grids;
    grids.emplace_back();
    while (!input.eof()) {
        std::string line;
        std::getline(input, line); 
        if(line == "") {
            grids.emplace_back();
            continue;
        }
        grids.back().emplace_back();
        for (auto& c : line) {
            grids.back().back().push_back(c == '#');
        }
    }

    uint64_t count = 0;
    int c = 0;
    for (auto& grid : grids) {
        auto [x, y] = smudgeSearch(grid);
        count += (x >= 0) ? (x+1) : ((y+1)*100);
        c++;
    }
    std::cout << count << std::endl;

    return 0;
}