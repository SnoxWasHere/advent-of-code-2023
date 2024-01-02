#include <iostream>
#include <istream>
#include <fstream>
#include <vector>
#include <cstdint>
#include <optional>
#include <queue>
#include <map>

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

enum pulse {
    low,
    high,
    none
};
struct Module;
struct Instruction {
    Module* m;
    pulse p;
};

struct Module {
    std::string name;
    std::vector<Module*> dest;
    Module(std::string n, std::vector<Module*> d = {}) : name(n), dest(d) {}
    virtual std::vector<Instruction> call(const Instruction& i) {
        return sendAll(i.p);
    }
    virtual void updateParams(Module* m) {}
    virtual std::vector<Instruction> sendAll(pulse s) {
        std::vector<Instruction> out;
        for (const auto& m : dest) {
            out.push_back({m, s});
        }
        return out;
    }
};

struct FlipFlop : Module {
    bool status = false;
    FlipFlop(std::string n, std::vector<Module*> d = {}) : Module(n, d) {}
    std::vector<Instruction> call(const Instruction& i) override {
        if (i.p == pulse::high) {return {};}
        status = !status;
        pulse out = (status) ? pulse::high : pulse::low;
        return sendAll(out);
    }
};

struct Conjunction : Module {
    std::map<Module*, pulse> memory;
    Conjunction(std::string n, std::vector<Module*> d = {}) : Module(n, d) {}
    void updateParams(Module* m) override {
        if (memory.count(m) == 0) {memory.insert({m, pulse::low});}
    }
    std::vector<Instruction> call(const Instruction& i) override {
        if (memory.count(i.m) == 0) {memory.insert({i.m, pulse::low});}
        memory[i.m] = i.p; 
        pulse allHigh = pulse::low;
        for (const auto& [mm, mp] : memory) {
            if (mp != pulse::high) {allHigh = pulse::high; break;}
        }
        return sendAll(allHigh);
    }
};

struct Output : Module {
    Output(std::string n, std::vector<Module*> d = {}) : Module(n, d) {}
    std::vector<Instruction> call(const Instruction& i) override {
        return {};
    }
};


class Network {
private:
    Module* button;
    void printInstruction(const std::pair<Module*, Instruction>& i) {
        if (i.second.m->name != "^rx") {return;}
        std::cout << i.first->name << " -" << i.second.p << "-> ";
        std::cout << i.second.m->name << " ";
        std::cout << std::endl;
    }
public:
    Network (Module* b) : button(b) {}
    std::pair<uint64_t, uint64_t> run() {
        std::queue<std::pair<Module*, Instruction>> actions;
        uint64_t low = 0, high = 0;
        actions.push({button, {button, pulse::low}});
        do {
            printInstruction(actions.front());
            auto [parent, current] = actions.front();
            actions.pop();
            if (current.p == pulse::low) {low++;}
            else {high++;}
            
            std::vector<Instruction> out = current.m->call({parent, current.p});
            for (auto& i : out) {actions.push({current.m, i});}
        } while (actions.size() > 0);
        return {low, high};
    }
};

int main() {
    std::ifstream input("input39.txt", std::ios::in);
    std::map<std::string, std::vector<std::string>> stringMap;
    std::map<std::string, Module*> nameToPointer;

    while (!input.eof()) {
        std::string line;
        std::getline(input, line);
        auto inst = process(line, " ");
        stringMap.insert({inst[0].substr(1), process(inst[2], ",")});

        Module* current;
        if (inst[0][0] == '%') {
            current = new FlipFlop(inst[0]);
        } else if (inst[0][0] == '&') {
            current = new Conjunction(inst[0]);
        } else if (inst[0][0] == '^') {
            current = new Output(inst[0]);
        } else {
            current = new Module(inst[0]);
        }
        nameToPointer.insert({inst[0].substr(1), current});

    } input.close();
    for (const auto& [mName, mDest] : stringMap) {
        Module* current = nameToPointer[mName];
        for (const auto& sn : mDest) {
            if (nameToPointer.count(sn) != 0) {
                current->dest.push_back(nameToPointer[sn]);
                current->dest.back()->updateParams(current);
            }
        }
    }

    Network n(nameToPointer["broadcaster"]);
    uint64_t low = 0, high = 0;
    for (int x = 0; x < 1000; x++) {
        auto [l, h] = n.run();
        low += l;
        high += h;
        std::cout << low*high << std::endl;
    }
    

    for (const auto& [mName, mPointer] : nameToPointer) {
        delete mPointer;
    }
    return 0;
}