#include <iostream>
#include <istream>
#include <fstream>
#include <vector>
#include <cstdint>
#include <map>

#define SPACE  0
#define GALAXY 1

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

int findPath(Coord first, Coord second) {
    return (std::abs(first.x - second.x) + std::abs(first.y - second.y));
}

class Universe {
private:
    std::vector<std::vector<uint8_t>> grid;
    void insertColumn(int x) {
        for (int y = 0; y < grid.size(); y++) {
            grid[y].insert(grid[y].begin() + x, SPACE);
        }
    }
    void insertRow(int y) {
        grid.insert(grid.begin() + y, std::vector<uint8_t>(grid.back().size(), SPACE));
    }
public:
    Universe(std::vector<std::vector<uint8_t>>& g) {grid = g;}
    void expand() {
        for (int x = 0; x < grid.back().size(); x++) {
            bool empty = true;
            for (int y = 0; y < grid.size(); y++) {
                if (grid[y][x] != SPACE) {empty = false; break;}
            }
            if (empty) {
                insertColumn(x); x++;
            }
        }
        for (int y = 0; y < grid.size(); y++) {
            bool empty = true;
            for (int x = 0; x < grid.back().size(); x++) {
                if (grid[y][x] != SPACE) {empty = false; break;}
            }
            if (empty) {
                insertRow(y); y++;
            }
        }
    }
    
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
    std::ofstream output("output.txt");
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

    int total = 0;
    auto pairs = getPairs(uni.getGalaxies());
    for (auto& pair : pairs) {
        total += findPath(pair.first, pair.second);
    }
    std::cout << total << std::endl;
}