#include <iostream>
#include <vector>
#include <stack>
#include <string>
#include <cstdint>

inline constexpr bool is_closing(char c) {
    return c == ')' || c == ']' || c == '}' || c == '>';
}

inline constexpr bool matches(char open, char close) {
    return (open == '(' && close == ')')
        || (open == '[' && close == ']')
        || (open == '{' && close == '}')
        || (open == '<' && close == '>');
}

inline constexpr int64_t points(char c) {
    switch (c) {
        case '(': return 1;
        case '[': return 2;
        case '{': return 3;
        case '<': return 4;
        default:  return 0;
    }
}

int64_t analyze(const std::string &line) {
    int64_t pts = 0;
    std::stack<char> brackets{};
    
    for (char c : line) {
        if (is_closing(c)) {
            if (!matches(brackets.top(), c))
                return 0;
            brackets.pop();
        }
        else {
            brackets.push(c);
        }
    }
    
    while (!brackets.empty()) {
        pts *= 5;
        pts += points(brackets.top());
        brackets.pop();
    }
    
    //std::cout << pts << '\n';
    return pts;
}

int main() {
    int64_t s;
    std::vector<int64_t> scores{};
    std::string line;
    
    while (std::cin >> line)
        if ((s = analyze(line)) != 0)
            scores.push_back(s);
    std::sort(scores.begin(), scores.end());    
        
    std::cout << "score=" << scores[scores.size() / 2] << '\n';
}













