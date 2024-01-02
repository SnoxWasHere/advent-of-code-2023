#include <iostream>
#include <istream>
#include <fstream>
#include <vector>
#include <cstdint>

std::vector<int64_t> process(std::string line, const std::string delimiter, const int size = 2) {
    std::vector<int64_t> output;
    while (true) {
        int pos = (line.find(delimiter) != std::string::npos) ? line.find(delimiter) : line.length();
        output.push_back(std::stoll(line.substr(0, pos)));
        if (line.length() > pos + size) {
            line.erase(0, pos + size);
        } else {
            break;
        }
    }
    return output;
}

template <typename T>
void printVec(std::vector<T>& vec) {
    for (auto& v : vec) {
        std::cout << v << " ";
    }
    std::cout << std::endl;
}

template <typename T>
std::pair<std::vector<T>, bool> derivative(std::vector<T>& sequence) {
    std::vector<T> secant;
    bool linear = true;
    for (int i = 0; i < sequence.size() - 1; i++) {
        T val = sequence[i+1] - sequence[i];
        if(linear && (i != 0) && (val != secant.back())) {
            linear = false;
        }
        secant.push_back(val);
    }
    return {secant, linear};
}

template <typename T>
T extrapolate(std::vector<std::vector<T>> derivs) {
    for (int i = derivs.size() - 2; i >= 0; i--) {
        derivs[i].insert(derivs[i].begin(), 1, derivs[i].front() - derivs[i+1].front());
        //printVec(derivs[i]);
    }
    return derivs[0].front();
}

int main() {
    std::vector<std::vector<int64_t>> sequences;
    std::ifstream input("input17.txt", std::ios::in);

    while (!input.eof()) {
        std::string line;
        std::getline(input, line);
        sequences.push_back(process(line, " ", 1));
    } 
    int64_t output = 0;
    for (auto& sequence : sequences) {
        std::vector<std::vector<int64_t>> derivs;
        derivs.push_back(sequence);
        //printVec(sequence);
        while (true) {
            auto [dx, linear] = derivative(derivs.back());
            derivs.push_back(dx);
            //printVec(dx);
            if (linear) {break;}   
        }
        output += extrapolate(derivs);
    }
    std::cout << output << std::endl;
    return 0;
}