#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <cstdint>
#include <set>
#include <optional>
#include <unordered_set>
#include <unordered_map>
#include <bitset>
#include <array>
#include <bit>
#include <bits/stdc++.h>
#define NODECOUNT 1479


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




template <typename T>
struct Connection {
    T lhs, rhs;
};

struct Blob {
    std::bitset<NODECOUNT> contains {};
    std::bitset<NODECOUNT> connects {};
    Blob() {};
    Blob(uint16_t idx, std::bitset<NODECOUNT> ct) {
        this->contains[idx] = 1;
        this->connects = ct; 
    }
};

struct Solver {
    std::array<std::bitset<NODECOUNT>, NODECOUNT> matrix;
    Solver(auto m) : matrix(m) {}

    Blob merge(const Blob& old, uint16_t idx) {
        Blob out = old;
        auto& mtx = matrix[idx];
        out.connects |= mtx; //merge connects
        out.contains[idx] = 1; //remove merged from contained
        out.connects &= ~out.contains; //removed contained from connects
        return out;
    }
    std::unordered_set<std::bitset<NODECOUNT>> seen;
    int count = 0;
    std::optional<std::bitset<NODECOUNT>> solve(Blob& in) {
        count++;
        //if (count % 1000 == 0) {std::cout << count << '\n';}
        if (in.connects.count() == 3) {
            if (in.contains.count() + 3 == NODECOUNT) {return std::nullopt;}
            return in.contains;
        }
        if (in.contains.count() > NODECOUNT/2) {
            return std::nullopt;
        }
        for (int i = 0; i < in.contains.size(); i++) {
            if (in.connects[i] == 0) {continue;}
            Blob x = merge(in, i);
            if (seen.contains(x.contains)) {continue;}
            if (auto k = solve(x); k.has_value()) {return k;}
            seen.insert(x.contains);
            break;
        }
        return std::nullopt;
    }

};




int main() {
    std::ifstream input("input49.txt", std::ios::in);
    std::vector<Connection<std::string>> strconnects;
    std::unordered_map<std::string, uint16_t> strmodules;
    while (!input.eof()) {
        std::string line;
        std::getline(input, line);
        auto processed = process(line, ":");
        auto connects = process(processed[1], " ");
        strmodules.insert({processed[0], strmodules.size()});
        for (const auto& connect : connects) {
            strconnects.push_back({processed[0], connect});
            strconnects.push_back({connect, processed[0]});
            strmodules.insert({connect, strmodules.size()});
        }
    } input.close();
    std::array<std::bitset<NODECOUNT>, NODECOUNT> adjMatrix{};
    for (const auto& connect : strconnects) {
        adjMatrix[strmodules[connect.lhs]][strmodules[connect.rhs]] = 1;
    }

    // for (const auto& r : adjMatrix) {
    //     for (int x = 0; x < NODECOUNT; x++) {
    //         std::cout << int(r[x]);
    //     }
    //     std::cout << std::endl;
    // }

    int count = 0;
    Solver s(adjMatrix);
    std::optional<std::bitset<NODECOUNT>> k;
    for (uint16_t i = 0; i < NODECOUNT; i++) {
        Blob starting{i, adjMatrix[i]};
        k = s.solve(starting);
        std::cout << count << '\n';
        if (k.has_value()) {
            std::cout << uint64_t(k.value().count() * (NODECOUNT - k.value().count())) << std::endl; 
            std::cin >> count;
            
        }
    }

    return 0;
}