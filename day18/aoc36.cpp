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
    {'0', dir::east},
    {'1', dir::south},
    {'2', dir::west},
    {'3', dir::north},
    {'U', dir::north},
    {'R', dir::east},
    {'D', dir::south},
    {'L', dir::west}
};

struct Instruction {
    dir d;
    int64_t len;
    Instruction(std::string in) {
        d = directions[in[7]];
        len = std::stoull(in.substr(2, 5), nullptr, 16);
    }
};

struct Coord {
    int64_t x = 0, y = 0;
};
Coord operator+(const Coord& lhs, const Coord& rhs) {
    return {lhs.x + rhs.x, lhs.y + rhs.y};
}
Coord operator-(const Coord& lhs, const Coord& rhs) {
    return {lhs.x - rhs.x, lhs.y - rhs.y};
}
template <typename T>
Coord operator*(const Coord& lhs, const T m) {
    return {lhs.x * m, lhs.y * m};
}

std::map<dir, Coord> cardinals {
    {dir::north, {0,-1}},
    {dir::east,  {1, 0}},
    {dir::south, {0, 1}},
    {dir::west,  {-1,0}},
};

inline bool outOfBounds(Coord& c, int h, int w) {
    return (c.x < 0 || c.x >= w || c.y < 0 || c.y >= h);
}

inline int64_t crossMult(const Coord& lhs, const Coord& rhs) {
    return ((lhs.x * rhs.y) - (lhs.y * rhs.x));
}

int64_t shoelace(std::vector<Coord>& grid) {
    //grid.push_back(grid.front());
    int64_t out = 0;
    for (uint64_t i = 0; i < grid.size() - 1; i++) {
        out += crossMult(grid[i], grid[i+1]);
    }
    return out/2;
}

int main() {
    std::ifstream input("input35.txt", std::ios::in);
    std::vector<Instruction> instructions;

    while (!input.eof()) {
        std::string line;
        std::getline(input, line);
        instructions.push_back(process(line, " ")[2]);
    } input.close();

    std::vector<Coord> grid {{0,0}};
    int len = 0;
    for (const auto& instr : instructions) {
        Coord ofs = cardinals[instr.d];
        len += instr.len;
        grid.push_back(grid.back() + (ofs*instr.len));
    }

    std::cout << (shoelace(grid) + (len)/2 + 1)  << std::endl;

    return 0;
}
