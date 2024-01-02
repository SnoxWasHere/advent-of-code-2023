#include <iostream>
#include <istream>
#include <fstream>
#include <vector>
#include <cstdint>
#include <map>

std::vector<std::string> process(std::string line, const std::string delimiter, const int size = 1) {
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

uint8_t hash(const std::string& str) {
    uint32_t output = 0;
    for (const auto& c : str) {
        output += c;
        output *= 17;
        output %= 256;
    }
    return (output % 256);
}

int main() {
    std::ifstream input("input29.txt", std::ios::in);
    std::vector<std::vector<std::vector<uint8_t>>> sequences;
    std::vector<std::vector<int>> fields;

    std::string line;
    std::getline(input, line);
    auto parts = process(line, ",");
    input.close();

    uint64_t total = 0;
    for (const auto& str : parts) {
        total += hash(str);
    }
    std::cout << total << std::endl;
    return 0;
}