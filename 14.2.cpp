#include <iostream>
#include <unordered_map>
#include <string>

constexpr const int steps = 40;

inline constexpr short combine(char before, char after) {
    return (short(before) << 8) | after;
}

class PolymerTemplate {
private:
    std::unordered_map<short, char> map{};
public:
    void add(char before, char after, char product) {
        map.insert({combine(before, after), product});
    }

    char get(char before, char after) const {
        const short key = combine(before, after);
        return map.contains(key) ? map.at(key) : '-';
    }

    inline char get(short key) const {
        return map.at(key);
    }
    
    inline const std::unordered_map<short, char>& get_map() const {
        return map;
    }
};

void inc_key(std::unordered_map<short, int64_t> &stat, short key, int64_t amnt = 1) {
    if (stat.contains(key))
        stat[key] += amnt;
    else
        stat[key] = amnt;   
}

inline constexpr short recomb_lo(short key, char prod) {
    return (key & 0xFF00) | prod;
}

inline constexpr short recomb_hi(short key, char prod) {
    return (key & 0xFF) | (short(prod) << 8);
}

std::unordered_map<short, int64_t> get_stat(char &last_char) {
    std::unordered_map<short, int64_t> stat{};
    std::string polym;
    std::cin >> polym;
    
    for (std::size_t i = 0; i < polym.length() - 1; ++i)
        stat[combine(polym[i], polym[i + 1])]++;
    last_char = polym.back();
    
    return stat;
};

int main() {    
    char last;
    PolymerTemplate pol_temp{};
    std::unordered_map<short, int64_t> stat{get_stat(last)}, buffer{};
    
    for (std::string line; std::getline(std::cin, line); )
        pol_temp.add(line[0], line[1], line[6]);
        
    for (int i = 0; i < steps; ++i) {
        for (const auto &[key, cnt] : stat) {
            const char prod = pol_temp.get(key);
            buffer[recomb_lo(key, prod)] += cnt;
            buffer[recomb_hi(key, prod)] += cnt;
        }
        stat = buffer;
        buffer.clear();
    }
    
    buffer[last]++;
    for (const auto &[key, cnt] : stat)
        buffer[key >> 8] += cnt;
    for (const auto &[key, cnt] : buffer)
        std::cout << char(key) << " -> " << cnt << '\n';
}

/*


void print_pair(short key) {
    std::cout << char(key >> 8) << char(key);
}

void print_stat(const std::unordered_map<short, int64_t> &stat) {
    for (const auto &[key, cnt] : stat) {
        print_pair(key);
        std::cout << " -> " << cnt << '\n';
    }
    std::cout << '\n';
}

*/














