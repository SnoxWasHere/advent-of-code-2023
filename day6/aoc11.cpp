#include <iostream>
#include <istream>
#include <fstream>
#include <vector>
#include <cstdint>

int main() {
    std::vector<std::pair<int,int>> races {{61, 430}, {67, 1036}, {75, 1307}, {71, 1150}};
    int total = 1;
    for (auto& race : races) {
        int count = 0;
        for (int x = 1; x <= race.first; x++) {
            int vel = x;
            int dist = vel * (race.first - x);
            if (dist > race.second) {count++;}
        }
        if (count != 0) {total *= count;}
    }
    std::cout << total << std::endl;
}