#include <iostream>
#include <istream>
#include <fstream>
#include <vector>
#include <cstdint>
#include <map>
#include <set>
#include <utility>
#include <queue>
#include <functional>
#include <unordered_set>
#define array(x) std::vector<std::vector<x>>
#define GRID_SIZE 4

enum dir {
    north,
    east,
    south,
    west
};


struct Coord {
    int16_t x = 0, y = 0;
    int32_t hash() {
        return x + (y << 8);
    }
};
Coord operator+(const Coord& lhs, const Coord& rhs) {
    Coord out;
    out.x += lhs.x + rhs.x;
    out.y += lhs.y + rhs.y;
    return out;
}

inline bool isBackwards(const dir my, const dir other) {
    return (dir)((my + 2) % 4) == other;
}
inline bool outOfBounds(Coord c, int w = GRID_SIZE, int h = GRID_SIZE) {
    return (c.x < 0 || c.x > w || c.y < 0 || c.y > h);
}

std::map<dir, Coord> cardinals {
    {dir::north, {0,-1}},
    {dir::east,  {1, 0}},
    {dir::south, {0, 1}},
    {dir::west,  {-1,0}},
};


struct Traveler {
    Coord pos;
    int cost = 0;
    dir heading = dir::north;
    int streak = 0;
};
auto operator<=>(const Traveler& lhs, const Traveler& rhs) {
    return lhs.cost <=> rhs.cost;
}

template <typename T>
class Grid {
private:
    array(T) grid;
public:
    Grid(array(T) g) : grid(g) {}
    inline T get(Coord c) const {return grid[c.y][c.x];} 
};

void dijkstra(const Grid<uint8_t>& grid) {
    std::unordered_set<int32_t> visited {};
    std::priority_queue<Traveler, std::vector<Traveler>, std::greater<Traveler>> pq_discovered;
    std::set<uint32_t> scores {UINT32_MAX};
    Traveler starting;
    pq_discovered.push(starting);

    while(!pq_discovered.empty()) {
        Traveler lowest = pq_discovered.top();
        std::cout << lowest.cost << " " << lowest.pos.x << "," << lowest.pos.y << std::endl;
        pq_discovered.pop();
        if (lowest.cost >= *scores.begin()) {continue;}
        if ((lowest.pos.x == GRID_SIZE) && (lowest.pos.y == GRID_SIZE)) {
            scores.insert(lowest.cost);
        }
        for (auto& [d, c] : cardinals) {
            Traveler child = lowest;
            child.pos = child.pos + c;
            if ((visited.count(child.pos.hash()) == 0) && !isBackwards(lowest.heading, d) && !outOfBounds(child.pos) && (child.streak < 3)) {
                child.cost += grid.get(child.pos);
                visited.insert(child.pos.hash());
                if (lowest.heading == d) {
                    child.streak += 1;
                } else {
                    child.heading = d;
                    child.streak = 0;
                }
                pq_discovered.push(child);
            }
        }
    }
    for (auto s : scores) {
        std::cout << s << std::endl;
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
    Grid grid(vgrid);
    dijkstra(grid);
    return 0;
}