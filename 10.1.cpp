#include <iostream>
#include <stack>
#include <string>

inline constexpr bool is_closing(char c) {
    return c == ')' || c == ']' || c == '}' || c == '>';
}

inline constexpr bool matches(char open, char close) {
    return (open == '(' && close == ')')
        || (open == '[' && close == ']')
        || (open == '{' && close == '}')
        || (open == '<' && close == '>');
}

inline constexpr int points(char c) {
    switch (c) {
        case ')': return 3;
        case ']': return 57;
        case '}': return 1197;
        case '>': return 25137;
        default:  return 0;
    }
}

int analyze(const std::string &line) {
    std::stack<char> brackets{};
    
    for (char c : line) {
        if (is_closing(c)) {
            if (!matches(brackets.top(), c))
                return points(c);
            brackets.pop();
        }
        else {
            brackets.push(c);
        }
    }
    
    return 0;
}

int main() {
    int score = 0;
    std::string line;
    
    while (std::cin >> line)
        score += analyze(line);
        
    std::cout << "score=" << score << '\n';
}













