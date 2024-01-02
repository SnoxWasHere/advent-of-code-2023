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

struct Object {
    int x, m, a, s;
    int& getReference(category r) {
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
        const int& r = getReference(c.r);
        bool out = (c.s == greater) ? (r > c.val) : (r < c.val);
        return out;
    }
    int score() {return x+m+a+s;}
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
    status process(Object o) const {
        for (const auto& i : book) {
            if (o.test(i.c)) { //if pass test
                if (i.s != processing) {return i.s;} //if A or R
                else {return i.w->process(o);} //else send to workflow
            } 
        }
        return processing;
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
    std::ifstream oinput("input37o.txt", std::ios::in);
    std::vector<Object> objects;
    while (!oinput.eof()) {
        std::string line;
        std::getline(oinput, line);
        auto params = process(line, ",");
        Object o;
        for (const auto& param : params) {
            int& c = o.getReference(catmap[param[0]]);
            c = std::stoi(param.substr(2));
        }
        objects.push_back(o);
    } oinput.close();
    //end input processing

    uint64_t total = 0;
    Workflow* in = nameToPointer["in"];
    for (auto& o : objects) {
        if(in->process(o) == accepted) {total += o.score();}
    }
    std::cout << total << std::endl;
    
    return 0;
}