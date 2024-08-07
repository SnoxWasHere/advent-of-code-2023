#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <cstdint>
#include <set>
#include <optional>
#include <unordered_set>

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

struct Module {
    std::set<Module*> contains;
    std::set<Module*> connects;
};
Module operator+(const Module& lhs, const Module& rhs) {
    Module out = lhs;
    out.contains.insert(rhs.contains.begin(), rhs.contains.end());
    out.connects.insert(rhs.connects.begin(), rhs.connects.end());
    for (const auto& cnt : out.contains) {
        out.connects.erase(cnt);
    }
    return out;
}

std::size_t hash(std::set<Module*> const& set) {
    std::size_t seed = set.size();
    for(auto& i : set) {
        seed ^= uint64_t(i) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
    }
    return seed;
}


struct Solver {
    std::set<std::size_t> seen;
    Solver() {};
    std::optional<std::set<Module*>> solve(Module& in, const uint16_t& count) {
        if (in.connects.size() == 3) {
            if (in.contains.size() + 3 == count) {return std::nullopt;}
            return in.contains;
        }
        auto cnt = (std::next(in.contains.begin(), rand() % in.contains.size()));
        Module x = in + **cnt;
        auto hashed = hash(x.contains);
        while (this->seen.count(hashed) > 0) {
            cnt = (std::next(in.contains.begin(), rand() % in.contains.size()));
            x = in + **cnt;
            hashed = hash(x.contains);
        }
        if (auto k = solve(x, count); k.has_value()) {return k;}
        seen.insert(hashed);
        
        return std::nullopt;
    }
};



int main() {
    std::ifstream input("input49.txt", std::ios::in);
    std::vector<Connection<std::string>> strconnects;
    std::map<std::string, Module*> strmodules;
    while (!input.eof()) {
        std::string line;
        std::getline(input, line);
        auto processed = process(line, ":");
        auto connects = process(processed[1], " ");
        strmodules.insert({processed[0], nullptr});
        for (const auto& connect : connects) {
            strconnects.push_back({processed[0], connect});
            //strconnects.push_back({connect, processed[0]});
            strmodules.insert({connect, nullptr});
        }
    } input.close();

    std::ofstream out("out.txt");
    for (const auto& c : strconnects) {
        out << c.lhs << " " << c.rhs << "\n";
    } out.flush(); out.close();
    

    return 0;
}