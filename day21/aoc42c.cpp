#include <iostream>
#include <cstdint>

uint64_t solve(uint64_t n){
    uint64_t i1 = 3759;
    uint64_t e1 = 3629;
    uint64_t i2 = 3646;
    uint64_t e2 = 3778;
    
    return ((n-1)*(n-1)*(i1+e1) + n*n*(i2+e2) + 3*(n-1)/4);                    
}

int main() {
    uint64_t steps = 202300;
    uint64_t total = 1;
    

    std::cout << total << '\n';

}