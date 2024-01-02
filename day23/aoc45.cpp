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
#include <cassert>
#define array(x) std::vector<std::vector<x>>

#define SIZE 141
#define NODECOUNT 9414
#define JUNCTIONCOUNT 36

struct Coord {
    int16_t x, y;
    auto operator<=>(const Coord&) const = default;
    Coord operator+(const Coord& rhs) const {return {(this->x + rhs.x), (this->y + rhs.y)};} 
    Coord operator-(const Coord& rhs) const {return {(this->x - rhs.x), (this->y - rhs.y)};} 
};

template <>
struct std::hash<Coord> {
    std::size_t operator()(const Coord& c) const noexcept {
        return c.x + (int32_t(c.y) << 16);
    }
};

enum dir : uint8_t {
    north,
    east,
    south,
    west
};
std::map<dir, Coord> cardinals {
    {north, {0, -1}}, {east, {1, 0}}, {south, {0, 1}}, {west, {-1, 0}}
};


struct Grid {
private:
    array(char) grid;
    std::set<std::pair<char, dir>> valid {
        {'>', east}, {'v', south}
    };
    inline bool outOfBounds(const Coord& c) const {
        return (c.x < 0 || c.x >= SIZE || c.y < 0 || c.y >= SIZE);
    }
public:
    Grid(auto g) : grid(g) {};
    std::unordered_map<Coord, uint32_t> setIndexes() {
        std::unordered_map<Coord, uint32_t> out;
        uint32_t count = 0;
        for (int16_t y = 0; y < SIZE; y++) {
            for (int16_t x = 0; x < SIZE; x++) {
                if (grid[y][x] != '#') {
                    out.insert({Coord{x, y}, count});
                    count++;
                }
            }
        }
        return out;
    }
    std::vector<Coord> getNeighbors(const Coord& c) {
        std::vector<Coord> out;
        for (const auto& [d, card] : cardinals) {
            Coord pos = c + card;
            if (!outOfBounds(pos)) {
                if (char k = grid[pos.y][pos.x]; ((k == '.') || valid.contains({k, d}))) {
                    if (char o = grid[c.y][c.x]; (o != '.') && !valid.contains({o, d})) {continue;}
                    // '>' cannot go left, 'v' cannot go up
                    out.push_back(pos);
                }
            }
        }
        return out;
    }
};

struct MrMorale {
    const std::array<std::array<uint16_t, JUNCTIONCOUNT>, JUNCTIONCOUNT>& matrix;
    MrMorale(const auto& m) : matrix(m) {}
    uint32_t bigStepper(uint32_t jdx, uint32_t score, std::set<uint32_t>& seen) const {
        if (jdx == 1) {return score;}
        std::set<uint32_t> scores;
        const auto& row = matrix[jdx];
        for (uint16_t i = 0; i < row.size(); i++) {
            if (row[i] != 0) { //add next
                if (seen.contains(i)) {break;}
                auto nseen = seen;
                nseen.insert(i);
                scores.insert(bigStepper(i, score + row[i], seen));
            }
        }
        return (*scores.rbegin());
    }
};

int main() {
    std::ifstream input("input45.txt", std::ios::in);
    std::unordered_map<Coord, std::vector<Coord>> connects;
    std::unordered_map<Coord, uint32_t> indexes;
    std::unordered_map<uint32_t, Coord> revindexes;
    array(char) vgrid;

    while (!input.eof()) {
        std::string line;
        std::getline(input, line); 
        vgrid.emplace_back();
        for (auto& c : line) {
            vgrid.back().push_back(c);
        }
    } input.close();
    Grid g(vgrid);
    indexes = g.setIndexes();
    for (const auto& [lhs, rhs] : indexes) {
        revindexes.insert({rhs, lhs});
    }
    for (const auto& [c, idx] : indexes) {
        connects.insert({c, g.getNeighbors(c)});
    }

    std::cout << indexes.size() << std::endl;
    assert(indexes.size() == NODECOUNT);

    std::array<std::bitset<NODECOUNT>, NODECOUNT> bigmatrix {};
    for (const auto& [c, neighbors] : connects) {
        uint32_t lidx = indexes[c];
        for (const auto& neighbor : neighbors) {
            bigmatrix[lidx][indexes[neighbor]] = 1;
        }
    }
    
    std::unordered_map<uint32_t, uint32_t> jndexes;
    jndexes.insert({indexes[{1,0}], 0});
    jndexes.insert({indexes[{SIZE-2,SIZE-1}], 1});
    uint32_t junctcount = 2;
    for (uint32_t i = 0; i < NODECOUNT; i++) {
        std::bitset<NODECOUNT> column;
        for (uint32_t y = 0; y < NODECOUNT; y++) {
            column[y] = bigmatrix[y][i];
        }
        const auto& row = bigmatrix[i];
        if (((row | column).count()) > 2) {
            jndexes.insert({i, junctcount});
            junctcount++;
        }
    }
    std::cout << junctcount << std::endl;
    assert(junctcount == JUNCTIONCOUNT);
    std::array<std::array<uint16_t, JUNCTIONCOUNT>, JUNCTIONCOUNT> smallmatrix {};

    for (const auto& [idx, jdx] : jndexes) {
        //std::cout << revindexes[idx].x << ',' << revindexes[idx].y << '\n';
        //get neighbors and create vector
        const auto& jrow = bigmatrix[idx];
        std::vector<uint32_t> search;
        for (uint16_t i = 0; i < jrow.size(); i++) {
            if (jrow[i] == 1) {
                search.push_back(i);
                //std::cout << revindexes[i].x << ',' << revindexes[i].y << '\n';
            }
            
        }
        for (const auto& cidx : search) {
            uint32_t sidx = cidx;
            uint32_t pdx = idx;
            uint32_t dist = 1;
            while (true) {
                //std::cout << revindexes[sidx].x << ',' << revindexes[sidx].y << '\n';
                //std::cout << revindexes[pdx].x << ',' << revindexes[pdx].y << '\n';
                if (jndexes.contains(sidx)) {
                    smallmatrix[jdx][jndexes[sidx]] = dist;
                    break; //update smallmatrix
                }
                std::bitset<NODECOUNT> subtract;
                subtract[pdx] = 1;
                auto row = bigmatrix[sidx];
                row &= ~subtract;
                if (row.count() == 1) {
                    for (uint16_t i = 0; i < row.size(); i++) {
                        if (row[i] == 1) { //move to next
                            pdx = sidx; 
                            sidx = i; 
                            dist++;
                        }
                    }
                } else {
                    break; //dead end (not a junction, not a straight)
                }
            }
            
        }
    }

    MrMorale kdot(smallmatrix);
    std::set<uint32_t> iseen {0};
    std::cout << kdot.bigStepper(0, 0, iseen);

    return 0;
}