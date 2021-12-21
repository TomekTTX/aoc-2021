#include <iostream>
#include <string>

int main() {
    int depth = 0, pos = 0, amount;
    std::string action;
    
    while (std::cin >> action >> amount) {
        if (action == "forward")
            pos += amount;
        else if (action == "up")
            depth -= amount;
        else if (action == "down")
            depth += amount;
    }
    
    std::cout << "depth=" << depth << "\npos=" << pos << '\n';
    std::cout << "product=" << depth * pos << '\n';
}
