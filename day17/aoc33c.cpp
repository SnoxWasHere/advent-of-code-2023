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

int64_t hash(const Coord& c, const dir d, const uint8_t steps) {
    return ((c.hash() << 32) + (uint32_t(steps) << 16) + uint8_t(d));
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
    uint8_t streak = 1;
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
    std::priority_queue<Traveler, std::deque<Traveler>, std::greater<Traveler>> pq;
    std::unordered_set<int64_t> visited {hash(starting, none, 0)};
    pq.push({starting, 0, none, 1});
    while (true) {
        //printPQ(pq);
        Traveler current = pq.top();
        pq.pop();
        visited.insert(hash(current.pos, current.heading, current.streak));
        if (current.score > highest) {
            std::cout << highest << std::endl;
            highest += 20;
        }
        if (current.pos == finishing) {
            //printVec(grid, current.history);
            std::cout << current.score << std::endl;
            break;
        }
        for (const auto& [cdir, cardinal] : cardinals) {
            Coord pos = current.pos + cardinal;
            uint8_t steps = (cdir == current.heading) ? (current.streak + 1) : 1;
            if (!(steps > 3) && !outOfBounds(pos, GRIDSIZE) && !(visited.contains(hash(pos, cdir, steps))) && (backwards[current.heading] != cdir)) {
                pq.push(Traveler{pos, uint16_t(current.score + uint16_t(grid[pos.y][pos.x])), cdir, steps});
            }
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
    return 0;
}