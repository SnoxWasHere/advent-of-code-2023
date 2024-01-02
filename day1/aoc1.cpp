#include <iostream>
#include <istream>
#include <fstream>
#include <string>

int main() {
    std::ifstream input;
    input.open("input.txt", std::ios::in);
    int total = 0;
    while (!input.eof()) {
        std::string line;
        std::getline(input, line);
        
        for (unsigned i = 0; i < line.length(); i++) {
            char num = line[i] - '0';
            if (num >= 0 && num <= 9) {
                total += 10*num;
                break;
            }
        }
        for (unsigned i = line.length() - 1; i >= 0; i--) {
            char num = line[i] - '0';
            if (num >= 0 && num <= 9) {
                total += num;
                break;
            }
        }
    }
    std::cout << total << std::endl;
    return 0;
}