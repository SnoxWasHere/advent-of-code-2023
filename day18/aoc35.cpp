#include <iostream>
#include <istream>
#include <fstream>
#include <vector>
#include <cstdint>
#include <map>
#include <unordered_map>
#include <set>
#define array(T) std::vector<std::vector<T>>

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
enum dir {
    none,
    north,
    east,
    south,
    west
};
std::unordered_map<char, dir> directions {
    {'U', dir::north},
    {'R', dir::east},
    {'D', dir::south},
    {'L', dir::west}
};

struct Instruction {
    dir d;
    int len;
    std::string color;
    Instruction(std::vector<std::string> in) {
        d = directions[in[0][0]];
        len = std::stoi(in[1]);
        color = in[2];
    }
};

struct Coord {
    int x = 0, y = 0;
};
Coord operator+(const Coord& lhs, const Coord& rhs) {
    return {lhs.x + rhs.x, lhs.y + rhs.y};
}
Coord operator-(const Coord& lhs, const Coord& rhs) {
    return {lhs.x - rhs.x, lhs.y - rhs.y};
}
std::map<dir, Coord> cardinals {
    {dir::north, {0,-1}},
    {dir::east,  {1, 0}},
    {dir::south, {0, 1}},
    {dir::west,  {-1,0}},
};

inline bool outOfBounds(Coord c, int h, int w) {
    return (c.x < 0 || c.x >= w || c.y < 0 || c.y >= h);
}

int floodFill(array(uint8_t) grid) {
    std::vector<Coord> nodes;
    nodes.push_back({0,0});
    grid[0][0] = 2;
    do {
        std::vector<Coord> newNodes = {};
        for (auto& node : nodes) {
            for (const auto& [d, cardinal] : cardinals) {
                auto c = node + cardinal;
                if (!outOfBounds(c, grid.size(), grid[c.y].size()) && (grid[c.y][c.x] == 0)) {
                    newNodes.push_back(c);
                    grid[c.y][c.x] = 2;
                }
            }
        } nodes = newNodes;
    } while (nodes.size() > 0);
    int count = 0;
    for (const auto& y : grid) {
        for (const auto& x : y) {
            if (x != 2) {count++;}
        }
    }
    return count;
}


int main() {
    std::ifstream input("input35.txt", std::ios::in);
    std::vector<Instruction> instructions;

    while (!input.eof()) {
        std::string line;
        std::getline(input, line);
        instructions.push_back(process(line, " "));
    } input.close();

    Coord topLeft;
    Coord bottomRight;
    std::vector<Coord> seen {{0,0}};
    for (const auto& instr : instructions) {
        Coord ofs = cardinals[instr.d];
        for (int l = 0; l < instr.len; l++) {
            seen.push_back(seen.back() + ofs);
        }
        const auto& b = seen.back();
        if (b.x < topLeft.x) {topLeft.x = b.x;} 
        else if (b.x > bottomRight.x) {bottomRight.x = b.x;}

        if (b.y < topLeft.y) {topLeft.y = b.y;} 
        else if (b.y > bottomRight.y) {bottomRight.y = b.y;}
    }
    topLeft = topLeft - Coord{1, 1};
    bottomRight = bottomRight + Coord{1, 1};
    Coord size = bottomRight - topLeft;
    array(uint8_t) grid(size.y+1, std::vector<uint8_t>(size.x+1, 0));
    for (const auto& c : seen) {
        grid[c.y - topLeft.y][c.x - topLeft.x] = 1;
    }

    std::cout << floodFill(grid) << std::endl;

    return 0;
}