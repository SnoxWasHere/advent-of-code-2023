#include <iostream>
#include <istream>
#include <fstream>
#include <vector>
#include <cstdint>
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
    uint16_t bCount;
    Node* wrk;
    Node* bke;
    Node* parent;

    Node(bool cond, int b = 0, Node* p = nullptr) { 
        this->condition = cond; 
        bCount = (cond == BROKEN) ? (b+1) : 0;
        wrk = nullptr; 
        bke = nullptr; 
        parent = p;
    }
    void createChildren(bool isWorking, bool isBroken) {
        if (isWorking) {
            this->wrk = new Node(WORKING, bCount, this);
        }
        if (isBroken) {
            this->bke = new Node(BROKEN, bCount, this);
        }
    }

    ~Node() {
        if (this->wrk != nullptr) {delete this->wrk;}
        if (this->bke != nullptr) {delete this->bke;}
    }
};

class Tree {
private:
    std::vector<std::vector<uint8_t>> sequence;
    std::vector<int> fields;

    Node* root;
    
    uint8_t findIndex(Node* n) {
        Node* node = n;
        uint8_t count = 0;
        while (node->parent != nullptr) {
            if ((node->condition == WORKING) && (node->parent->condition == BROKEN)) {count++;}
            node = node->parent;
        }
        return count;
    }

    void printLineage(Node* n) {
        Node* node = n;
        while (node->parent != nullptr) {
            std::cout << ((node->condition == WORKING) ? 'W' : 'B');
            node = node->parent;
        }
        std::cout << std::endl;
    }

    bool kill(Node* node) {
        auto idx = findIndex(node);
        if (node->condition == BROKEN) {
            if (node->bCount > fields[idx]) {
                return true;
            } else {
                return false;
            }
        }
        else if (node->condition == WORKING) {
            if ((idx > 0) && (node->parent->condition == BROKEN) && (node->parent->bCount < fields[idx-1])) {
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

    int populate() {
        std::vector<Node*> layer {root};
        
        for (int i = 0; i < sequence.size(); i++) {
            std::vector<Node*> newLayer;
            bool working = sequence[i][0];
            bool broken = sequence[i][1];

            for (auto& node : layer) {
                node->createChildren(working, broken);
                if (working) {
                    if (!kill(node->wrk)) {
                        newLayer.push_back(node->wrk);
                    }
                }
                if (broken) {
                    if (!kill(node->bke)) {
                        newLayer.push_back(node->bke);
                    } 
                }
            }
            layer = newLayer;
        }
        int count = 0;
        for (auto& node : layer) {
            if (findIndex(node) == fields.size()) {
                count++;
            }
        }
        return count;
    }

    void print(const std::string& prefix, Node* node, bool working) {
        if( node != nullptr ) {
            std::cout << prefix;
            std::cout << (working ? "├──" : "└──" );
            // print the value of the node
            std::cout << ((node->condition == WORKING) ? 'W' : 'B') << std::endl;
            // enter the next tree level - left and right branch
            print( prefix + (working ? "│  " : "   "), node->wrk, true);
            print( prefix + (working ? "│  " : "   "), node->bke, false);
        }
    }
    void print() {
        print("", root, false);    
    }

    ~Tree() {
        delete root;
    }
};



int main() {
    std::ifstream input("input23.txt", std::ios::in);
    std::vector<std::vector<std::vector<uint8_t>>> sequences;
    std::vector<std::vector<int>> fields;

    while (!input.eof()) {
        std::string line;
        std::getline(input, line);
        auto parts = process(line, " ");

        sequences.emplace_back();
        for (int i = 0; i < parts[0].length(); i++) {
            switch (parts[0][i]) {
            case '.':
                sequences.back().push_back({W});
                break;
            case '#':
                sequences.back().push_back({B});
                break;
            case '?':
                sequences.back().push_back({U});
                break;
            default:
                break;
            }
        }

        fields.emplace_back();
        auto fvals = process(parts[1], ",");
        for (auto& fval : fvals) {
            fields.back().push_back(std::stoi(fval));
        }

    } input.close();

    int count = 0;
    for (int i = 0; i < sequences.size(); i++) {
        Tree t(sequences[i], fields[i]);
        count += t.populate();
    }
    
    std::cout << count << std::endl; 
    return 0;
}

