#include <iostream>
#include <istream>
#include <fstream>
#include <vector>
#include <functional>

struct Gear {
    int x, y, num = 0;
};

inline bool outOfBounds(int w, int h, int x, int y) {
    return (x < 0 || x > w || y < 0 || y > h);
}

Gear checkAdjacent(std::vector<std::vector<char>>& grid, int wx, int hy) {
    std::string characters = "0123456789.";
    const int height = grid.size() - 1;
    const int width  = grid.back().size() - 1;
    for (int x = -1; x <= 1; x++) {
        for (int y = -1; y <= 1; y++) {
            if(!(x == 0 && y == 0) && !outOfBounds(width, height, wx + x, hy + y)) {
                if (grid[hy + y][wx + x] == '*') {return Gear{.x = wx + x, .y = hy + y, .num = 1};}
            }
        }
    }
    return Gear{.x = 0, .y = 0, .num = 0};
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
    std::vector<Gear> gears;
    std::vector<Gear> tempGears;
    bool isAdjacent = false;
    while (true) {
        num.push_back(grid[y][x]);
        if (checkIfNum(num.back()) != -1) {
            Gear found = checkAdjacent(grid, x, y);
            if (found.num) {
                isAdjacent = true;
                if(tempGears.size() == 0 || !(tempGears.back().x == found.x && tempGears.back().y == found.y)) {
                    tempGears.push_back(found);
                }
            }
        } else {
            if(isAdjacent) {
                int gearNum = vecToInt(num);
                for (auto& gear : tempGears) {
                    gear.num = gearNum;
                    gears.push_back(gear);
                }
            }
            num.clear();
            tempGears.clear();
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
    for (int a = 0; a < gears.size(); a++) {
        for (int b = a + 1; b < gears.size(); b++) {
            if(gears[a].x == gears[b].x && gears[a].y == gears[b].y && gears[a].num != gears[b].num) {
                total += gears[a].num * gears[b].num;
            }
        }
    }

    std::cout << total << std::endl;
    return 0;
}