#include <iostream>
#include <istream>
#include <fstream>
#include <vector>
#include <map>
#include <cstdint>
#define GRIDSIZE 140


enum dir {
    none,
    north,
    east,
    south,
    west
};

struct Coord {
    int x, y;
    dir from, to;
};

//{{char, to}, to}
std::map<std::pair<char, dir>, dir> connectors = {
    {{'|', dir::north}, dir::north},
    {{'|', dir::south}, dir::south},

    {{'-', dir::east}, dir::east},
    {{'-', dir::west}, dir::west},

    {{'7', dir::north}, dir::west},
    {{'7', dir::east}, dir::south},

    {{'F', dir::north}, dir::east},
    {{'F', dir::west}, dir::south},

    {{'L', dir::south}, dir::east},
    {{'L', dir::west}, dir::north},

    {{'J', dir::south}, dir::west},
    {{'J', dir::east}, dir::north},
};

//dir is to val
std::map<dir, Coord> cardinals {
    {dir::north, {0,-1, dir::south, dir::north}},
    {dir::east,  {1, 0, dir::west,  dir::east}},
    {dir::south, {0, 1, dir::north, dir::south}},
    {dir::west,  {-1,0, dir::east,  dir::west}},
};

Coord findLinear(std::vector<std::vector<char>>& array, const Coord node) {
    Coord pos = cardinals[node.to];
    pos.x += node.x; 
    pos.y += node.y; 
    
    char val = array[pos.y][pos.x];
    if(connectors.count({val, pos.to}) > 0) {
        array[pos.y][pos.x] = 'O';
        //to becomes from, new to becomes to
        return {pos.x, pos.y, pos.to, connectors[{val, pos.to}]};
    }
    return {-1, -1, dir::none, dir::none};
}

template <typename T>
void printVec(const std::vector<std::vector<T>>& vec) {
    std::ofstream output("output.txt");
    for (int y = 0; y < vec.size(); y++) {
        for (int x = 0; x < vec[y].size(); x++) {
            output << ((vec[y][x] == 0) ? ' ' : '#');

        }
        output << std::endl;
    }
}

template <typename T>
std::vector<std::vector<uint8_t>> xConcave(const std::vector<std::vector<T>>& grid) {
    std::vector<std::vector<uint8_t>> output(GRIDSIZE, std::vector<uint8_t>(GRIDSIZE));
    for (int y = 0; y < GRIDSIZE; y++) {
        std::vector<std::vector<uint8_t>> spans;
        
        spans.push_back({0}); //bookend with 0
        for (int x = 0; x < GRIDSIZE; x++) {
            uint8_t val = (grid[y][x] == 'O');
            if (spans.back().back() == val) {
                spans.back().push_back(val);
            } else {
                spans.push_back({val});
            }
        }
        spans.back().push_back(0);
        
        // 0   1  [2 3  4  ]5    6
        // 000 111 0 11 000 1111 0
        int w = spans.size() - 1;
        int final = 0;
        for (int x = 0; x <= w; x++) {
            int val = 0;
            if ((x >= 2) && (x <= (w - 2))) {
                if (spans[x][0] == 0) {
                    val = 1;
                }
            }
            for (int i = 0; i < spans[x].size(); i++) {
                if (final > GRIDSIZE) {break;}
                output[y][final] = val;
                final++;
            } 
        }
    }
    return output;
}
template <typename T>
std::vector<std::vector<uint8_t>> yConcave(const std::vector<std::vector<T>>& grid) {
    std::vector<std::vector<uint8_t>> output(GRIDSIZE, std::vector<uint8_t>(GRIDSIZE));
    for (int x = 0; x < GRIDSIZE; x++) {
        std::vector<std::vector<uint8_t>> spans;
        
        spans.push_back({0}); //bookend with 0
        for (int y = 0; y < GRIDSIZE; y++) {
            uint8_t val = (grid[y][x] == 'O');
            if (spans.back().back() == val) {
                spans.back().push_back(val);
            } else {
                spans.push_back({val});
            }
        }
        spans.back().push_back(0);
        
        // 0   1  [2 3  4  ]5    6
        // 000 111 0 11 000 1111 0
        int w = spans.size() - 1;
        int final = 0;
        for (int y = 0; y <= w; y++) {
            int val = 0;
            if ((y >= 2) && (y <= (y - 2))) {
                if (spans[y][0] == 0) {
                    val = 1;
                }
            }
            for (int i = 0; i < spans[y].size(); i++) {
                if ((final == 0 ) || (final >= GRIDSIZE)) {final++; break;}
                output[final][x] = val;
                final++;
            } 
        }
    }
    return output;
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
    std::vector<Coord> nodes {{10, 61, dir::east, dir::north}, {10, 61, dir::south, dir::west}};

    while(true) {
        std::vector<Coord> newNodes = {};
        for(auto& node : nodes) {
            Coord output = findLinear(grid, node);
            if (output.x != -1) {newNodes.push_back(output);}
        }
        nodes = newNodes;

        if(nodes.size() == 0) {break;}
    }

    auto xgrid = xConcave(grid);
    printVec(xgrid);

    auto ygrid = yConcave(grid);
    printVec(ygrid);

    for (int x = 0; x < GRIDSIZE; x++) {
        for (int y = 0; y < GRIDSIZE; y++) {
            if(xgrid[y][x] && ygrid[y][x]) {count++;}
        }
    }

    std::cout << count << std::endl;

    return 0;
}