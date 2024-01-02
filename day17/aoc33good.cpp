#include <iostream>
#include <istream>
#include <fstream>
#include <vector>
#include <cstdint>
#include <map>
#include <unordered_map>
#include <unordered_set>
#include <queue>
#include <bit>
#define array(x) std::vector<std::vector<x>>


struct Coord {
    int16_t x = 0, y = 0;
    inline uint32_t hash() const {return std::bit_cast<uint32_t>((int32_t(x) << 16) +  y);}
    auto operator<=>(const Coord&) const = default;
    Coord operator+(const Coord& rhs) const {return {(this->x + rhs.x), (this->y + rhs.y)};} 
    Coord operator-(const Coord& rhs) const {return {(this->x - rhs.x), (this->y - rhs.y)};} 
};

struct Traveler {
    Coord pos;
    Coord dir;
    uint16_t score;
    int16_t streak;
    //std::vector<dir> history {};
};
auto operator<=>(const Traveler& lhs, const Traveler& rhs) {
    return lhs.score <=> rhs.score;
}
bool operator==(const Traveler& lhs, const Traveler& rhs) { 
    return std::tuple(lhs.pos, lhs.dir, lhs.streak) == std::tuple(rhs.pos, rhs.dir, rhs.streak); 
}

struct TravelerHash {
    size_t operator()(const Traveler& move) const {
        auto cantor = [](size_t a, size_t b){ return (a + b + 1) * (a + b) / 2 + b; };
        return cantor(move.pos.x, cantor(move.pos.y, cantor(move.dir.x, cantor(move.dir.y, move.streak))));
    }
};

Coord rotateCW(const Coord& dir) { 
    return { -dir.y, dir.x }; 
}
Coord rotateCCW(const Coord& dir) { 
    return { dir.y, -dir.x }; 
}


class Grid {
private:
    array(uint8_t) vgrid;
    int16_t bound;
    std::priority_queue<Traveler, std::vector<Traveler>, std::greater<Traveler>> pq;
    std::unordered_set<Traveler, TravelerHash> seen;
    Coord finishing;
    
    uint8_t at(const Coord& c) const {
        return vgrid[c.y][c.x];
    }
    inline bool outOfBounds(const Coord& c) const {
        return (c.x < 0 || c.x >= bound || c.y < 0 || c.y >= bound);
    }
    void insertOptions(const Traveler& in) {
        //insert left
        if (in.streak >= 3) {
            Traveler trav;
            trav.dir = rotateCCW(in.dir);
            trav.pos = in.pos + trav.dir;
            if (!outOfBounds(trav.pos)) {
                trav.score = in.score + at(trav.pos);
                trav.streak = 0;
                pq.push(trav);
            }
        } if (in.streak >= 3) {
            Traveler trav;
            trav.dir = rotateCW(in.dir);
            trav.pos = in.pos + trav.dir;
            if (!outOfBounds(trav.pos)) {
                trav.score = in.score + at(trav.pos);
                trav.streak = 0;
                pq.push(trav);
            }
        } if (in.streak < 9) {
            Traveler trav;
            trav.dir = in.dir;
            trav.pos = in.pos + trav.dir;
            if (!outOfBounds(trav.pos)) {
                trav.score = in.score + at(trav.pos);
                trav.streak = in.streak + 1;
                pq.push(trav);
            }
        }
    }
public:
    Grid(const array(uint8_t)& g) : vgrid(g), bound(g.size()) {
        pq.push(Traveler{{0, 0}, {0, 1}, 0, -1});
        pq.push(Traveler{{0, 0}, {1, 0}, 0, -1});
        finishing = {int16_t(bound-1), int16_t(bound-1)};
    }
    uint16_t dijkstra() {
        int highest = 0;
        while (true) {
            Traveler current = pq.top();
            pq.pop();
            if (seen.contains(current)) {continue;}
            seen.insert(current);
            //std::cout << current.pos.x << ", " << current.pos.y << " | " << current.score << '\n';
            if ((current.pos == finishing) && (current.streak >= 3)) {
                return current.score;
            }
            if (current.score > highest) {
                std::cout << highest << std::endl;
                highest += 20;
            }
            insertOptions(current); 
        }
    }

};

int main() {
    std::ifstream input("input33.txt", std::ios::in);
    array(uint8_t) vgrid;
    
    while (!input.eof()) {
        std::string line;
        std::getline(input, line); 
        vgrid.emplace_back();
        for (auto& c : line) {
            vgrid.back().push_back((c - '0'));
        }
    } input.close();

    Grid g(vgrid);
    std::cout << g.dijkstra() << std::endl;
    return 0;
}