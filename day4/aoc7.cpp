#include <iostream>
#include <istream>
#include <fstream>
#include <vector>

std::vector<std::string> process(std::string line, const std::string delimiter, const int size = 2) {
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

int main() { 
    std::ifstream input("input7.txt", std::ios::in);
    int total = 0;
    std::vector<std::vector<std::vector<std::string>>> games;
    while (!input.eof()) {
        std::string line;
        std::getline(input, line);
        line = line.substr(line.find(": ") + 2);

        std::vector<std::string> lists;
        lists = process(line, " | ", 3);

        games.emplace_back();
        for (auto & list : lists) {
            games.back().push_back(process(list, " ", 1));
        }
    } input.close();
    for (auto& game : games) {
        int score = 0;
        for (auto& myNum : game.back()) {
            for (auto& winNum : game.front()) {
                if (myNum == winNum) {
                    score += (score == 0) ? 1 : score;
                }
            }
        }
        total += score;
    }
    std::cout << total << std::endl;
    return 0;
}