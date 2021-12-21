#include <iostream>
#include <algorithm>

#include <array>
#include <string>

struct Input {
    std::array<std::string, 10> in{};
    std::array<std::string, 4> test{};
        
    friend std::istream& operator>>(std::istream &ist, Input &inp) {
        char d;
        for (auto &a : inp.in)
            ist >> a;
        ist >> d;
        for (auto &a : inp.test)
            ist >> a;
        return ist;
    }
};

int main() {
    Input input{};
    std::size_t counter = 0;
    
    while (std::cin >> input)
        for (const auto &s : input.test)
            counter += (s.length() != 5 && s.length() != 6);
    
    std::cout << counter << '\n';
}

