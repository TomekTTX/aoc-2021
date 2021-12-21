#include <iostream>
#include <algorithm>

#include <array>
#include <string>
#include <bitset>
#include <unordered_map>

class NumberResolver {
private:
    std::array<std::bitset<7>, 7> seg{{}};
    std::unordered_map<std::bitset<7>, short> digit_map{};
public:
    void process_input(const std::array<std::string, 10>& in) {
        const std::array<std::bitset<7>, 10> bits = get_bitsets(in);
        
        seg[0] = bits[0] ^ bits[1];
        seg[3] = bits[2] & bits[3] & bits[4] & bits[5];
        seg[6] = (bits[3] & bits[4] & bits[5]) ^ (seg[0] | seg[3]);
        seg[1] = (bits[6] & bits[7] & bits[8] & ~bits[1]) ^ seg[6];
        seg[4] = (bits[3] ^ bits[4] ^ bits[5]) ^ (seg[0] | seg[1] | seg[3] | seg[6]);
        seg[5] = (bits[6] & bits[7] & bits[8]) ^ (seg[0] | seg[1] | seg[6]);
        seg[2] = ~(seg[0] | seg[1] | seg[3] | seg[4] | seg[5] | seg[6]);
            
        map_digits();
    }
    
    short resolve(const std::string &s) {
        const std::bitset<7> bitset = get_bitset(s);
        return digit_map.contains(bitset) ? digit_map[bitset] : -1;
    }
private:
    void map_digits() {
        const std::array<std::bitset<7>, 10> digits{
            ~seg[3],                           // 0
            seg[2] | seg[5],                   // 1
            ~(seg[1] | seg[5]),                // 2
            ~(seg[1] | seg[4]),                // 3
            seg[1] | seg[2] | seg[3] | seg[5], // 4
            ~(seg[2] | seg[4]),                // 5
            ~seg[2],                           // 6
            seg[0] | seg[2] | seg[5],          // 7
            ~0,                                // 8
            ~seg[4],                           // 9
        };
        
        short dig = 0;
        for (const auto &digit : digits)
            digit_map[digit] = dig++;
    }

    static std::array<std::bitset<7>, 10> get_bitsets(const std::array<std::string, 10>& in) {  
        std::array<std::bitset<7>, 10> bitsets{};
        
        for (std::size_t i = 0; i < in.size(); ++i)
            bitsets[i] = get_bitset(in[i]);
        std::sort(bitsets.begin(), bitsets.end(), [](auto a, auto b){ return a.count() < b.count(); });
        
        return bitsets;
    }
    
    static std::bitset<7> get_bitset(const std::string &s) {
        std::bitset<7> ret{ 0 };
        for (char c : s) {
            ret.set('g' - c);
        }
        return ret;
    }
};
 
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
    NumberResolver resolver{};
    Input input{};
    std::size_t sum = 0, mult;
    
    while (std::cin >> input) {
        resolver.process_input(input.in);
        
        mult = 1000;
        for (const auto &s : input.test) {
             sum += resolver.resolve(s) * mult;
             mult /= 10;
        }
    }
    std::cout << sum << '\n';
}









