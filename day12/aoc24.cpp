#include <iostream>
#include <istream>
#include <fstream>
#include <vector>
#include <cstdint>
#include <set>
#include <map>

//working is on the left
#define WORKING true
//broken is on the right
#define BROKEN false

#define W true, false 
#define B false, true
#define U true, true

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

struct Node {
    bool condition;
    uint8_t bCount;
    uint8_t idx;
    uint64_t copies;
    Node* wrk;
    Node* bke;

    Node(bool cond, uint8_t b = 0, uint8_t i = 0, uint64_t c = 1) { 
        this->condition = cond; 
        this->bCount = (cond == BROKEN) ? (b+1) : 0;
        this->idx = i;
        this->copies = c;
        this->wrk = nullptr; 
        this->bke = nullptr; 
    }
    void createChildren(bool isWorking, bool isBroken) {
        if (isWorking) {
            uint8_t val = ((condition == BROKEN) ? idx+1 : idx);
            this->wrk = new Node(WORKING, bCount, val, copies);
        }
        if (isBroken) {
            this->bke = new Node(BROKEN, bCount, idx, copies);
        }
    }

    uint16_t hash() {
        return (this->bCount + (this->idx << 8) + (this->condition << 15));
    }

    ~Node() {}
};


class Tree {
private:
    std::vector<std::vector<uint8_t>> sequence;
    std::vector<int> fields;
    Node* root;

    bool kill(Node* node, Node* parent) {
        auto idx = node->idx;
        if (node->condition == BROKEN) {
            if (node->bCount > fields[idx]) {
                return true;
            } else {
                return false;
            }
        }
        else if (node->condition == WORKING) {
            if ((idx > 0) && (parent->condition == BROKEN) && (parent->bCount < fields[idx-1])) {
                return true;
            } else {
                return false;
            }
        }
        return false;
    }

public:
    Tree(auto s, auto f) : sequence(s), fields(f) {
        root = new Node(WORKING);
        sequence.push_back({W});
    }

    uint64_t populate() {
        std::vector<Node*> layer {root};
        for (int i = 0; i < sequence.size(); i++) {
            std::map<uint16_t, Node*> hashes;

            bool working = sequence[i][0];
            bool broken = sequence[i][1];

            for (auto& node : layer) {
                node->createChildren(working, broken);
                if (working) {
                    auto hash = node->wrk->hash();
                    if (!kill(node->wrk, node)) {
                        if (hashes.count(hash) == 0) {
                            hashes.insert({hash, node->wrk});
                        } else {
                            hashes[hash]->copies += node->wrk->copies;
                        }
                    } else {
                        delete node->wrk;
                    }
                }
                if (broken) {
                    auto hash = node->bke->hash();
                    if (!kill(node->bke, node)) {
                        if (hashes.count(hash) == 0) {
                            hashes.insert({hash, node->bke});
                        } else {
                            hashes[hash]->copies += node->bke->copies;
                        }
                    } else {
                        delete node->bke;
                    }
                }
            }
            for (auto& node : layer) {
                delete node;
            }
            layer = {};
            for (auto& [hash, node] : hashes) {
                layer.push_back(node);
            }
        }
        uint64_t count = 0;
        for (auto& node : layer) {
            if (node->idx == fields.size()) {
                count += node->copies;
            }
        }
        return count;
    }

    ~Tree() {
        //delete root;
    }
};

template <typename T> 
std::vector<T> copyFiveTimes (std::vector<T> input) {
    std::vector<T> output = input;
    for (int i = 0; i < 4; i++) {
        output.insert(output.end(), input.begin(), input.end());
    }
    return output;
}

template <typename T> 
std::vector<T> copyFiveTimes (std::vector<T> input, T divider) {
    std::vector<T> output {};
    for (int i = 0; i < 5; i++) {
        output.insert(output.end(), input.begin(), input.end());
        if(i != 4) {output.push_back(divider);}
    }
    return output;
}



int main() {
    std::ifstream input("input23.txt", std::ios::in);
    std::vector<std::vector<std::vector<uint8_t>>> sequences;
    std::vector<std::vector<int>> fields;

    while (!input.eof()) {
        std::string line;
        std::getline(input, line);
        auto parts = process(line, " ");

        std::vector<std::vector<uint8_t>> s;
        for (int i = 0; i < parts[0].length(); i++) {
            switch (parts[0][i]) {
            case '.':
                s.push_back({W});
                break;
            case '#':
                s.push_back({B});
                break;
            case '?':
                s.push_back({U});
                break;
            default:
                break;
            }
        }
        //sequences.push_back(s);
        sequences.push_back(copyFiveTimes(s, {U}));

        std::vector<int> f;
        auto fvals = process(parts[1], ",");
        for (auto& fval : fvals) {
            f.push_back(std::stoi(fval));
        }

        //fields.push_back(f);
        fields.push_back(copyFiveTimes(f));


    } input.close();

    uint64_t count = 0;
    for (int i = 0; i < sequences.size(); i++) {
        Tree* t = new Tree(sequences[i], fields[i]);
        count += t->populate();
        //std::cout << count << std::endl;
        delete t;
    }
    
    std::cout << count << std::endl; 
    return 0;
}

