#include <iostream>
#include <istream>
#include <fstream>
#include <vector>
#include <map>

enum dir {
    none,
    north,
    east,
    south,
    west
};

struct Coord {
    int x;
    int y;
    dir origin;
};

template <typename T>
Coord find2D(const std::vector<std::vector<T>>& array, T member) {
    for (int y = 0; y < array.size(); y++) {
        for (int x = 0; x < array[y].size(); x++) {
            if(array[y][x] == member) {
                return {x, y, dir::none};
            }
        }
    }
    return {-1, -1, dir::none};
}

inline bool outOfBounds(int x, int y, int w = 139, int h = 139) {
    return (x < 0 || x > w || y < 0 || y > h);
}

template <typename T>
inline bool inPair(std::pair<T, T> pair, T val) {
    return ((pair.first == val) || (pair.second == val));
}

std::vector<Coord> cardinals {{0, 1, dir::north}, {1, 0, dir::east}, {0, -1, dir::south}, {-1, 0, dir::west}};
std::map<char, std::pair<dir, dir>> connectors = {
    {'|', {dir::north, dir::south}},
    {'-', {dir::east,  dir::west}},
    {'7', {dir::north, dir::east}},
    {'F', {dir::north, dir::west}},
    {'L', {dir::south, dir::west}},
    {'J', {dir::south, dir::east}}
};



std::vector<Coord> findAdjacent(std::vector<std::vector<char>>& array, const Coord node) {
    std::vector<Coord> output;
    for (const auto& cardinal : cardinals) {
        Coord pos {node.x + cardinal.x, node.y + cardinal.y, dir::none};
        if (!outOfBounds(pos.x, pos.y)) {
            char val = array[pos.y][pos.x];
            if ((connectors.count(val) > 0) && inPair(connectors[val], cardinal.origin)) {
                std::pair<dir, dir> dirs = connectors[val];
                pos.origin = (dirs.first == cardinal.origin) ? dirs.second : dirs.first;
                array[pos.y][pos.x] = 'O';
                output.push_back(pos);
            }
        }
    }
    return output;
}

Coord findLinear(std::vector<std::vector<char>>& array, const Coord node) {
    Coord output;
    Coord myCardinal;
    for (const auto& cardinal : cardinals) {
        if (cardinal.origin == node.origin) {
            myCardinal = cardinal;
            break;
        }
    }

    Coord pos {node.x + myCardinal.x, node.y + myCardinal.y, dir::none};
    if (!outOfBounds(pos.x, pos.y)) {
        char val = array[pos.y][pos.x];
        std::pair<dir, dir> dirs = connectors[val];
        pos.origin = (dirs.first == myCardinal.origin) ? dirs.second : dirs.first;
        array[pos.y][pos.x] = 'O';
        return pos;
    }
    return {-1, -1, dir::none};
}

template <typename T>
void printVec(const std::vector<std::vector<T>>& vec, const std::vector<std::vector<T>>& old) {
    std::ofstream output("output.txt");
    for (int y = 0; y < vec.size(); y++) {
        for (int x = 0; x < vec[y].size(); x++) {
            if(vec[y][x] == old[y][x]) {
                output << " ";
            } else {
                output << old[y][x];
            }
        }
        output << std::endl;
    }
}

int main() {
    std::ifstream input("input19.txt", std::ios::in);
    std::vector<std::vector<char>> grid;

    while (!input.eof()) {
        std::string line; std::getline(input, line);
        grid.emplace_back();
        for (int i = 0; i < line.length(); i++) {
            grid.back().push_back(line[i]);
        }
    } input.close();
    auto oldGrid = grid;

    int count = 0;

    std::vector<Coord> nodes;
    nodes = findAdjacent(grid, find2D(grid, 'S'));
    printVec(grid, oldGrid);
    while(true) {
        std::vector<Coord> newNodes = {};
        for(auto& node : nodes) {
            Coord output = findLinear(grid, node);
            if (output.x != -1) {newNodes.push_back(output);}
        }
        nodes = newNodes;
        count++;
        
        printVec(grid, oldGrid);

        if(nodes.size() == 0) {
            break;
        }

    }
    
    printVec(grid, oldGrid);
    
    std::cout << count << std::endl;
}