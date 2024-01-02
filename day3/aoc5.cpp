#include <iostream>
#include <istream>
#include <fstream>
#include <vector>
#include <functional>


inline bool outOfBounds(int w, int h, int x, int y) {
    return (x < 0 || x > w || y < 0 || y > h);
}



template <typename T>
T checkAdjacent(std::vector<std::vector<char>>& grid, int wx, int hy, std::function<T(char)> func) {
    const int height = grid.size() - 1;
    const int width  = grid.back().size() - 1;
    for (int x = -1; x <= 1; x++) {
        for (int y = -1; y <= 1; y++) {
            if(!(x == 0 && y == 0) && !outOfBounds(width, height, wx + x, hy + y)) {
                T result = func(grid[hy + y][wx + x]);
                if (result != 0) {
                    return result;
                    }
            }
        }
    }
    return 0;
}

bool checkIfSymbol(char val) {
    std::string characters = "0123456789.";
    return characters.find(val) == std::string::npos;
}

int checkIfNum(char val) {
    std::string characters = "0123456789";
    if (characters.find(val) != std::string::npos) {
        return (val - '0');
    } else {
        return -1;
    }
}

int vecToInt(std::vector<int> vec) {
    int output = vec[0] - '0';
    for (int i = 1; i < vec.size() - 1; i++) {
        output *= 10;
        output += vec[i] - '0';
    }
    return output;
}

int main() { 
    std::ifstream input("input5.txt", std::ios::in);
    std::vector<std::vector<char>> grid;
    while (!input.eof()) {
        std::string line; std::getline(input, line);
        grid.emplace_back();
        for (int i = 0; i < line.length(); i++) {
            grid.back().push_back(line[i]);
        }
    } input.close();

    int x = 0;
    int y = 0;
    int h = grid.size() - 1;
    int w = grid.back().size() - 1;

    int total = 0;
    std::vector<int> num;
    bool isAdjacent = false;
    while (true) {
        num.push_back(grid[y][x]);
        if (checkIfNum(num.back()) != -1) {
            if (!isAdjacent) {
                isAdjacent = checkAdjacent<bool>(grid, x, y, &checkIfSymbol);
            }
        } else {
            if(isAdjacent) {
                total += vecToInt(num);
            }
            num.clear();
            isAdjacent = false;
        }
        if (x >= w) {
            x = 0;
            if (y >= h) {
                break;
            } else {
                y++;
            }
        } else {
            x++;
        }
    } 
    std::cout << total << std::endl;
    return 0;
}