#include <iostream>
#include <istream>
#include <fstream>
#include <vector>
#include <cstdint>
#include <map>
#include <algorithm>
#include <set>

int hash(const std::string& str) {
    uint64_t output = 0;
    for (const auto& c : str) {
        output += c;
        output *= 17;
        output %= 256;
    }
    return (output % 256);
}

struct Lens {
    std::string label;
    int val;
};

int findPos(const std::vector<Lens>& vec, const Lens& other) {
    for (int x = 0; x < vec.size(); x++) {
        if(vec[x].label == other.label) {
            return x;
        }
    }
    return -1;
}

int main() {
    std::ifstream input("input29.txt", std::ios::in);

    std::vector<Lens> instructions;
    std::map<int, std::vector<Lens>> boxes;
    std::set<char> nums {'1', '2', '3', '4', '5', '6', '7', '8', '9'};
    while (!input.eof()) {
        std::string line;
        std::getline(input, line);
        if (line.back() == '-') {
            instructions.push_back({line.substr(0, line.size()-1), -1});
        } else {
            if (nums.count(line.back()) == 0) {std::cout << "HELP!";}
            instructions.push_back({line.substr(0, line.size()-2), (line[line.size()-1] - '0')});
        }

    } input.close();

    std::cout << instructions.size() << std::endl;
    for (const auto& instruction : instructions) {
        //std::cout << instruction.label << " " << instruction.val << "\n";
        uint8_t hashed = hash(instruction.label);
        if (instruction.val == -1) {
            if (boxes.count(hashed) >= 1) {
                auto& vec = boxes[hashed];
                if (int pos = findPos(vec, instruction); pos >= 0) {
                    vec.erase(vec.begin() + pos);
                }
            }
        } else {
            if (boxes.count(hashed) >= 1) {
                auto& vec = boxes[hashed];
                if (int pos = findPos(vec, instruction); pos >= 0){
                    vec[pos].val = instruction.val;
                } else {
                    vec.push_back(instruction);
                }
            } else {
                boxes.insert({hashed, {instruction}});
            }
        }
    }
    //calc score
    uint64_t total = 0;
    for (int i = 0; i < 256; i++) {
        if (boxes.count(i) == 0) {continue;}
        auto& vec = boxes[i];
        for (int v = 0; v < vec.size(); v++) {
            if (vec[v].val < 1) {std::cout << v << " HELP" << std::endl;}
            total += (i+1)*(v+1)*vec[v].val;
        }
    }

    std::cout << total << std::endl;
    return 0;
}