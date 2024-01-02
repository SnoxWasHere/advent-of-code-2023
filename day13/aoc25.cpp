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

int search(const array(uint8_t)& grid, int by = -1, int bx = -1) {
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
                return (y+1)*100;
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
                return (x+1);
            }
        }
    }
    return 0;
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
    int x = 0;
    for (auto& grid : grids) {
        count += search(grid);
        x++;
    }
    std::cout << count << std::endl;

    return 0;
}