#include <iostream>
#include <istream>
#include <fstream>
#include <vector>
#include <map>
#include <cstdint>
#include <initializer_list>
#define GRIDSIZE 140
#define CHECK_CONCAVE(a, b, dir) {\
output[a][b] += (dir) ? vec[a][b] : 0;\
if(!dir && (vec[a][b] == 0)) {dir = true;}\
}


template <typename T>
void printVec(const std::vector<std::vector<T>>& vec) {
    std::ofstream output("output2.txt");
    for (int y = 0; y < vec.size(); y++) {
        for (int x = 0; x < vec[y].size(); x++) {
            if(vec[y][x] == 0) {output << ' ';}
            else {output << (char(vec[y][x] + '0'));}

        }
        output << std::endl;
    }
}

std::vector<std::vector<uint8_t>> concaveFind(const std::vector<std::vector<uint8_t>>& vec) {
    std::vector<std::vector<uint8_t>> output(GRIDSIZE, std::vector<uint8_t>(GRIDSIZE, 0));
    for (int a = 0; a < GRIDSIZE; a++) {
        bool left = false, right = false, up = false, down = false;
        for (int b = 0; b < GRIDSIZE; b++) {
            CHECK_CONCAVE(a, b,          left);
            CHECK_CONCAVE(a, GRIDSIZE-b-1, right);
            CHECK_CONCAVE(b, a,          down);
            CHECK_CONCAVE(GRIDSIZE-b-1, a, up);
        }
        
    }
    return output;
}

int main() {
    std::ifstream input("input20.txt", std::ios::in);
    std::vector<std::vector<uint8_t>> grid;

    while (!input.eof()) {
        std::string line; std::getline(input, line);
        grid.emplace_back();
        for (int i = 0; i < line.length(); i++) {
            grid.back().push_back(line[i] == ' ');
        }
    } input.close();
    auto oldGrid = grid;
    printVec(concaveFind(grid));

    return 0;
}