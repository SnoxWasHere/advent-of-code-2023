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
#define UPDATE(D, sign, range) if (h.pos.D sign range.D) {range.D = h.pos.D;}
#define CONTAINS(cont, dim, val, label) if (cont.contains(h.val.dim)) {std::cout << label << o << " "; for (auto& v : cont[h.val.dim]) {std::cout << v << ", ";} std::cout << std::endl; cont[h.val.dim].push_back(o);} else {cont.insert({h.val.dim, {o}});}


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
    Coord(int64_t _x, int64_t _y, int64_t _z) : x(_x), y(_y), z(_z) {}
    Coord(int64_t n) : x(n), y(n), z(n) {}
    Coord(std::vector<std::string> in) {
        x = std::stoll(in[0]);
        y = std::stoll(in[1]);
        z = std::stoll(in[2]);
    }
    Coord operator<=>(const Coord& other) const = default;
};
Coord operator-(const Coord& lhs, const Coord& rhs) {
    return {lhs.x-rhs.x, lhs.y-rhs.y, lhs.z-rhs.z,};
}

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


template <typename T>
struct opt {
    std::optional<T> val;
    bool skippable;
};

opt<Coord> findIntersect(const Hailstone& lhs, const Hailstone& rhs) {
    const int64_t LEFTBOUND = 7;
    const int64_t RIGHTBOUND = 27;


    double A1 = -lhs.vel.y;
    double B1 =  lhs.vel.x;
    double C1 = A1*lhs.pos.x + B1*lhs.pos.y;

    double A2 = -rhs.vel.y;
    double B2 =  rhs.vel.x;
    double C2 = A2*rhs.pos.x + B2*rhs.pos.y;

    double determinate = A1*B2 - A2*B1;
    if (determinate == 0) {return {std::nullopt, 1};}
    double x = (B2*C1 - B1*C2) / determinate;
    //if (x < LEFTBOUND || x > RIGHTBOUND) {return std::nullopt;}
    if (signOf(x - lhs.pos.x) != signOf(lhs.vel.x)) {return {std::nullopt, 0};}
    if (signOf(x - rhs.pos.x) != signOf(rhs.vel.x)) {return {std::nullopt, 0};}
    
    double y = (A1*C2 - A2*C1) / determinate;
    //if (y < LEFTBOUND || y > RIGHTBOUND) {return std::nullopt;}

    return {std::optional<Coord>{Coord{x, y, 0}}, 0};
}





int main() {
    std::ifstream input ("input47.txt", std::ios::in);
    std::vector<Hailstone> hailstones;
    while (!input.eof()) {
        std::string line;
        std::getline(input, line);
        hailstones.push_back(Hailstone(process(line, "@")));
    } input.close();
    std::map<uint64_t, std::vector<uint16_t>> xvals;
    std::map<uint64_t, std::vector<uint16_t>> yvals;
    std::map<uint64_t, std::vector<uint16_t>> zvals;

    for (uint16_t o = 0; o < hailstones.size(); o++) {
        auto& h = hailstones[o];
        CONTAINS(xvals, x, pos, "px");
        CONTAINS(yvals, y, pos, "py");
        CONTAINS(zvals, z, pos, "pz");
        //CONTAINS(xvals, x, vel, "vx");
        //CONTAINS(yvals, y, vel, "vy");
        //CONTAINS(zvals, z, vel, "vz");
    }


    for (int o = 0; o < hailstones.size(); o++) {
        auto& h = hailstones[o];
    }

    return 0;
}