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

bool illegalRoll(const std::vector<std::string>& round) {
    for (auto & pull : round ) {
        int num = std::stoi(pull.substr(0, pull.find(" ")));
        char color = pull[pull.find(" ") + 1];
        if (((color == 'r' && num > 12) || (color == 'g' && num > 13) || (color == 'b' && num > 14))) {
            //std::cout << id << " - " << num << color << std::endl;
            return true;
        }
    } return false;
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
    } 
    input.close();

    for (int id = 0; id < games.size(); id++) {
        total += id+1;
        bool found = false;
        for (auto & round : games[id]) {
            if (illegalRoll(round)) {total -= id+1; break;}
        }
    }
    std::cout << total << std::endl;
    return 0;
}