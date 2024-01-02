#include <iostream>
#include <istream>
#include <fstream>
#include <vector>
#include <cstdint>
#include <optional>
#include <queue>
#include <map>
#include <utility>

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

enum category {
    x, m, a, s
};
std::map<char, category> catmap {
    {'x', x}, {'m', m}, {'a', a}, {'s', s}
};
enum sign {
    greater, less
};
std::map<char, sign> signmap {
    {'>', greater}, {'<', less}
};
enum status {
    processing, accepted, rejected
};

struct Condition { //defaults to always true
    category r = x;
    sign s = greater; //r greater/less than val
    int val = -1;
};

template <typename T>
bool testSign(T lhs, T rhs, sign s) {
    if (s == greater) {
        return lhs > rhs;
    } else {
        return lhs < rhs;
    }
}

template <typename T>
bool testBoth(T lhs1, T lhs2, T rhs, sign s) {
    return (testSign(lhs1, rhs, s)) && (testSign(lhs2, rhs, s));
}

template <typename T>
bool testNeither(T lhs1, T lhs2, T rhs, sign s) {
    return !(testSign(lhs1, rhs, s)) && !(testSign(lhs2, rhs, s));
}


struct Object {
    int64_t x, m, a, s;
    int64_t& getReference(category r) {
        switch (r) {
        case category::x:
            return x;    
        case category::m:
            return m;  
        case category::a:
            return a;  
        case category::s:
            return s;  
        default:
            std::unreachable();
        }
    }
    bool test(Condition c) {
        const int64_t& r = getReference(c.r);
        bool out = (c.s == greater) ? (r > c.val) : (r < c.val);
        return out;
    }
    int score() {return x+m+a+s;}
};
Object operator-(const Object& lhs, const Object& rhs) {
    return {lhs.x-rhs.x, lhs.m-rhs.m, lhs.a-rhs.a, lhs.s-rhs.s};
}

struct Range {
    Object low {1, 1, 1, 1};
    Object high {4000, 4000, 4000, 4000};
};

struct Workflow;
struct Instruction {
    Condition c;
    Workflow* w;
    status s = processing;
};
struct Workflow {
    std::string name;
    std::vector<Instruction> book;
    Workflow(std::string n) : name(n) {}
    uint64_t process(std::vector<Range> ranges, int idx) const {
        if (idx >= this->book.size()) {return 0;}
        Instruction i = book[idx];
        if (i.s != processing) {
            if (i.s == rejected) {return 0;}
            else {
                uint64_t out = 0;
                for (auto& r : ranges) {
                    Object ll = r.low - Object{1,1,1,1};
                    Object o = r.high - ll;
                    out += o.x*o.m*o.a*o.s;
                }
                return out;
            }
        }
        //find split in range
        int split = i.c.val;
        std::vector<Range> trueRanges, falseRanges;
        
        for (auto& r : ranges) {
            int low = r.low.getReference(i.c.r);
            int high = r.high.getReference(i.c.r);

            if (testBoth(low, high, split, i.c.s)) {
                trueRanges.push_back(r);
            } else if (testNeither(low, high, split, i.c.s)) {
                falseRanges.push_back(r);
            } else {
                Object borderHigh = r.high;
                Object borderlow = r.low;
                if (i.c.s == greater) {
                    borderHigh.getReference(i.c.r) = split;
                    borderlow.getReference(i.c.r) = split+1;
                    falseRanges.push_back({r.low, borderHigh});
                    trueRanges.push_back({borderlow, r.high});
                } else {
                    borderHigh.getReference(i.c.r) = split-1;
                    borderlow.getReference(i.c.r) = split;
                    trueRanges.push_back({r.low, borderHigh});
                    falseRanges.push_back({borderlow, r.high});
                }
            }
        }
        //trueranges + falseranges
        return (i.w->process(trueRanges, 0) + this->process(falseRanges, idx+1));
    }
};




int main() {
    //begin input processing
    std::ifstream winput("input37w.txt", std::ios::in);
    std::map<std::string, std::vector<std::string>> stringMap;
    std::map<std::string, Workflow*> nameToPointer;
    
    Workflow* accept = new Workflow("A");
    accept->book.push_back({{}, nullptr, accepted});
    nameToPointer.insert({accept->name, accept});

    Workflow* reject = new Workflow("R");
    reject->book.push_back({{}, nullptr, rejected});
    nameToPointer.insert({reject->name, reject});

    while (!winput.eof()) {
        std::string line;
        std::getline(winput, line);
        auto inst = process(line, " ");
        Workflow* w = new Workflow(inst[0]);
        nameToPointer.insert({w->name, w});
        auto book = process(inst[1], ",");
        stringMap.insert({w->name, book});
    } winput.close();
    //create instructions 
    for (const auto& [n, b] : stringMap) {
        Workflow* current = nameToPointer[n];
        for (const auto& cond : b) {
            Condition i;
            std::string sto;
            if (cond == b.back()) {
                sto = cond;
            } else {
                auto instruct = process(cond, ":");
                i = {
                    catmap[instruct[0][0]],
                    signmap[instruct[0][1]],
                    std::stoi(instruct[0].substr(2))
                };
                sto = instruct[1];
            }
            Workflow* to = nameToPointer[sto];
            current->book.push_back({i, to});
        }
    }
    //end input processing
    

    Workflow* in = nameToPointer["in"];
    Range full;
    std::cout << in->process({full}, 0) << std::endl;
    
    return 0;
}