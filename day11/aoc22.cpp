#include <iostream>
#include <istream>
#include <fstream>
#include <vector>
#include <cstdint>
#include <map>

#define SPACE  0
#define GALAXY 1
#define HYPERSPACE 2

struct Coord {
    int x, y;
};

template <typename T>
std::vector<std::pair<T, T>> getPairs(std::vector<T> vals) {
    std::vector<std::pair<T, T>> output;
    for (int i = 0; i < vals.size(); i++) {
        for (int j = i + 1; j < vals.size(); j++) {
            output.push_back({vals[i], vals[j]});
        }
    }
    return output;
} 

uint64_t findPath(Coord first, Coord second, const std::vector<std::vector<uint8_t>>& grid) {
    int xDir = (first.x < second.x) ? 1 : -1;
    int yDir = (first.y < second.y) ? 1 : -1;

    uint64_t total = 0;
    for (int y = first.y; y != second.y; y += yDir) {
        total += (grid[y][first.x] == HYPERSPACE) ? 1'000'000 : 1;
    }
    for (int x = first.x; x != second.x; x += xDir) {
        total += (grid[second.y][x] == HYPERSPACE) ? 1'000'000 : 1;
    }
    
    return total;
}

class Universe {
private:
    std::vector<std::vector<uint8_t>> grid;
    void insertColumn(int x) {
        for (int y = 0; y < grid.size(); y++) {
            grid[y][x] = HYPERSPACE;
        }
    }
    void insertRow(int y) {
        for (int x = 0; x < grid[y].size(); x++) {
            grid[y][x] = HYPERSPACE;
        }
    }
public:
    Universe(std::vector<std::vector<uint8_t>>& g) {grid = g;}
    void expand() {
        for (int x = 0; x < grid.back().size(); x++) {
            bool empty = true;
            for (int y = 0; y < grid.size(); y++) {
                if (grid[y][x] == GALAXY) {empty = false; break;}
            }
            if (empty) {
                insertColumn(x); x++;
            }
        }
        for (int y = 0; y < grid.size(); y++) {
            bool empty = true;
            for (int x = 0; x < grid.back().size(); x++) {
                if (grid[y][x] == GALAXY) {empty = false; break;}
            }
            if (empty) {
                insertRow(y); y++;
            }
        }
    }
    auto get() {return grid;}
    
    std::vector<Coord> getGalaxies() {
        std::vector<Coord> output;
        for (int y = 0; y < grid.size(); y++) {
            for (int x = 0; x < grid.back().size(); x++) {
                if(grid[y][x] == GALAXY) {
                    output.push_back({x, y});
                }
            } 
        }
        return output;
    }
};

template <typename T>
void printVec(const std::vector<std::vector<T>>& vec) {
    std::ofstream output("output3.txt");
    for (int y = 0; y < vec.size(); y++) {
        for (int x = 0; x < vec[y].size(); x++) {
            output << char(vec[y][x] + '0');
        }
        output << std::endl;
    }
}


int main() {
    std::ifstream input("input21.txt", std::ios::in);
    std::vector<std::vector<uint8_t>> grid;

    while (!input.eof()) {
        std::string line; std::getline(input, line);
        grid.emplace_back();
        for (int i = 0; i < line.length(); i++) {
            grid.back().push_back(line[i] != '.');
        }
    } input.close();

    Universe uni(grid);
    uni.expand();
    grid = uni.get();
    uint64_t total = 0;
    auto pairs = getPairs(uni.getGalaxies());
    for (auto& pair : pairs) {
        total += findPath(pair.first, pair.second, grid);
    }
    std::cout << total << std::endl;
}