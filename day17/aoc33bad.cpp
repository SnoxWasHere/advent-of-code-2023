#include <iostream>
#include <istream>
#include <fstream>
#include <vector>
#include <cstdint>
#include <map>
#include <unordered_set>
#include <queue>
#define array(x) std::vector<std::vector<x>>


enum dir : uint8_t {
    none,
    north,
    east,
    south,
    west
};

struct Coord {
    int16_t x = 0, y = 0;
    inline int64_t hash() const {return ((int32_t(x) << 16) +  y);}
    auto operator<=>(const Coord&) const = default;
    Coord operator+(const Coord& rhs) const {return {(this->x + rhs.x), (this->y + rhs.y)};} 
    Coord operator-(const Coord& rhs) const {return {(this->x - rhs.x), (this->y - rhs.y)};} 
};

int64_t hash(const Coord& c, const dir d) {
    return ((c.hash() << 32) + uint8_t(d));
}

inline bool outOfBounds(const Coord& c, const int s) {
    return (c.x < 0 || c.x >= s || c.y < 0 || c.y >= s);
}


std::map<dir, Coord> cardinals {
    {north, {0, -1}}, {east, {1, 0}}, {south, {0, 1}}, {west, {-1, 0}}
};
std::map<dir, dir> backwards{
    {north, south}, {south, north}, {east, west}, {west, east}, {none, none}
};
std::map<dir, std::pair<dir, dir>> turns{
    {north, {east,west}}, {south, {east,west}}, {east, {north,south}}, {west, {north,south}}
};

std::map<dir, char> dirChars {
    {north, '^'}, {east, '>'}, {south, 'v'}, {west, '<'}
};

void printVec(const array(uint8_t)& grid, const std::vector<dir>& directions) {
    std::map<Coord, dir> cdirs;
    Coord current {0, 0};
    for (const auto& d : directions) {
        current = current + cardinals[d];
        cdirs.insert({current, d});
    }
    
    for (int16_t y = 0; y < grid.size(); y++) {
        for (int16_t x = 0; x < grid[y].size(); x++) {
            char val = ('0' + grid[y][x]);
            if (cdirs.contains(Coord{x, y})) {
                val = dirChars[cdirs[Coord{x, y}]];
            }
            std::cout << val;
        } std::cout << '\n';
    } std::cout << std::endl;
}


struct Traveler {
    Coord pos;
    uint16_t score = 0;
    dir heading = none;
    //std::vector<dir> history {};
};
auto operator<=>(const Traveler& lhs, const Traveler& rhs) {
    return lhs.score <=> rhs.score;
}

template<typename T1, typename T2>
void printPQ(std::priority_queue<Traveler, T1, T2> q) {
    while (!q.empty() ) {
        auto& t = q.top();
        std::cout << t.score << " - ("  << t.pos.x << ", " << t.pos.y << ")\n";
        return;
        q.pop();
    } std::cout << std::endl;
}


void dijkstra(array(uint8_t) grid) {
    const int16_t GRIDSIZE = grid.size();
    Coord starting {0,0};
    Coord finishing {GRIDSIZE-1, GRIDSIZE-1};
    int highest = 0;
    std::priority_queue<Traveler, std::vector<Traveler>, std::greater<Traveler>> pq;
    std::unordered_set<int64_t> visited {hash(starting, none)};
    pq.push({starting, 0, none});
    while (true) {
        //printPQ(pq);
        Traveler current = pq.top();
        pq.pop();
        visited.insert(hash(current.pos, current.heading));
        if (current.score > highest) {
            std::cout << highest << std::endl;
            highest += 20;
        }
        if (current.pos == finishing) {
            //printVec(grid, current.history);
            std::cout << current.score << std::endl;
            break;
        }

        if (current.heading != none) {
            Coord cardinal = cardinals[current.heading];
            auto [left, right] = turns[current.heading];
            Coord lofs = cardinals[left];
            Coord rofs = cardinals[right];

            Coord pos = current.pos;
            uint16_t score = current.score;
            for (int s = 1; s <= 3; s++) {
                Coord lhs = pos + lofs;
                Coord rhs = pos + rofs;


                if (!outOfBounds(lhs, GRIDSIZE) && !(visited.contains(hash(lhs, right)))) {
                    Traveler child {lhs, score + grid[lhs.y][lhs.x], right};
                    pq.push(child);
                }
                if (!outOfBounds(rhs, GRIDSIZE) && !(visited.contains(hash(rhs, right)))) {
                    Traveler child {rhs, score + grid[rhs.y][rhs.x], right};
                    pq.push(child);
                }
                if (pos == finishing) {
                    Traveler child {pos, score + grid[pos.y][pos.x], current.heading};
                    pq.push(child);
                }

                pos = pos + cardinal;
                if (outOfBounds(pos, GRIDSIZE)) {
                    break;
                }
                score += grid[pos.y][pos.x];
            }
        } else {
            Coord r = current.pos + Coord{1, 0};
            Coord d = current.pos + Coord{0, 1};
            Traveler right {r, grid[r.y][r.x], east};
            Traveler down  {d, grid[d.y][d.x], south};
            pq.push(right);
            pq.push(down);
        }
    }
}

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

    dijkstra(vgrid);
}