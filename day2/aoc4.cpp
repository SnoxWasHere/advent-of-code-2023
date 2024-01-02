#include <iostream>
#include <istream>
#include <fstream>
#include <vector>

struct Bag {
    int red = 0, green = 0, blue = 0;
};

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
    std::ifstream input("input3.txt", std::ios::in);
    int total = 0;
    std::vector<std::vector<std::vector<std::string>>> games;
    while (!input.eof()) {
        std::string line;
        std::getline(input, line);
        line = line.substr(line.find(": ") + 2);

        std::vector<std::string> lists;
        lists = process(line, ";");

        games.emplace_back();
        for (auto & list : lists) {
            games.back().push_back(process(list, ","));
        }
    } input.close();

    for (int id = 0; id < games.size(); id++) {
        bool found = false;
        Bag maximum;
        for (auto & round : games[id]) {
            for (auto & pull : round ) {
                int num = std::stoi(pull.substr(0, pull.find(" ")));
                char color = pull[pull.find(" ") + 1];
                if (color == 'r') {
                    if (maximum.red < num)  {maximum.red = num;}
                } else if (color == 'g') {
                    if (maximum.green < num){maximum.green = num;}
                } else if(color == 'b') {
                    if (maximum.blue < num) {maximum.blue = num;}
                }
            } 
        } 
        total += maximum.red * maximum.blue * maximum.green;
    } 
    std::cout << total << std::endl;
    return 0;
}