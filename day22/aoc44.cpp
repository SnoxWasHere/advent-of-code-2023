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
#define array(x)

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
    uint16_t x = 0, y = 0, z = 0;
    Coord() {}
    Coord(int _x, int _y, int _z) {
        x = _x;
        y = _y;
        z = _z;
    }
    Coord(const std::vector<std::string>& l) {
        x = std::stoi(l[0]); 
        y = std::stoi(l[1]);
        z = std::stoi(l[2]);
    }
    uint64_t hash() const {
        return (x + (uint64_t(y) << 16) + (uint64_t(z) << 32));
    }
};
Coord operator-(const Coord& lhs, const Coord& rhs) {
    Coord x = {lhs.x-rhs.x, lhs.y-rhs.y, lhs.z-rhs.z};
    return x;
}

struct Block {
    uint16_t name;
    std::vector<Coord> cubes;
    Block(uint16_t count, Coord lhs, Coord rhs) {
        name = count;
        Coord difference = rhs - lhs;
        uint16_t* start = &lhs.x;
        uint16_t* end = &rhs.x;
        if (difference.y > 0) {
            start = &lhs.y;
            end = &rhs.y;
        } else if (difference.z > 0){
            start = &lhs.z;
            end = &rhs.z;
        }
        do {
            cubes.push_back(lhs);
            (*start)++;
        } while (*start <= *end);
        
    }
    Block fall() const {
        Block out = *this;
        for (auto& c : out.cubes) {
            c.z--;
        }
        return out;
    }
};
auto operator<=>(const Block& lhs, const Block& rhs) {
    return lhs.cubes.front().z <=> rhs.cubes.front().z;
}


void print(std::priority_queue<Block, std::vector<Block>, std::greater<Block>> pq)
{
    while (!pq.empty()) {
        std::cout << char('A' + pq.top().name) << " ";
        pq.pop();
    }
    std::cout << std::endl;
}

void updateFilled(std::map<uint64_t, uint16_t>& filledVoxels, const Block& in, const std::vector<Coord>& old = {}) {
    for (auto& g : old) {
        filledVoxels.erase(g.hash());
    }
    for (auto& g : in.cubes) {
        filledVoxels.insert({g.hash(), in.name});
    }
}

void printVoxels(std::vector<Block> blocks) {
    std::ofstream output("out.txt");
    //19,19,17,#8A9CB0,true;
    for (auto& block : blocks) {
        for (auto& cube : block.cubes) {
            output << cube.x << ","
                << cube.z << ","
                << cube.y << ",#"
                << std::hex << (((uint64_t(block.name) * 3452371) + 0x100000) % 0xFFFFFF) << ",true;";
        }
    } output.close();
}

int main() {
    std::ifstream input("input43.txt", std::ios::in);
    std::priority_queue<Block, std::vector<Block>, std::greater<Block>> blocks;
    std::map<uint64_t, uint16_t> filledVoxels;
    std::map<uint16_t, std::set<uint16_t>> nameCarry; //name of blocks each name is being carried by
    std::map<uint16_t, bool> removable;
    uint16_t count = 0;
    while (!input.eof()) {
        std::string line;
        std::getline(input, line);
        auto prc = process(line, "~");
        Coord lhs(process(prc[0], ","));
        Coord rhs(process(prc[1], ","));
        Coord backwards = rhs - lhs;
        if ((backwards.x > 300) || (backwards.y > 300) || (backwards.z > 300)) {
            std::cout << "HELP";
        }
        Block out {count, lhs, rhs};
        blocks.push(out);
        updateFilled(filledVoxels, out);
        nameCarry.insert({count, {}});
        removable.insert({count, true});
        count++;
    } input.close();

    std::priority_queue<Block, std::vector<Block>, std::greater<Block>> updated;
    while (blocks.size() > 0) {
        Block current = blocks.top();
        blocks.pop();

        while (current.cubes.front().z > 1) {
            Block test = current.fall();
            bool legal = true;
            for (auto& t : test.cubes) {
                if ((filledVoxels.count(t.hash()) > 0) && (filledVoxels[t.hash()] != test.name)) {
                    legal = false;
                    uint16_t name = filledVoxels[t.hash()];
                    nameCarry[test.name].insert(name);
                }
            }
            if (!legal) {break;}
            else {
                updateFilled(filledVoxels, test, current.cubes);
                current = test;
            }
        }
        updated.push(current);
    }

    for (const auto& [name, carriedBy] : nameCarry) {
        if (carriedBy.size() == 1) {
            removable[*carriedBy.begin()] = false;
        }
    }
    std::vector<uint16_t> names;
    for (const auto& [name, remove] : removable) {
        if (!remove) {names.push_back(name);}
    }
    
    uint64_t total = 0;
    for (const auto& name : names) {
        auto currentVoxels = filledVoxels;
        auto currentBlocks = updated;
        std::set<uint16_t> fallenNames;
        std::priority_queue<Block, std::vector<Block>, std::greater<Block>> updated;
        while (currentBlocks.size() > 0) {
            Block current = currentBlocks.top();
            currentBlocks.pop();

            while (current.cubes.front().z > 1) {
                Block test = current.fall();
                bool legal = true;
                for (auto& t : test.cubes) {
                    if ((currentVoxels.count(t.hash()) > 0) && ((currentVoxels[t.hash()] != test.name) && (currentVoxels[t.hash()] != name))) {
                        legal = false;
                        uint16_t name = currentVoxels[t.hash()];
                        nameCarry[test.name].insert(name);
                    }
                }
                if (!legal) {break;}
                else {
                    updateFilled(currentVoxels, test, current.cubes);
                    current = test;
                    fallenNames.insert(current.name);
                }
            }
            updated.push(current);
        }
        total += fallenNames.size();
    }

    std::cout << total << std::endl;
    return 0;
}