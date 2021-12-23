#include <iostream>
#include <array>

constexpr const int size = 10;

struct Point {
    int x, y;
    
    std::array<Point, 8> neighbours() const {
        return {{
            {x - 1, y - 1},
            {x    , y - 1},
            {x + 1, y - 1},
            {x - 1, y    },
            {x + 1, y    },
            {x - 1, y + 1},
            {x    , y + 1},
            {x + 1, y + 1},
        }};
    }
    
};

struct Octopus {
    short energy;
    bool flashed = false;
    
    void increment() {
        if (!flashed)
            energy++;
    }
};

class OctopusMatrix {
private:
    int total_flashes = 0;
    std::array<Octopus, size * size> vals{};
public:
    inline int total() const { return total_flashes; }

    void step() {
        int flashes;
        
        for (auto &v : vals)
            v.energy += 1;
        while ((flashes = do_flashes()))
            total_flashes += flashes;       
        for (auto &v : vals)
            v.flashed = false;
    }
    
    void step(unsigned int count, bool log = false) {       
        if (log) std::cout << "\nStep 0:" << *this << '\n';
        for (unsigned int i = 0; i < count; ++i) {
            step();
            if (log) std::cout << "\nStep " << i + 1 << ':' << *this << '\n';
        }
    }
    
    friend std::istream& operator>>(std::istream &ist, OctopusMatrix &om) {
        char c;
        
        for (int i = 0; i < (int)om.vals.size(); ++i) {
            ist >> c;
            om.vals[i] = Octopus(c & 0xF);
        }           
        
        return ist;
    }
    
    friend std::ostream& operator<<(std::ostream &ost, const OctopusMatrix &om) {
        for (int i = 0; i < (int)om.vals.size(); ++i) {
            if (i % size == 0)
                ost << '\n';
            ost << om.vals[i].energy;
        }           
        
        ost << '\n';
        return ost;
    }
private:
    inline Octopus &at(Point p) { return at(p.x, p.y); }
    inline Octopus &at(int x, int y) { return vals[y * size + x]; } 
    inline const Octopus &at(Point p) const { return at(p.x, p.y); }
    inline const Octopus &at(int x, int y) const { return vals[y * size + x]; }
    inline bool in_boundary(Point p) const { return in_boundary(p.x, p.y); }
    inline bool in_boundary(int x, int y) const { 
        return x >= 0 && y >= 0 && x < size && y < size;
    }
    
    int do_flashes() {
        int flashes = 0;
        
        for (int y = 0; y < size; ++y) {
            for (int x = 0; x < size; ++x) {
                Octopus &oct = at(x, y);
                
                if (!oct.flashed && oct.energy > 9) {
                    oct.energy = 0;
                    oct.flashed = true;
                    flashes += 1;
                    
                    for (const auto &pos : Point(x, y).neighbours())
                        if (in_boundary(pos))
                            at(pos).increment();
                }
            }
        }

        return flashes;
    }
};

int main() {
    OctopusMatrix matrix{};
    std::cin >> matrix;
    
    matrix.step(100);
    std::cout << matrix << '\n' << "total flashes = " << matrix.total() << '\n';
}

























