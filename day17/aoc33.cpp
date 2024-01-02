#include <iostream>
#include <istream>
#include <fstream>
#include <vector>
#include <cstdint>
#include <map>
#include <set>
#include <utility>
#define array(x) std::vector<std::vector<x>>

inline bool outOfBounds(int x, int y, int w = 12, int h = 12) {
    return (x < 0 || x > w || y < 0 || y > h);
}

enum dir {
    none,
    north,
    east,
    south,
    west
};

std::map<dir, std::pair<int, int>> cardinals {
    {dir::north, {0,-1}},
    {dir::east,  {1, 0}},
    {dir::south, {0, 1}},
    {dir::west,  {-1,0}},
};

struct Node {
    int x, y;
    uint8_t weight;
    Node* north = nullptr;
    Node* east = nullptr;
    Node* west = nullptr;
    Node* south = nullptr;

    Node*& getChild(dir d) {
        switch (d) {
        case dir::north:
            return this->north;
        case dir::east:
            return this->east;
        case dir::south:
            return this->south;
        case dir::west:
            return this->west;
        default:
            std::unreachable();
        }
    }

    int h() {
        return (7*(12 - x) + 7*(12 - y));
    }
};

struct Traverser {
    std::vector<std::pair<Node*, dir>> history {};
    dir heading = dir::none;
    int g = 0;
    int streak = 0;
    
    Traverser(auto n) {
        history.push_back({n, dir::none});
    }
    Node*& getNode() {
        return this->history.back().first;
    }

    Traverser move(dir d) {
        Traverser output(*this);
        if (output.heading == d) {output.streak++;}
        else {output.heading = d; output.streak = 0;}
        output.history.push_back({output.getNode()->getChild(d), d});
        output.g += output.getNode()->weight;
        return output;
    }

    bool alreadyVisited(Node* n) {
        for(auto& [h, d] : history) {
            if ((h->x == n->x) && (h->y == n->y)) {return true;}
        }
        return false;
    }

    int f() {
        return (this->g + getNode()->h());
    }
};

class WeightedGrid {
private:
    array(uint8_t) grid;
    array(Node*) nodeGrid;
public:
    WeightedGrid(auto g) : grid(g) {
        for (int y = 0; y < grid.size(); y++) {
            nodeGrid.emplace_back();
            for (int x = 0; x < grid[y].size(); x++) {
                Node* n = new Node;
                n->weight = grid[y][x];
                n->x = x;
                n->y = y;
                nodeGrid[y].push_back(n);
            }
        }
        for (int y = 0; y < grid.size(); y++) {
            for (int x = 0; x < grid[y].size(); x++) {
                Node* n = nodeGrid[y][x];
                for (const auto& cardinal : cardinals) {
                    const auto& [cx, cy] = cardinal.second;
                    if (!outOfBounds(x+cx, y+cy)) {
                        Node*& child = n->getChild(cardinal.first);
                        child = nodeGrid[y+cy][x+cx];
                    }
                }
                
            }
        }
    }

    int AStar() {
        Traverser starting(nodeGrid[0][0]);
        std::map<int, Traverser> discovered {{starting.f(), starting}};
        std::set<uint32_t> scores {UINT32_MAX};

        while (discovered.size() != 0) {
            // for (auto& [f, t] : discovered) {
            //     std::cout << f << " - " << t.getNode()->x << "," << t.getNode()->y << std::endl; 
            // }
            // std::cout << std::endl;

            auto current = discovered.begin()->second;
            discovered.erase(discovered.begin());

            if (current.g >= *scores.begin()) {continue;}
            if ((current.getNode()->x == 12) && (current.getNode()->y == 12)) {
                std::ofstream out("output.txt");
                for (int y = 0; y < grid.size(); y++) {
                    for (int x = 0; x < grid[y].size(); x++) {
                        dir f = dir::none;
                        for(auto& [h, d] : current.history) {
                            if (h->x == x && h->y == y) {f = d; break;}
                        }
                        out << (char)(f + '0');
                    }
                    out << std::endl;
                }
                out.close();
                scores.insert(current.g);
                continue;
            }
            for (const auto& cardinal : cardinals) {
                const auto& [cx, cy] = cardinal.second;
                if (!outOfBounds(current.getNode()->x+cx, current.getNode()->y+cy)) {
                    Traverser found = current.move(cardinal.first);
                    if (!(found.streak >= 3) && !(current.alreadyVisited(found.getNode()))) {discovered.insert({found.f(), found});}
                }
            }
        }

        return 0;
    }

};

int main() {
    std::ifstream input("input33.txt", std::ios::in);
    array(uint8_t) grid;
    
    while (!input.eof()) {
        std::string line;
        std::getline(input, line); 
        grid.emplace_back();
        for (auto& c : line) {
            grid.back().push_back((c - '0'));
        }
    } input.close();
    WeightedGrid w(grid);
    int i = w.AStar();
    return 0;
}