#include <iostream>
#include <string>

int main() {
    int depth = 0, pos = 0, aim = 0, amount;
    std::string action;
    
    while (std::cin >> action >> amount) {
        if (action == "forward") {
            pos += amount;
            depth += aim * amount;
        }
        else if (action == "up") {
            aim -= amount;
        }
        else if (action == "down") {
            aim += amount;
        }
    }
    
    std::cout << "depth=" << depth << "\npos=" << pos << '\n';
    std::cout << "product=" << depth * pos << '\n';
}
