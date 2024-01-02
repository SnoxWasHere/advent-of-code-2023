#include <iostream>
#include <istream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm> 

inline int notNeg(int i) {int val = ((i >= 0) ? i : 99999); return val;}


int main() { 
    std::ifstream input;
    std::vector<std::string> numbers = {"zero", "one", "two", "three", "four", "five", "six", "seven", "eight", "nine", "0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "10"};
    input.open("input.txt", std::ios::in);
    int total = 0;
    while (!input.eof()) {
        int round = 0;
        std::string line;
        std::getline(input, line); 

        int lowest = 999999;
        int lidx = 0;

        for (int i = 0; i < numbers.size(); i++) {
            int val = -1;
            if (line.find(numbers[i]) != std::string::npos) {
                val = line.find(numbers[i]);
                if (val < lowest) {
                    lowest = val;
                    lidx = i;
                }
            }
        }

        std::reverse(line.begin(), line.end());
        int highest = 0;
        int hidx = 0;
        
        for (int i = 0; i < numbers.size(); i++) {
            std::string num = numbers[i];
            std::reverse(num.begin(), num.end());
            int val = -1;
            if (line.find(num) != std::string::npos) {
                val = line.find(numbers[i]);
                if (val > highest) {
                    highest = val;
                    hidx = i;
                }
            }
        }
        std::reverse(line.begin(), line.end());
        round += (lidx % 10) * 10 + (hidx % 10);
        std::cout << line << " - " << round << std::endl;
        total += round;
    }
    std::cout << total << std::endl;
    return 0;
}
