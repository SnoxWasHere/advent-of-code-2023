#include <iostream>
#include <cstdint>

int main() {
    uint64_t steps = 202300;
    uint64_t total = 1;
    uint64_t i1 = 3759;
    uint64_t e1 = 3629;
    uint64_t i2 = 3646;
    uint64_t e2 = 3778;
    for (uint64_t x = 1; x <= steps-1; x++) {
        if ((x % 2) == 0) {total += (x*4)*(i1);}
        else {total += (x*4)*(i2);} 
        total += ((x)*2)*(e1+e2);
    }
    total += (steps)*(i1+e1+i2+e2-2);
    //total += (i1*steps*4);

    std::cout << total << '\n';

}