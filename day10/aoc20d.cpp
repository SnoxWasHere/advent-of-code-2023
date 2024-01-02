#include <iostream>
#include <istream>
#include <fstream>
#include <vector>
#include <map>
#include <cstdint>
#include <initializer_list>
#define GRIDSIZE 140
//empty is 0
//filled is 1


struct Coord {
    int x = 0, y = 0;
};

inline bool outOfBounds(int x, int y, int w = (GRIDSIZE*3) - 1, int h = (GRIDSIZE*3) - 1) {
    return (x < 0 || x > w || y < 0 || y > h);
}

template <typename T>
void printVec(const std::vector<std::vector<T>>& vec) {
    std::ofstream output("output6.txt");
    for (int y = 0; y < vec.size(); y++) {
        for (int x = 0; x < vec[y].size(); x++) {
            if(vec[y][x] == 0) {output << ' ';}
            else {output << (char(vec[y][x] + '0'));}

        }
        output << std::endl;
    }
}
std::map<char, std::vector<uint8_t>> connectors = {
    {'|', {1, 0, 1, 0}}, 
    {'-', {0, 1, 0, 1}},
    {'7', {0, 0, 1, 1}}, 
    {'F', {0, 1, 1, 0}},
    {'L', {1, 1, 0, 0}},
    {'J', {1, 0, 0, 1}}, 
};

std::vector<Coord> cardinals = {
    {0, -1},
    {1,  0},
    {0,  1},
    {-1, 0}
};


std::vector<std::vector<uint8_t>> expandGrid(std::vector<std::vector<char>>& grid) {
    std::vector<std::vector<uint8_t>> output(GRIDSIZE*3, std::vector<uint8_t>(GRIDSIZE*3, 0));
    for (int wy = 0; wy <= grid.size(); wy++) {
        for (int wx = 0; wx < grid[wy].size(); wx++) {
            char val = grid[wy][wx];
            if(connectors.count(val) == 0) {continue;}
            auto map = connectors[val];

            int nx = wx*3 + 1;
            int ny = wy*3 + 1;
            output[ny][nx] = 1;

            for (int it = 0; it < 4; it++) {
                auto [x, y] = cardinals[it];
                output[ny + y][nx + x] = map[it];
            }

        }
    }
    return output;
}

std::vector<Coord> findNeighbors(std::vector<std::vector<uint8_t>>& grid, Coord node) {
    std::vector<Coord> output {};
    for (int x = -1; x <= 1; x++) {
        for (int y = -1; y <= 1; y++) {
            if(!(x == 0 && y == 0) && !outOfBounds(node.x + x, node.y + y)) {
                if(grid[node.y + y][node.x + x] == 0) {
                    output.push_back({node.x + x, node.y + y});
                    grid[node.y + y][node.x + x] = 2;
                }
            }
        }
    }
    return output;
}

auto concaveFind(std::vector<std::vector<uint8_t>> grid) {
    std::vector<Coord> nodes;
    nodes.push_back({0,0});
    grid[0][0] = 2;
    do {
        std::vector<Coord> newNodes = {};
        for (auto& node : nodes) {
            auto neighbors = findNeighbors(grid, node);
            newNodes.insert(newNodes.end(), neighbors.begin(), neighbors.end());
        } nodes = newNodes;
    } while (nodes.size() > 0);
    return grid;
}

auto condense(const std::vector<std::vector<uint8_t>>& grid) {
    std::vector<std::vector<uint8_t>> output(GRIDSIZE, std::vector<uint8_t>(GRIDSIZE, 0));
    for (int wy = 0; wy <= output.size(); wy++) {
        for (int wx = 0; wx < output[wy].size(); wx++) {
            int nx = wx*3 + 1;
            int ny = wy*3 + 1;
            output[wy][wx] = grid[ny][nx];
        }
    }
    return output;
}


int main() {
    std::ifstream input("input20.txt", std::ios::in);
    std::vector<std::vector<char>> grid;

    while (!input.eof()) {
        std::string line; std::getline(input, line);
        grid.emplace_back();
        for (int i = 0; i < line.length(); i++) {
            grid.back().push_back(line[i]);
        }
    } input.close();

    auto newGrid = expandGrid(grid);
    newGrid = concaveFind(newGrid);
    printVec(newGrid);
    printVec(condense(newGrid));

    return 0;
}