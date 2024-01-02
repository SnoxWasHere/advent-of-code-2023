#include <iostream>
#include <vector>


struct Base {
    int x;
    virtual void calc() {std::cout << "b";};
    virtual void ping() {calc();}
};

struct Derived : Base {
    void ping() override {
        std::cout << "h";
        calc();
    }
};

int main() {
    Base* d = new Derived;
    d->ping();
    return 0;
}