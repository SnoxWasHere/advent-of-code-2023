#include <iostream>
#include <istream>
#include <fstream>
#include <vector>
#include <cstdint>
#include <optional>
#include <queue>
#include <map>
#include <set>
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

struct Coord {
    int64_t x, y, z;
    Coord() {}
    Coord(std::vector<std::string> in) {
        x = std::stoll(in[0]);
        y = std::stoll(in[1]);
        z = std::stoll(in[2]);
    }
};

struct Hailstone {
    Coord pos;
    Coord vel;
    Hailstone(std::vector<std::string> in) {
        pos = Coord(process(in[0], ","));
        vel = Coord(process(in[1], ","));
    }
};

template <typename T>
inline int8_t signOf(T var) {
    return ((var > 0) ? (1) : (-1));
}
//pxr (vy0 - vy1) + pyr (vx1 - vx0) + vxr (py1 - py0) + vyr (px0 - px1) - px0 vy0 + py0 vx0 + px1 vy1 - py1 vx1 = 0
void createEquation (Hailstone lhs, Hailstone rhs) {
    std::cout << "a*(" << (lhs.vel.y - rhs.vel.y) << ") + b*(" << (rhs.vel.x - lhs.vel.x) 
            << ") + c*(" << (rhs.pos.y - lhs.pos.y) << ") + d*(" << (lhs.pos.x - rhs.pos.x)
            << ") - (" << (lhs.pos.x*lhs.vel.y) << ") + (" << (lhs.pos.y*lhs.vel.x + rhs.pos.x*rhs.vel.y - rhs.pos.y*rhs.vel.x) << ") = 0\n";
}


bool findIntersect(const Hailstone& lhs, const Hailstone& rhs) {
    const int64_t LEFTBOUND = 7;
    const int64_t RIGHTBOUND = 27;

    double A1 = -lhs.vel.y;
    double B1 =  lhs.vel.x;
    double C1 = A1*lhs.pos.x + B1*lhs.pos.y;

    double A2 = -rhs.vel.y;
    double B2 =  rhs.vel.x;
    double C2 = A2*rhs.pos.x + B2*rhs.pos.y;

    double determinate = A1*B2 - A2*B1;
    
    double x = (B2*C1 - B1*C2) / determinate;
    //if (x < LEFTBOUND || x > RIGHTBOUND) {return false;}
    if (signOf(x - lhs.pos.x) != signOf(lhs.vel.x)) {return false;}
    if (signOf(x - rhs.pos.x) != signOf(rhs.vel.x)) {return false;}
    
    double y = (A1*C2 - A2*C1) / determinate;
    //if (y < LEFTBOUND || y > RIGHTBOUND) {return false;}

    return true;
}



int main() {
    std::ifstream input ("input47.txt", std::ios::in);
    std::vector<Hailstone> hailstones;
    while (!input.eof()) {
        std::string line;
        std::getline(input, line);
        hailstones.push_back(Hailstone(process(line, "@")));
    } input.close();

    uint64_t count = 0;
    for (int o = 0; o < hailstones.size(); o++) {
        for (int i = o + 1; i < hailstones.size(); i++) {
            const auto& lhs = hailstones[o];
            const auto& rhs = hailstones[i];
            std::cout << o << "&" << i << " - " << (findIntersect(lhs, rhs)) << std::endl;
        }
    }
    std::cout << count << std::endl;
    createEquation(hailstones[0], hailstones[1]);
    createEquation(hailstones[0], hailstones[2]);
    createEquation(hailstones[1], hailstones[2]);
    createEquation(hailstones[1], hailstones[3]);
    createEquation(hailstones[2], hailstones[3]);
    
    return 0;
}