#include <iostream>
#include <istream>
#include <fstream>
#include <vector>
#include <cstdint>
#include <map>
#include <unordered_set>
#include <queue>
#include <bit>
#define array(x) std::vector<std::vector<x>>

enum dir : uint8_t {
    north,
    east,
    south,
    west
};

std::map<dir, std::pair<dir, dir>> turns{
    {north, {east,west}}, {south, {east,west}}, {east, {north,south}}, {west, {north,south}}
};
struct Coord {
    int16_t x = 0, y = 0;
    inline uint32_t hash() const {return std::bit_cast<uint32_t>((int32_t(x) << 16) +  y);}
    auto operator<=>(const Coord&) const = default;
    Coord operator+(const Coord& rhs) const {return {(this->x + rhs.x), (this->y + rhs.y)};} 
    Coord operator-(const Coord& rhs) const {return {(this->x - rhs.x), (this->y - rhs.y)};} 
};

std::map<dir, Coord> cardinals {
    {north, {0, -1}}, {east, {1, 0}}, {south, {0, 1}}, {west, {-1, 0}}
};


struct Traveler {
    Coord pos;
    uint16_t score = 0;
    dir heading;
    //std::vector<dir> history {};
    uint32_t hash() const {return (pos.hash() + ((uint32_t(heading)) << 14));}
};
auto operator<=>(const Traveler& lhs, const Traveler& rhs) {
    return lhs.score <=> rhs.score;
}
std::map<dir, char> dirChars {
    {north, '^'}, {east, '>'}, {south, 'v'}, {west, '<'}
};


class Grid {
private:
    array(uint8_t) vgrid;
    int16_t bound;
    std::priority_queue<Traveler, std::vector<Traveler>, std::greater<Traveler>> pq;
    std::unordered_set<uint32_t> seen;
    Coord finishing;
    
    uint8_t at(const Coord& c) const {
        return vgrid[c.y][c.x];
    }
    inline bool outOfBounds(const Coord& c) const {
        return (c.x < 0 || c.x >= bound || c.y < 0 || c.y >= bound);
    }
    void insertOptions(const Traveler& in) {
        const Coord card = cardinals[in.heading];
        std::pair<dir, dir> turn = turns[in.heading];
        Coord lhs = cardinals[turn.first];
        Coord rhs = cardinals[turn.second];
        Coord pos = in.pos;
        uint16_t score = in.score;
        for (int i = 0; i < 3; i++) {
            if (pos == finishing) {pq.push({pos, score, in.heading});}
            //lhs
            Coord resultant = pos + lhs;
            if (!outOfBounds(resultant)) {
                if (Traveler k = {resultant, uint16_t(score + at(resultant)), turn.first}; !(seen.contains(k.hash()))) {
                    pq.push(k);
                }
            }
            //rhs
            resultant = pos + rhs;
            if (!outOfBounds(resultant)) {
                if (Traveler k = {resultant, uint16_t(score + at(resultant)), turn.second}; !(seen.contains(k.hash()))) {
                    pq.push(k);
                }
            }
            pos = pos + card;
            if (outOfBounds(pos)) {break;}
            score += at(pos);
        }
    }
public:
    Grid(const array(uint8_t)& g) : vgrid(g), bound(g.size()) {
        pq.push({{0,1}, at({0,1}), south});
        pq.push({{1,0}, at({1,0}), east});
        finishing = {int16_t(bound-1), int16_t(bound-1)};
    }
    uint16_t dijkstra() {
        int highest = 0;
        while (true) {
            Traveler current = pq.top();
            pq.pop();
            seen.insert(current.hash());
            //std::cout << current.pos.x << ", " << current.pos.y << " - " << dirChars[current.heading] << " | " << current.score << '\n';
            if (current.pos == finishing) {
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