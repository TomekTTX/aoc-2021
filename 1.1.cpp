#include <iostream>

int main() {
    unsigned int elem, prev = ~0U, ctr = 0;

    while (std::cin >> elem) {
        ctr += (elem > prev);
        prev = elem;
    }
    
    std::cout << ctr << '\n';
}
