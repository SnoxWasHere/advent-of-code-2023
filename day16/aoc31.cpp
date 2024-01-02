#include <iostream>
#include <istream>
#include <fstream>
#include <vector>
#include <cstdint>
#include <map>
#include <set>
#define array(x) std::vector<std::vector<x>>

inline bool outOfBounds(int x, int y, int w = 109, int h = 109) {
    return (x < 0 || x > w || y < 0 || y > h);
}

enum dir {
    none,
    north,
    east,
    south,
    west
};

struct Coord {
    int x, y;
    dir to;
};

//{{char, to}, to}
std::map<std::pair<char, dir>, std::vector<dir>> connectors = {
    {{'\\', dir::east}, {dir::south}},
    {{'\\', dir::south}, {dir::east}},
    {{'\\', dir::north}, {dir::west}},
    {{'\\', dir::west}, {dir::north}},

    {{'/', dir::west}, {dir::south}},
    {{'/', dir::south}, {dir::west}},
    {{'/', dir::north}, {dir::east}},
    {{'/', dir::east}, {dir::north}},

    {{'|', dir::north}, {dir::north}},
    {{'|', dir::south}, {dir::south}},
    {{'|', dir::east}, {dir::north, dir::south}},
    {{'|', dir::west}, {dir::north, dir::south}},

    {{'-', dir::east}, {dir::east}},
    {{'-', dir::west}, {dir::west}},
    {{'-', dir::north}, {dir::east, dir::west}},
    {{'-', dir::south}, {dir::east, dir::west}}
};

//dir is to val
std::map<dir, Coord> cardinals {
    {dir::north, {0,-1, dir::north}},
    {dir::east,  {1, 0, dir::east}},
    {dir::south, {0, 1, dir::south}},
    {dir::west,  {-1,0, dir::west}},
};

std::vector<Coord> step(const array(char)& grid, const Coord& coord, std::vector<std::vector<std::pair<uint8_t, std::set<dir>>>>& illuminated) {
    Coord pos = cardinals[coord.to];
    pos.x += coord.x; 
    pos.y += coord.y;

    if (!outOfBounds(pos.x, pos.y)) {
        if (illuminated[pos.y][pos.x].second.count(pos.to) != 0) {return {};}
        illuminated[pos.y][pos.x].first = 1;
        illuminated[pos.y][pos.x].second.insert(pos.to);
        char val = grid[pos.y][pos.x];
        if (connectors.count({val, coord.to}) == 0) {
            return {pos};
        } else {
            std::vector<dir> newDirs = connectors[{val, coord.to}];
            std::vector<Coord> output;
            for (const auto& d : newDirs) {
                output.push_back({pos.x, pos.y, d});
            }
            return output;
        }
    } 
    return {};
}

void printVec(const std::vector<std::vector<std::pair<uint8_t, std::set<dir>>>>& vec, const std::vector<Coord>& active, const array(char)& old) {
    std::ofstream output("output.txt");
    for (int y = 0; y < vec.size(); y++) {
        for (int x = 0; x < vec[y].size(); x++) {
            bool isNode = false;
            for(const auto& c : active) {
                if ((c.x == x) && (c.y == y)) {
                    isNode = true;
                    break;
                }
            }
            if (isNode) {output << 'O'; continue;;}
            output << ((vec[y][x].first == 0) ? old[y][x] : '#');

        }
        output << std::endl;
    }
}


int countVal(const std::vector<std::vector<std::pair<uint8_t, std::set<dir>>>>& vec, const uint8_t val = 1) {
    int c = 0;
    for (const auto& y : vec) {
        for (const auto& x : y) {
            if (x.first == val) {c++;}
        }
    }
    return c;
}

int main() {
    std::ifstream input("input31.txt", std::ios::in);
    array(char) grid;
    std::vector<std::vector<std::pair<uint8_t, std::set<dir>>>> illuminated;
    
    while (!input.eof()) {
        std::string line;
        std::getline(input, line); 
        grid.emplace_back();
        illuminated.emplace_back();
        for (auto& c : line) {
            grid.back().push_back(c);
            illuminated.back().push_back({0, {}});
        }
    } input.close();

    int maxVal = 0;
    //left edge
    for (int y = 0; y < 110; y++) {
        auto newIll = illuminated;
        std::vector<Coord> active {{-1, y, dir::east}};
        while (active.size() > 0) {
            std::vector<Coord> newActive {};
            for (const auto& node : active) {
                auto out = step(grid, node, newIll);
                newActive.insert(newActive.begin(), out.begin(), out.end());
            }
            active = newActive;
        }
        if (int cval = countVal(newIll); cval > maxVal) {maxVal = cval;}
    }
    
    //right edge
    for (int y = 0; y < 110; y++) {
        auto newIll = illuminated;
        std::vector<Coord> active {{110, y, dir::west}};
        while (active.size() > 0) {
            std::vector<Coord> newActive {};
            for (const auto& node : active) {
                auto out = step(grid, node, newIll);
                newActive.insert(newActive.begin(), out.begin(), out.end());
            }
            active = newActive;
        }
        if (int cval = countVal(newIll); cval > maxVal) {maxVal = cval;}
    }

    //top edge
    for (int x = 0; x < 110; x++) {
        auto newIll = illuminated;
        std::vector<Coord> active {{x, -1, dir::south}};
        while (active.size() > 0) {
            std::vector<Coord> newActive {};
            for (const auto& node : active) {
                auto out = step(grid, node, newIll);
                newActive.insert(newActive.begin(), out.begin(), out.end());
            }
            active = newActive;
        }
        if (int cval = countVal(newIll); cval > maxVal) {maxVal = cval;}
    }

    //bottom edge
    for (int x = 0; x < 110; x++) {
        auto newIll = illuminated;
        std::vector<Coord> active {{x, 100, dir::north}};
        while (active.size() > 0) {
            std::vector<Coord> newActive {};
            for (const auto& node : active) {
                auto out = step(grid, node, newIll);
                newActive.insert(newActive.begin(), out.begin(), out.end());
            }
            active = newActive;
        }
        if (int cval = countVal(newIll); cval > maxVal) {maxVal = cval;}
    }


    std::cout << maxVal << std::endl;
    return 0;
}