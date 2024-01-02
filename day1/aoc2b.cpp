#include <iostream>
#include <istream>
#include <fstream>
#include <vector>

int main() { 
    std::ifstream input("input2.txt", std::ios::in);
    std::vector<std::string> numbers = {"zero", "one", "two", "three", "four", "five", "six", "seven", "eight", "nine", "0", "1", "2", "3", "4", "5", "6", "7", "8", "9"};
    int total = 0;
    while (!input.eof()) {
        std::string line;
        std::getline(input, line);
        std::vector<int> found;
        for (int j = 0; j < line.length(); j++) { //iterate over line
            for (int i = 0; i < numbers.size(); i++) { //iterate over potential num words
                std::string sub = line.substr(j, numbers[i].length()); //take substring which could
                if (sub == numbers[i]) {found.push_back(i % 10); break;} //potentially equal a number
            }
        }
        total += found.front() * 10 + found.back(); //access first and last found nums
    }
    std::cout << total << std::endl;
    return 0;
}